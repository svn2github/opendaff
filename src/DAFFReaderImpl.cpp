#include "DAFFReaderImpl.h"

#include "DAFFHeader.h"
#include "DAFFMetadataImpl.h"
#include <DAFFUtils.h>

#include "Utils.h"

#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <sstream>

// Disable MSVC security warning for unsafe fopen
#ifdef _MSC_VER
#pragma warning(disable: 4996)
#endif // _MSC_VER


DAFFReaderImpl::DAFFReaderImpl()
	: m_bDAFFObjectValid( false )
	, m_bDAFFObjectFromFileValid( false )
	, m_file( NULL ),
	m_pFileBlockTable( NULL ),
	m_pMainHeader( NULL ),
	m_pContentHeader( NULL ),
	m_pRecordDescriptorBlock( NULL ),
	m_pDataBlock( NULL ),
	m_bOverallPeakInitialized( false ),
	m_fOverallPeak( 0.0 )
{
	m_pEmptyMetadata = new DAFFMetadataImpl;
}

DAFFReaderImpl::~DAFFReaderImpl()
{
	tidyup();
	delete m_pEmptyMetadata;
}

bool DAFFReaderImpl::isFileOpened() const
{
	return ( m_bDAFFObjectFromFileValid && m_bDAFFObjectValid );
}

bool DAFFReaderImpl::isValid() const
{
	return m_bDAFFObjectValid;
}

int DAFFReaderImpl::deserialize( char* pDAFFDataBuffer )
{
	if( m_bDAFFObjectValid )
		return DAFF_MODAL_ERROR;

	// File header
	char* pFileHeaderBuffer = ( char* ) &m_fileHeader;
	for( size_t i = 0; i < sizeof( DAFFFileHeader ); i++ )
		pFileHeaderBuffer[ i ] = pDAFFDataBuffer[ i ];

	int ec = loadFileHeader();
	if( ec != DAFF_NO_ERROR )
		return ec;

	size_t nBufferReadPos = sizeof( DAFFFileHeader );

	// File block table
	size_t nFileBlockTableSize = m_fileHeader.iNumFileBlocks * sizeof( DAFFFileBlockEntry );
	m_pFileBlockTable = ( DAFFFileBlockEntry* ) DAFF::malloc_aligned16( nFileBlockTableSize );
	char* pFileBlockTableBuffer = ( char* ) m_pFileBlockTable;

	for( size_t i = 0; i < nFileBlockTableSize; i++ )
		pFileBlockTableBuffer[ i ] = pDAFFDataBuffer[ i + nBufferReadPos ];

	ec = loadFileBlockTable();
	if( ec != DAFF_NO_ERROR )
	{
		DAFF::free_aligned16( m_pFileBlockTable );
		tidyup();
		return ec;
	}

	nBufferReadPos += nFileBlockTableSize;

	// Main header
	DAFFFileBlockEntry* pfbMainHeader;
	if( getFirstFileBlockByID( FILEBLOCK_DAFF1_MAIN_HEADER_ID, pfbMainHeader ) != 1 )
	{
		tidyup();
		return DAFF_FILE_INVALID;
	}

	m_pMainHeader = ( DAFFMainHeader* ) DAFF::malloc_aligned16( sizeof( DAFFMainHeader ) );
	char* pMainHeaderBuffer = ( char* ) m_pMainHeader;

	for( size_t i = 0; i < sizeof( DAFFMainHeader ); i++ )
		pMainHeaderBuffer[ i ] = pDAFFDataBuffer[ i + pfbMainHeader->ui64Offset ];

	ec = loadMainHeader();
	if( ec != DAFF_NO_ERROR )
	{
		DAFF::free_aligned16( m_pMainHeader );
		tidyup();
		return ec;
	}

	// Content header
	DAFFFileBlockEntry* pfbContentHeader;
	if( getFirstFileBlockByID( FILEBLOCK_DAFF1_CONTENT_HEADER_ID, pfbContentHeader ) != 1 )
	{
		tidyup();
		return DAFF_FILE_CORRUPTED;
	}

	m_pContentHeader = ( char* ) DAFF::malloc_aligned16( ( size_t ) pfbContentHeader->ui64Size );
	char* pContentHeaderBuffer = ( char* ) m_pContentHeader;

	for( size_t i = 0; i < ( size_t ) pfbContentHeader->ui64Size; i++ )
		pContentHeaderBuffer[ i ] = pDAFFDataBuffer[ i + pfbContentHeader->ui64Offset ];

	ec = loadContentHeader();
	if( ec != DAFF_NO_ERROR )
	{
		DAFF::free_aligned16( m_pContentHeader );
		tidyup();
		return ec;
	}

	// Record descriptor
	if( getFirstFileBlockByID( FILEBLOCK_DAFF1_RECORD_DESC_ID, m_pRecordDescriptorTable ) != 1 )
	{
		tidyup();
		return DAFF_FILE_CORRUPTED;
	}

	m_pRecordDescriptorBlock = ( char* ) DAFF::malloc_aligned16( ( size_t ) m_pRecordDescriptorTable->ui64Size );
	char* pRecordDescriptorBuffer = ( char* ) m_pRecordDescriptorBlock;

	for( size_t i = 0; i < ( size_t ) m_pRecordDescriptorTable->ui64Size; i++ )
		pRecordDescriptorBuffer[ i ] = pDAFFDataBuffer[ i + m_pRecordDescriptorTable->ui64Offset ];

	ec = loadRecordDescriptor();
	if( ec != DAFF_NO_ERROR )
	{
		DAFF::free_aligned16( m_pRecordDescriptorBlock );
		tidyup();
		return ec;
	}

	// Record data
	if( getFirstFileBlockByID( FILEBLOCK_DAFF1_DATA_ID, m_pDataFileBlock ) != 1 )
	{
		tidyup();
		return DAFF_FILE_CORRUPTED;
	}

	m_pDataBlock = DAFF::malloc_aligned16( ( size_t ) m_pDataFileBlock->ui64Size );
	char* pDataBlockBuffer = ( char* ) m_pDataBlock;

	for( size_t i = 0; i < ( size_t ) m_pDataFileBlock->ui64Size; i++ )
		pDataBlockBuffer[ i ] = pDAFFDataBuffer[ i + m_pDataFileBlock->ui64Offset ];

	ec = loadRecordData();
	if( ec != DAFF_NO_ERROR )
	{
		DAFF::free_aligned16( m_pDataBlock );
		tidyup();
		return ec;
	}

	// Metadata
	DAFFFileBlockEntry* pMetadataFileBlock = NULL;
	if( getFirstFileBlockByID( FILEBLOCK_DAFF1_METADATA_ID, pMetadataFileBlock ) > 1 )
	{
		tidyup();
		return DAFF_FILE_CORRUPTED;
	}

	if( pMetadataFileBlock == nullptr )
	{
		m_vpMetadata.push_back( new DAFFMetadataImpl ); // Empty
	}
	else if( pMetadataFileBlock->ui64Size == 0 )
	{
		m_vpMetadata.push_back( new DAFFMetadataImpl ); // Empty  )
	}
	else
	{
		// Metadata block present
		void* pMetadataBuf = DAFF::malloc_aligned16( ( size_t ) pMetadataFileBlock->ui64Size );
		char* pMetadataBuffer = ( char* ) pMetadataBuf;

		for( size_t i = 0; i < ( size_t ) pMetadataFileBlock->ui64Size; i++ )
			pMetadataBuffer[ i ] = pDAFFDataBuffer[ i + pMetadataFileBlock->ui64Offset ];

		ec = loadMetadata( pMetadataBuffer ); // Converts to internal representation

		DAFF::free_aligned16( pMetadataBuf );

		if( ec != DAFF_NO_ERROR )
		{
			tidyup();
			return ec;
		}

	}

	fixAngleRanges();

	// ... done.

	m_bDAFFObjectFromFileValid = false;
	m_bDAFFObjectValid = true;

	return DAFF_NO_ERROR;
}

int DAFFReaderImpl::openFile( const std::string& sFilePath )
{
	if( m_bDAFFObjectValid )
		return DAFF_MODAL_ERROR;

	m_file = fopen( sFilePath.c_str(), "rb" );
	if( !m_file )
		return DAFF_FILE_NOT_FOUND;

	// File header
	if( fread( &m_fileHeader, 1, sizeof( DAFFFileHeader ), m_file ) != sizeof( DAFFFileHeader ) )
	{
		tidyup();
		return DAFF_FILE_INVALID;
	}

	int ec = loadFileHeader();
	if( ec != DAFF_NO_ERROR )
		return ec;

	// File block table
	size_t iFileBlockTableSize = m_fileHeader.iNumFileBlocks * sizeof( DAFFFileBlockEntry );
	m_pFileBlockTable = ( DAFFFileBlockEntry* ) DAFF::malloc_aligned16( iFileBlockTableSize );
	if( fread( m_pFileBlockTable, 1, iFileBlockTableSize, m_file ) != iFileBlockTableSize )
	{
		tidyup();
		return DAFF_FILE_INVALID;
	}

	ec = loadFileBlockTable();
	if( ec != DAFF_NO_ERROR )
		return ec;

	// Main header
	DAFFFileBlockEntry* pfbMainHeader;
	if( getFirstFileBlockByID( FILEBLOCK_DAFF1_MAIN_HEADER_ID, pfbMainHeader ) != 1 )
	{
		tidyup();
		return DAFF_FILE_INVALID;
	}

	m_pMainHeader = ( DAFFMainHeader* ) DAFF::malloc_aligned16( sizeof( DAFFMainHeader ) );
	fseek( m_file, ( long ) pfbMainHeader->ui64Offset, SEEK_SET );
	if( fread( m_pMainHeader, 1, sizeof( DAFFMainHeader ), m_file ) != sizeof( DAFFMainHeader ) )
	{
		tidyup();
		return DAFF_FILE_INVALID;
	}

	ec = loadMainHeader();
	if( ec != DAFF_NO_ERROR )
		return ec;

	// Content header
	DAFFFileBlockEntry* pfbContentHeader;
	if( getFirstFileBlockByID( FILEBLOCK_DAFF1_CONTENT_HEADER_ID, pfbContentHeader ) != 1 )
	{
		tidyup();
		return DAFF_FILE_CORRUPTED;
	}

	m_pContentHeader = ( char* ) DAFF::malloc_aligned16( ( size_t ) pfbContentHeader->ui64Size );
	fseek( m_file, ( long ) pfbContentHeader->ui64Offset, SEEK_SET );
	if( fread( m_pContentHeader, 1, ( size_t ) pfbContentHeader->ui64Size, m_file ) != ( size_t ) pfbContentHeader->ui64Size )
	{
		tidyup();
		return DAFF_FILE_CORRUPTED;
	}

	ec = loadContentHeader();
	if( ec != DAFF_NO_ERROR )
		return ec;

	// Record descriptor
	if( getFirstFileBlockByID( FILEBLOCK_DAFF1_RECORD_DESC_ID, m_pRecordDescriptorTable ) != 1 )
	{
		tidyup();
		return DAFF_FILE_CORRUPTED;
	}

	m_pRecordDescriptorBlock = ( char* ) DAFF::malloc_aligned16( ( size_t ) m_pRecordDescriptorTable->ui64Size );
	fseek( m_file, ( long ) m_pRecordDescriptorTable->ui64Offset, SEEK_SET );
	if( fread( m_pRecordDescriptorBlock, 1, ( size_t ) m_pRecordDescriptorTable->ui64Size, m_file ) != ( size_t ) m_pRecordDescriptorTable->ui64Size )
	{
		tidyup();
		return DAFF_FILE_CORRUPTED;
	}

	ec = loadRecordDescriptor();
	if( ec != DAFF_NO_ERROR )
		return ec;

	// Record data
	if( getFirstFileBlockByID( FILEBLOCK_DAFF1_DATA_ID, m_pDataFileBlock ) != 1 )
	{
		tidyup();
		return DAFF_FILE_CORRUPTED;
	}

	m_pDataBlock = DAFF::malloc_aligned16( ( size_t ) m_pDataFileBlock->ui64Size );
	fseek( m_file, ( long ) m_pDataFileBlock->ui64Offset, SEEK_SET );
	if( fread( m_pDataBlock, 1, ( size_t ) m_pDataFileBlock->ui64Size, m_file ) != ( size_t ) m_pDataFileBlock->ui64Size )
	{
		DAFF::free_aligned16( m_pDataBlock );
		tidyup();
		return DAFF_FILE_CORRUPTED;
	}

	ec = loadRecordData();
	if( ec != DAFF_NO_ERROR )
		return ec;

	// Metadata
	DAFFFileBlockEntry* pMetadataFileBlock = NULL;
	if( getFirstFileBlockByID( FILEBLOCK_DAFF1_METADATA_ID, pMetadataFileBlock ) > 1 )
	{
		tidyup();
		return DAFF_FILE_CORRUPTED;
	}

	if( pMetadataFileBlock == nullptr )
	{
		m_vpMetadata.push_back( new DAFFMetadataImpl ); // Empty
	}
	else if( pMetadataFileBlock->ui64Size == 0 )
	{
		m_vpMetadata.push_back( new DAFFMetadataImpl ); // Empty  )
	}
	else
	{
		// Metadata block present
		void* pMetadataBuf = DAFF::malloc_aligned16( ( size_t ) pMetadataFileBlock->ui64Size );
		fseek( m_file, ( long ) pMetadataFileBlock->ui64Offset, SEEK_SET );
		if( fread( pMetadataBuf, 1, ( size_t ) pMetadataFileBlock->ui64Size, m_file ) != ( size_t ) pMetadataFileBlock->ui64Size )
		{
			DAFF::free_aligned16( pMetadataBuf );
			tidyup();
			return DAFF_FILE_CORRUPTED;
		}

		ec = loadMetadata( ( char* ) pMetadataBuf );
		if( ec != DAFF_NO_ERROR )
			return ec;

		DAFF::free_aligned16( pMetadataBuf );
	}

	fixAngleRanges();

	fclose( m_file );
	m_file = NULL;

	// ... done.

	m_sFilePath = sFilePath;
	m_bDAFFObjectValid = true;
	m_bDAFFObjectFromFileValid = true;

	return DAFF_NO_ERROR;
}

int DAFFReaderImpl::loadFileHeader()
{
	/*
	 *  1st step: Load the file header and validate it for correctness
	 */

	m_fileHeader.fixEndianness();

	// Check signature
	if( !( ( m_fileHeader.pcSignature[ 0 ] == 'F' ) && ( m_fileHeader.pcSignature[ 1 ] == 'W' ) ) )
	{
		// File is not an OpenDAFF database.
		tidyup();
		return DAFF_FILE_INVALID;
	}

	// Check version
	// [This implementation supports version number 1.7]
	if( m_fileHeader.iFileFormatVersion != 170 )
	{
		// Database version not supported
		tidyup();
		return DAFF_FILE_FORMAT_VERSION_UNSUPPORTED;
	}

	// Check file block entries
	if( m_fileHeader.iNumFileBlocks <= 0 )
	{
		// There must be at least one block
		tidyup();
		return DAFF_FILE_INVALID;
	}

	return DAFF_NO_ERROR;
}

int DAFFReaderImpl::loadFileBlockTable()
{
	// Read the file block table
	size_t iFileBlockTableSize = m_fileHeader.iNumFileBlocks * sizeof( DAFFFileBlockEntry );

	for( int i = 0; i < m_fileHeader.iNumFileBlocks; i++ )
		m_pFileBlockTable[ i ].fixEndianness();

#if ( DAFF_DEBUG == 1 )
	for( int i=0; i<m_fileHeader.iNumFileBlocks; i++ )
		printf( "FILE BLOCK[%d] = { ID = 0x%04X, offset = %llu, size = %llu bytes }\n",
		i, m_pFileBlockTable[i].iID, m_pFileBlockTable[i].ui64Offset, m_pFileBlockTable[i].ui64Size );
#endif

	// Check for correctness
	for( int i = 0; i < m_fileHeader.iNumFileBlocks; i++ )
	{
		// Note: IDs are not checked
		if( ( m_pFileBlockTable[ i ].ui64Offset < ( sizeof( DAFFFileHeader ) + iFileBlockTableSize ) ) )
		{
			tidyup();
			return DAFF_FILE_INVALID;
		}
	}

	return DAFF_NO_ERROR;
}

int DAFFReaderImpl::loadMainHeader()
{
	/*
	 *  2nd step: Load the main header, validate it for correctness
	 */

	m_pMainHeader->fixEndianness();

	// Content type
	switch( m_pMainHeader->iContentType )
	{
	case DAFF_IMPULSE_RESPONSE:
	case DAFF_MAGNITUDE_SPECTRUM:
	case DAFF_PHASE_SPECTRUM:
	case DAFF_MAGNITUDE_PHASE_SPECTRUM:
	case DAFF_DFT_SPECTRUM:
		break;

	default:
		// Invalid content type
		tidyup();
		return DAFF_FILE_CONTENT_TYPE_UNKOWN;
	};

	// Quantization
	switch( m_pMainHeader->iQuantization )
	{
	case DAFF_INT16:
	case DAFF_INT24:
	case DAFF_FLOAT32:
		break;

	default:
		// Invalid quantization
		tidyup();
		return DAFF_FILE_QUANTIZATION_UNKOWN;
	};

	if( m_pMainHeader->iNumChannels < 1 )
		return DAFF_FILE_INVALID_MAIN_PARAMETER;

	if( m_pMainHeader->iNumRecords < 1 )
		return DAFF_FILE_INVALID_MAIN_PARAMETER;

	if( m_pMainHeader->iElementsPerRecord < 1 )
		return DAFF_FILE_INVALID_MAIN_PARAMETER;

	if( m_pMainHeader->iAlphaPoints < 1 )
		return DAFF_FILE_ALPHA_ANGLES_INVALID;

	// alpha start value must not be 360&deg;
	if( ( m_pMainHeader->fAlphaStart < 0.0f ) || ( m_pMainHeader->fAlphaStart >= 360.0f ) )
		return DAFF_FILE_ALPHA_ANGLES_INVALID;

	// alpha stop value may be 360&deg; indicating that the full alpha range is covered.
	if( ( m_pMainHeader->fAlphaEnd < 0.0f ) || ( m_pMainHeader->fAlphaEnd > 360.0f ) )
		return DAFF_FILE_ALPHA_ANGLES_INVALID;

	// Beta angle validation
	if( m_pMainHeader->iBetaPoints < 1 )
		return DAFF_FILE_BETA_ANGLES_INVALID;

	if( m_pMainHeader->fBetaStart > m_pMainHeader->fBetaEnd )
		return DAFF_FILE_BETA_ANGLES_INVALID;

	if( ( m_pMainHeader->fBetaStart < 0.0f ) || ( m_pMainHeader->fBetaStart > 180.0f ) )
		return DAFF_FILE_BETA_ANGLES_INVALID;

	if( ( m_pMainHeader->fBetaEnd < 0.0f ) || ( m_pMainHeader->fBetaEnd > 180.0f ) )
		return DAFF_FILE_BETA_ANGLES_INVALID;

	// Orientation
	m_orientationDefault.fYawAngleDeg = m_pMainHeader->fOrientYaw;
	m_orientationDefault.fPitchAngleDeg = m_pMainHeader->fOrientPitch;
	m_orientationDefault.fRollAngleDeg = m_pMainHeader->fOrientRoll;
	m_tTrans.setOrientation( m_orientationDefault );

	return DAFF_NO_ERROR;
}

int DAFFReaderImpl::loadContentHeader()
{
	/*
	 *  3rd step: Load the content header, validate it for correctness
	 */

	float* pfFreqs = 0;

	switch( m_pMainHeader->iContentType )
	{
	case DAFF_IMPULSE_RESPONSE:
		m_pContentHeaderIR = static_cast< DAFFContentHeaderIR* >( m_pContentHeader );
		m_pContentHeaderIR->fixEndianness();

		if( m_pContentHeaderIR->fSamplerate < 0.0f )
			return DAFF_FILE_CONTENT_INVALID_PARAMETER;

		if( ( m_pContentHeaderIR->iMaxEffectiveFilterLength < 0 ) ||
			( m_pContentHeaderIR->iMaxEffectiveFilterLength > m_pMainHeader->iElementsPerRecord ) )
			return DAFF_FILE_CONTENT_INVALID_PARAMETER;

		if( ( m_pContentHeaderIR->iMinFilterOffset < 0 ) ||
			( m_pContentHeaderIR->iMinFilterOffset > m_pMainHeader->iElementsPerRecord ) )
			return DAFF_FILE_CONTENT_INVALID_PARAMETER;

		break;

	case DAFF_MAGNITUDE_SPECTRUM:
		m_pContentHeaderMS = static_cast< DAFFContentHeaderMS* >( m_pContentHeader );
		m_pContentHeaderMS->fixEndianness();

		if( m_pContentHeaderMS->iNumFreqs <= 0 )
			return DAFF_FILE_CONTENT_INVALID_PARAMETER;

		pfFreqs = ( float* ) ( ( char* ) m_pContentHeader + 8 );

		// Fix the endianness of the frequency list
		DAFF::le2se_4byte( pfFreqs, m_pContentHeaderMS->iNumFreqs );

		// Load the (dynamically sized) frequency list
		m_vfFreqs.resize( m_pContentHeaderMS->iNumFreqs );
		for( int i = 0; i < m_pContentHeaderMS->iNumFreqs; i++ )
			m_vfFreqs[ i ] = pfFreqs[ i ];

		break;

	case DAFF_PHASE_SPECTRUM:
		m_pContentHeaderPS = static_cast< DAFFContentHeaderPS* >( m_pContentHeader );
		m_pContentHeaderPS->fixEndianness();

		if( m_pContentHeaderPS->iNumFreqs <= 0 )
			return DAFF_FILE_CONTENT_INVALID_PARAMETER;

		pfFreqs = ( float* ) ( ( char* ) m_pContentHeader + 8 );

		// Fix the endianness of the frequency list
		DAFF::le2se_4byte( pfFreqs, m_pContentHeaderPS->iNumFreqs );

		// Load the (dynamically sized) frequency list
		m_vfFreqs.resize( m_pContentHeaderPS->iNumFreqs );
		for( int i = 0; i < m_pContentHeaderPS->iNumFreqs; i++ ) m_vfFreqs[ i ] = pfFreqs[ i ];

		break;

	case DAFF_MAGNITUDE_PHASE_SPECTRUM:
		m_pContentHeaderMPS = static_cast< DAFFContentHeaderMPS* >( m_pContentHeader );
		m_pContentHeaderMPS->fixEndianness();

		if( m_pContentHeaderMPS->iNumFreqs <= 0 )
			return DAFF_FILE_CONTENT_INVALID_PARAMETER;

		pfFreqs = ( float* ) ( ( char* ) m_pContentHeader + 8 );

		// Fix the endianness of the frequency list
		DAFF::le2se_4byte( pfFreqs, m_pContentHeaderMPS->iNumFreqs );

		// Load the (dynamically sized) frequency list
		m_vfFreqs.resize( m_pContentHeaderMPS->iNumFreqs );
		for( int i = 0; i < m_pContentHeaderMPS->iNumFreqs; i++ ) m_vfFreqs[ i ] = pfFreqs[ i ];

		break;

	case DAFF_DFT_SPECTRUM:
		m_pContentHeaderDFT = static_cast< DAFFContentHeaderDFT* >( m_pContentHeader );
		m_pContentHeaderDFT->fixEndianness();

		if( ( m_pContentHeaderDFT->iNumDFTCoeffs != m_pContentHeaderDFT->iTransformSize ) &&
			( m_pContentHeaderDFT->iNumDFTCoeffs != floor( m_pContentHeaderDFT->iTransformSize / 2.0 ) + 1 ) )
			return DAFF_FILE_CONTENT_INVALID_PARAMETER;

		break;
	}

	return DAFF_NO_ERROR;
}

int DAFFReaderImpl::loadRecordDescriptor()
{
	/*
	 *  4rd step: Load the record descriptor block
	 */

	// Set access pointer and fix the endianness
	switch( m_pMainHeader->iContentType )
	{
	case DAFF_IMPULSE_RESPONSE:
		// A single IR record channel desc is 4+4+4+8 Byte = 20 Bytes
		m_iRecordChannelDescSize = sizeof( DAFFRecordChannelDescIR );
		assert( m_iRecordChannelDescSize == 20 );

		// Fix endianness for channel descriptors and metadata index
		for( int i = 0; i < m_pMainHeader->iNumRecords; i++ )
		{
			for( int c = 0; c < m_pMainHeader->iNumChannels; c++ )
			{
				DAFFRecordChannelDescIR* pDesc = reinterpret_cast< DAFFRecordChannelDescIR* >( getRecordChannelDescPtr( i, c ) );
				pDesc->fixEndianness();
			}

			DAFF::le2se_4byte( getRecordMetadataIndexPtr( i ), 1 );
		};

		break;

	case DAFF_MAGNITUDE_SPECTRUM:
	case DAFF_PHASE_SPECTRUM:
	case DAFF_MAGNITUDE_PHASE_SPECTRUM:
	case DAFF_DFT_SPECTRUM:
		// All other content use a default record channel desc (MS/PS/MPS/DFT) which is 8 Bytes
		m_iRecordChannelDescSize = sizeof( DAFFRecordChannelDescDefault );

		// Fix endianness for channel descriptors and metadata index
		for( int i = 0; i < m_pMainHeader->iNumRecords; i++ )
		{
			for( int c = 0; c < m_pMainHeader->iNumChannels; c++ )
			{
				DAFFRecordChannelDescDefault* pDesc = reinterpret_cast< DAFFRecordChannelDescDefault* >( getRecordChannelDescPtr( i, c ) );
				pDesc->fixEndianness();
			}

			DAFF::le2se_4byte( getRecordMetadataIndexPtr( i ), 1 );
		};

		break;
	};

	return DAFF_NO_ERROR;
}

int DAFFReaderImpl::loadRecordData()
{
	/*
	 *  5th step: Load the record data
	 */

	// Fix the endianess of the data
	switch( m_pMainHeader->iQuantization )
	{
	case DAFF_INT16:
		DAFF::le2se_2byte( m_pDataBlock, ( size_t ) ( m_pDataFileBlock->ui64Size / 2 ) );
		break;

	case DAFF_INT24:
		DAFF::le2se_3byte( m_pDataBlock, ( size_t ) ( m_pDataFileBlock->ui64Size / 3 ) );
		break;

	case DAFF_FLOAT32:
		DAFF::le2se_4byte( m_pDataBlock, ( size_t ) ( m_pDataFileBlock->ui64Size / 4 ) );
		break;
	}

	return DAFF_NO_ERROR;
}

int DAFFReaderImpl::loadMetadata( char* pMetadataBuf )
{
	/*
	 *  6th step: Load the metadata
	 */

	DAFFFileBlockEntry* pMetadataFileBlock = NULL;
	if( getFirstFileBlockByID( FILEBLOCK_DAFF1_METADATA_ID, pMetadataFileBlock ) > 1 )
	{
		tidyup();
		return DAFF_FILE_CORRUPTED;
	}

	if( pMetadataFileBlock == nullptr )
	{
		m_vpMetadata.push_back( new DAFFMetadataImpl ); // Empty
	}
	else if( pMetadataFileBlock->ui64Size == 0 )
	{
		m_vpMetadata.push_back( new DAFFMetadataImpl ); // Empty  )
	}
	else
	{

		// Read all the DAFFMetadataImpl instances
		DAFFMetadataImpl* pMetadataImpl = NULL;
		size_t iBytesRead = 0;
		char* pCurrentBuf = pMetadataBuf;
		char* pEndOfMetaDataBuf = pCurrentBuf + ( size_t ) pMetadataFileBlock->ui64Size;
		while( pCurrentBuf < pEndOfMetaDataBuf )
		{
			pMetadataImpl = new DAFFMetadataImpl;
			int iError = pMetadataImpl->load( pCurrentBuf, iBytesRead );
			if( ( iError != 0 ) || ( iBytesRead == 0 ) )
			{
				tidyup();
				return iError;
			}

			m_vpMetadata.push_back( pMetadataImpl );

			// Set Buffer to next DAFFMetaDataImpl instance
			pCurrentBuf += iBytesRead;
		}
	}

	return DAFF_NO_ERROR;
}

void DAFFReaderImpl::fixAngleRanges()
{

	// Important: If there is only one point in a dimension => Then there is no resolution
	// TODO: Recheck if this is correct for non-full and full angular ranges

	float fAlphaSpan;
	if( m_pMainHeader->fAlphaEnd > m_pMainHeader->fAlphaStart )
		fAlphaSpan = m_pMainHeader->fAlphaEnd - m_pMainHeader->fAlphaStart;
	else
		fAlphaSpan = 360 - m_pMainHeader->fAlphaStart + m_pMainHeader->fAlphaEnd;
	float fBetaSpan = m_pMainHeader->fBetaEnd - m_pMainHeader->fBetaStart;

	/*
	 *  [fwe] Bugfix 2011-05-30
	 *  Only if the full azimuth range is span, the formula
	 *
	 *  m_fAlphaResolution = fAlphaSpan / m_pMainHeader->iAlphaPoints
	 *
	 *  is valid. In case there is no full alpha coverage, the last point
	 *  marks the end of the interval and does not count. Therefore:
	 *
	 *  m_fAlphaResolution = fAlphaSpan / (m_pMainHeader->iAlphaPoints-1)
	 *
	 */

	if( m_pMainHeader->iAlphaPoints > 1 )
	{
		if( fAlphaSpan == 360 )
			m_fAlphaResolution = fAlphaSpan / m_pMainHeader->iAlphaPoints;
		else
			m_fAlphaResolution = fAlphaSpan / ( m_pMainHeader->iAlphaPoints - 1 );
	}
	else
		m_fAlphaResolution = 0;

	if( m_pMainHeader->iBetaPoints > 1 )
		m_fBetaResolution = fBetaSpan / ( m_pMainHeader->iBetaPoints - 1 );
	else
		m_fBetaResolution = 0;
}

void DAFFReaderImpl::closeFile()
{
	if( !m_bDAFFObjectValid )
		return;

	tidyup();
}

void DAFFReaderImpl::tidyup()
{
	if( m_file )
	{
		fclose( m_file );
		m_file = NULL;
	}

	DAFF::free_aligned16( m_pFileBlockTable );
	m_pFileBlockTable = NULL;

	DAFF::free_aligned16( m_pMainHeader );
	m_pMainHeader = NULL;

	DAFF::free_aligned16( m_pContentHeader );
	m_pContentHeader = NULL;

	DAFF::free_aligned16( m_pRecordDescriptorBlock );
	m_pRecordDescriptorBlock = NULL;

	DAFF::free_aligned16( m_pDataBlock );
	m_pDataBlock = NULL;

	for( size_t i = 0; i < m_vpMetadata.size(); ++i )
		delete m_vpMetadata[ i ];

	m_vpMetadata.clear();

	m_sFilePath = "";
	m_bDAFFObjectValid = false;
}

int DAFFReaderImpl::getFirstFileBlockByID( int iID, DAFFFileBlockEntry*& pfDest ) const
{
	std::vector<DAFFFileBlockEntry*> v;
	getFileBlocksByID( iID, v );

	if( v.empty() )
	{
		pfDest = NULL;
		return 0;
	}

	pfDest = v.front();
	return ( int ) v.size();
}

int DAFFReaderImpl::getFileBlocksByID( int iID, std::vector<DAFFFileBlockEntry*>& vpfDest ) const
{
	vpfDest.clear();
	for( int i = 0; i < m_fileHeader.iNumFileBlocks; i++ ) {
		if( m_pFileBlockTable[ i ].iID == iID ) vpfDest.push_back( &( m_pFileBlockTable[ i ] ) );
	}

	return ( int ) vpfDest.size();
}

std::string DAFFReaderImpl::getFilename() const
{
	assert( m_bDAFFObjectValid );
	return m_sFilePath;
}

int DAFFReaderImpl::getFileFormatVersion() const
{
	assert( m_bDAFFObjectValid );
	return m_fileHeader.iFileFormatVersion;
}

int DAFFReaderImpl::getContentType() const
{
	assert( m_bDAFFObjectValid );
	return m_pMainHeader->iContentType;
}

DAFFContent* DAFFReaderImpl::getContent() const
{
	assert( m_bDAFFObjectValid );
	// Logical constness here!
	// TODO: Bad cast
	return ( DAFFContentIR* ) this;
}

const DAFFMetadata* DAFFReaderImpl::getMetadata() const
{
	assert( m_bDAFFObjectValid );
	return m_vpMetadata[ 0 ];
}

DAFFProperties* DAFFReaderImpl::getProperties() const
{
	assert( m_bDAFFObjectValid );
	// TODO: Fixme
	return ( DAFFProperties* ) this;
}

std::string DAFFReaderImpl::toString() const
{
	assert( m_bDAFFObjectValid );

	std::stringstream ss;

	float fVersion = getFileFormatVersion() / 1000.0F;;
	ss << "File format version: " << DAFFUtils::Float2StrNice( fVersion, 3, false ) << std::endl;
	ss << "Content type:        " << DAFFUtils::StrContentType( getProperties()->getContentType() ) << std::endl;
	ss << "Quantization:        " << DAFFUtils::StrQuantizationType( getProperties()->getQuantization() ) << std::endl << std::endl;
	ss << "Number of channels:  " << getProperties()->getNumberOfChannels() << std::endl;
	ss << "Number of records:   " << getProperties()->getNumberOfRecords() << std::endl << std::endl;
	ss << "Alpha points:        " << getProperties()->getAlphaPoints() << std::endl;
	ss << "Alpha range:         [" <<
		DAFFUtils::Float2StrNice( getProperties()->getAlphaStart(), 3, false ) << "\xF8, " <<
		DAFFUtils::Float2StrNice( getProperties()->getAlphaEnd(), 3, false ) << "\xF8]" << std::endl;
	ss << "Beta points:         " << getProperties()->getBetaPoints() << std::endl;
	ss << "Beta range:          [" <<
		DAFFUtils::Float2StrNice( getProperties()->getBetaStart(), 3, false ) << "\xF8, " <<
		DAFFUtils::Float2StrNice( getProperties()->getBetaEnd(), 3, false ) << "\xF8]" << std::endl;
	ss << "Full sphere:         " << ( getProperties()->coversFullSphere() ? "yes" : "no" ) << std::endl;
	DAFFOrientationYPR o;
	getOrientation( o );
	ss << "Orientation:         " << o.toString() << std::endl;

	DAFFContentIR* pContentIR;
	DAFFReaderImpl* pContent;
	DAFFContentDFT* pContentDFT;
	std::vector<float> vsFrequencies;

	switch( getContentType() ) {
	case DAFF_IMPULSE_RESPONSE:
		pContentIR = dynamic_cast< DAFFContentIR* >( getContent() );
		ss << "Samplerate:          " << DAFFUtils::Double2StrNice( pContentIR->getSamplerate(), 1, false ) << " Hz" << std::endl;
		ss << "Filter length:       " << pContentIR->getFilterLength() << std::endl;
		break;

	case DAFF_MAGNITUDE_SPECTRUM:
	case DAFF_PHASE_SPECTRUM:
	case DAFF_MAGNITUDE_PHASE_SPECTRUM:
		pContent = dynamic_cast< DAFFReaderImpl* >( getContent() );
		ss << "Number of frequencies: " << pContent->getNumFrequencies() << std::endl;
		vsFrequencies = pContent->getFrequencies();
		ss << "Frequency support:     ";
		for( size_t i = 0; i < vsFrequencies.size(); i++ ) {
			ss << DAFFUtils::Float2StrNice( vsFrequencies[ i ], 3, false );
			if( i < ( vsFrequencies.size() - 1 ) ) ss << ", ";
		}
		ss << " Hz" << std::endl;
		break;


	case DAFF_DFT_SPECTRUM:
		pContentDFT = dynamic_cast< DAFFContentDFT* >( getContent() );
		ss << "Transform size: " << pContentDFT->getTransformSize() << std::endl;
		ss << "Symmetric: " << ( ( pContentDFT->isSymmetric() ) ? "yes" : "no" ) << std::endl;
		ss << "Samplerate: " << pContentDFT->getSamplerate() << std::endl;
		break;
	}
	return ss.str();
}

// --------------------------------------------------------

int DAFFReaderImpl::getQuantization() const {
	assert( m_bDAFFObjectValid );
	return m_pMainHeader->iQuantization;
}

int DAFFReaderImpl::getNumberOfChannels() const {
	assert( m_bDAFFObjectValid );
	return m_pMainHeader->iNumChannels;
}

int DAFFReaderImpl::getNumberOfRecords() const
{
	assert( m_bDAFFObjectValid );
	return m_pMainHeader->iNumRecords;
}

std::string DAFFReaderImpl::getChannelLabel( int iChannel ) const
{
	/* @todo
	This specific function requires a certain key/value pair
	within the global metadata entries in the DAFF file. It appears
	more elegant to provide "styles" that can be met with a defined
	set of keys and values. Compatibility can be checked during loading
	or runtime and the file is marked as "whateever"-kompatible.
	*/

	assert( m_bDAFFObjectValid );
	assert( ( iChannel >= 0 ) && ( iChannel < m_pMainHeader->iNumChannels ) );

	// Fetch the channel name from the metadata
	std::stringstream ss;
	ss << "LABEL_CHANNEL_" << ( iChannel + 1 );
	return ( ( m_vpMetadata.size() > 0 ) && ( m_vpMetadata[ 0 ]->hasKey( ss.str() ) ) ? m_vpMetadata[ 0 ]->getKeyString( ss.str() ) : "" );
}

int DAFFReaderImpl::getAlphaPoints() const
{
	assert( m_bDAFFObjectValid );
	return m_pMainHeader->iAlphaPoints;
}

float DAFFReaderImpl::getAlphaResolution() const {
	assert( m_bDAFFObjectValid );

	return m_fAlphaResolution;
}

float DAFFReaderImpl::getAlphaStart() const {
	assert( m_bDAFFObjectValid );
	return m_pMainHeader->fAlphaStart;
}

float DAFFReaderImpl::getAlphaEnd() const {
	assert( m_bDAFFObjectValid );
	return m_pMainHeader->fAlphaEnd;
}

float DAFFReaderImpl::getAlphaSpan() const {
	assert( m_bDAFFObjectValid );
	if( m_pMainHeader->fAlphaStart <= m_pMainHeader->fAlphaEnd )
		return m_pMainHeader->fAlphaEnd - m_pMainHeader->fAlphaStart;
	else // wrap around 0&deg;
		return 360 - m_pMainHeader->fAlphaStart + m_pMainHeader->fAlphaEnd;
}

int DAFFReaderImpl::getBetaPoints() const {
	assert( m_bDAFFObjectValid );
	return m_pMainHeader->iBetaPoints;
}

float DAFFReaderImpl::getBetaResolution() const {
	assert( m_bDAFFObjectValid );
	return m_fBetaResolution;
}

float DAFFReaderImpl::getBetaStart() const {
	assert( m_bDAFFObjectValid );
	return m_pMainHeader->fBetaStart;
}

float DAFFReaderImpl::getBetaEnd() const {
	assert( m_bDAFFObjectValid );
	return m_pMainHeader->fBetaEnd;
}

float DAFFReaderImpl::getBetaSpan() const {
	assert( m_bDAFFObjectValid );
	return m_pMainHeader->fAlphaEnd - m_pMainHeader->fAlphaStart;
}

void DAFFReaderImpl::getDefaultOrientation( DAFFOrientationYPR& o ) const {
	assert( m_bDAFFObjectValid );
	o = m_orientationDefault;
}

void DAFFReaderImpl::setDefaultOrientation() {
	assert( m_bDAFFObjectValid );
	m_tTrans.setOrientation( m_orientationDefault );
}

void DAFFReaderImpl::getOrientation( DAFFOrientationYPR& o ) const {
	assert( m_bDAFFObjectValid );
	m_tTrans.getOrientation( o );
}

void DAFFReaderImpl::setOrientation( const DAFFOrientationYPR& o ) {
	assert( m_bDAFFObjectValid );
	m_tTrans.setOrientation( o );
}

bool DAFFReaderImpl::coversFullAlphaRange() const {
	assert( m_bDAFFObjectValid );
	// full range coverage is given only when alphastart == 0 and alphaend == 360
	if( ( m_pMainHeader->fAlphaStart == 0 ) && ( m_pMainHeader->fAlphaEnd == 360 ) )
		return true;
	else
		return false;
}

bool DAFFReaderImpl::coversFullBetaRange() const {
	assert( m_bDAFFObjectValid );
	// full range coverage is given only when betastart == 0 and betaend == 180
	if( ( m_pMainHeader->fBetaStart == 0 ) && ( m_pMainHeader->fBetaEnd == 180 ) )
		return true;
	else
		return false;
}

bool DAFFReaderImpl::coversFullSphere() const {
	assert( m_bDAFFObjectValid );
	/*
	if ( ((m_pMainHeader->fAlphaEnd - m_pMainHeader->fAlphaStart) >= (360.0f - getAlphaResolution()) &&
	((m_pMainHeader->fBetaEnd - m_pMainHeader->fBetaStart) == 180.0f)) )
	return true;

	return false;*/
	return coversFullAlphaRange() && coversFullBetaRange();
}

DAFFReader* DAFFReaderImpl::getParent() const {
	// We are the reader ourself!
	// Logical constness here!
	return const_cast< DAFFReaderImpl* >( this );
}

void DAFFReaderImpl::getNearestNeighbour( int iView, float fAngle1, float fAngle2, int& iRecordIndex ) const {
	assert( ( iView == DAFF_DATA_VIEW ) || ( iView == DAFF_OBJECT_VIEW ) );

	bool bDummy;
	getNearestNeighbour( iView, fAngle1, fAngle2, iRecordIndex, bDummy );
}

const DAFFMetadata* DAFFReaderImpl::getRecordMetadata( int iRecordIndex ) const
{
	assert( ( iRecordIndex >= 0 ) && ( iRecordIndex < m_pMainHeader->iNumRecords ) );

	if( ( iRecordIndex < 0 ) || ( iRecordIndex >= m_pMainHeader->iNumRecords ) )
		return m_pEmptyMetadata;

	int iMetadataIndex = *getRecordMetadataIndexPtr( iRecordIndex );

	assert( ( iMetadataIndex >= -1 ) && ( iMetadataIndex < ( int ) m_vpMetadata.size() ) );

	if( iMetadataIndex == -1 )
		return m_pEmptyMetadata;
	else
		return m_vpMetadata[ iMetadataIndex ];
}

int DAFFReaderImpl::getRecordCoords( int iRecordIndex, int iView, float& fAngle1, float& fAngle2 ) const {
	assert( ( iRecordIndex >= 0 ) || ( iRecordIndex < m_pMainHeader->iNumRecords ) );
	assert( ( iView == DAFF_DATA_VIEW ) || ( iView == DAFF_OBJECT_VIEW ) );

	// Invalid record index - projection to get a valid index
	if( ( iRecordIndex < 0 ) || ( iRecordIndex >= m_pMainHeader->iNumRecords ) ) {
		// TODO: Discuss if this makes sense
		return -1;
	}

	int iAlpha, iBeta;
	float fAlpha, fBeta;

	bool bSouthPolePresent = false;
	if( m_pMainHeader->fBetaStart == 0.0f ) // South pole present (single record here)
		bSouthPolePresent = true;

	if( bSouthPolePresent ) {
		if( iRecordIndex == 0 ) { // Hit south pole
			fAlpha = 0.0f;
			fBeta = 0.0f;
		}
		else { // somewhere inside the grid (with single value at south pole)
			iAlpha = ( iRecordIndex - 1 ) % m_pMainHeader->iAlphaPoints;
			iBeta = 1 + ( int ) ( ( iRecordIndex - 1 ) / m_pMainHeader->iAlphaPoints );
			fAlpha = m_pMainHeader->fAlphaStart + ( ( float ) iAlpha * m_fAlphaResolution );
			fBeta = ( float ) iBeta  * m_fBetaResolution;
		}
	}
	else { // somewhere inside the grid (without south pole present)
		iAlpha = iRecordIndex % m_pMainHeader->iAlphaPoints;
		iBeta = ( int ) ( iRecordIndex / m_pMainHeader->iAlphaPoints );
		fAlpha = m_pMainHeader->fAlphaStart + ( ( float ) iAlpha * m_fAlphaResolution );
		fBeta = m_pMainHeader->fBetaStart + ( ( float ) iBeta  * m_fBetaResolution );
	}

	if( iView == DAFF_DATA_VIEW ) {
		fAngle1 = fAlpha;
		fAngle2 = fBeta;
	}
	else {
		transformAnglesD2O( fAlpha, fBeta, fAngle1, fAngle2 );
	}

	return 0;
}

void DAFFReaderImpl::getNearestNeighbour( int iView, float fAngle1, float fAngle2, int& iRecordIndex, bool& bOutOfBounds ) const
{
	assert( ( iView == DAFF_DATA_VIEW ) || ( iView == DAFF_OBJECT_VIEW ) );

	float fAlpha;
	float fBeta;

	if( iView == DAFF_DATA_VIEW )
	{
		fAlpha = fAngle1;
		fBeta = fAngle2;
	}
	else
	{
		// Transform the coordinates into the DSC
		transformAnglesO2D( fAngle1, fAngle2, fAlpha, fBeta );
	}

	// Normalize the direction
	DAFFUtils::NormalizeDirection( DAFF_DATA_VIEW, fAlpha, fBeta, fAlpha, fBeta );

	iRecordIndex = -1;
	bOutOfBounds = false;

	int iAlphaIndex, iBetaIndex;

	if( m_pMainHeader->iAlphaPoints == 1 )
	{
		// Trivial case: Just one point
		iAlphaIndex = 0;
		bOutOfBounds = true;
		if( fAlpha == m_pMainHeader->fAlphaStart && fAlpha == std::fmod( m_pMainHeader->fAlphaEnd, 360.0f ) )
			bOutOfBounds = false; // Direct hit
	}
	else
	{
		if( ( fAlpha >= m_pMainHeader->fAlphaStart ) && ( fAlpha <= m_pMainHeader->fAlphaEnd ) )
		{
			// Within the covered alpha range
			iAlphaIndex = ( int ) roundf( ( fAlpha - m_pMainHeader->fAlphaStart ) / m_fAlphaResolution );
		}
		else
		{
			// Outside the covered alpha range
			// Decide: Which is closer? Start boundary or end boundary?
			if( DAFF::anglef_mindiff_abs_0_360_DEG( m_pMainHeader->fAlphaStart, fAlpha ) <= DAFF::anglef_mindiff_abs_0_360_DEG( m_pMainHeader->fAlphaEnd, fAlpha ) )
				iAlphaIndex = 0; // Start index
			else
				iAlphaIndex = ( int ) m_pMainHeader->iAlphaPoints - 1; // End index

			bOutOfBounds = true;
		}
	}

	if( m_pMainHeader->iBetaPoints == 1 )
	{
		// Trivial case: Just one point
		iBetaIndex = 0;
		bOutOfBounds = true;
		if( fBeta == m_pMainHeader->fBetaEnd && fBeta == m_pMainHeader->fBetaStart )
			bOutOfBounds = false; // Direct hit
	}
	else
	{
		if( ( fBeta >= m_pMainHeader->fBetaStart ) && ( fBeta <= m_pMainHeader->fBetaEnd ) )
		{
			// Within the covered beta range
			iBetaIndex = ( int ) roundf( ( fBeta - m_pMainHeader->fBetaStart ) / m_fBetaResolution );
		}
		else
		{
			// Outside the covered beta range
			// Decide: Which is closer? Start boundary or end boundary?
			if( std::abs( m_pMainHeader->fBetaStart - fBeta ) <= std::abs( m_pMainHeader->fBetaEnd - fBeta ) )
				iBetaIndex = 0; // Start index
			else
				iBetaIndex = ( int ) m_pMainHeader->iBetaPoints - 1; // End index

			bOutOfBounds = true;
		}
	}

	// Calculate index
	if( m_pMainHeader->fBetaStart == 0.0f )
	{ // South pole present: increment by one (single record at poles)
		if( iBetaIndex == 0 )
		{ // Hit south pole
			iRecordIndex = 0;
		}
		else
		{
			if( ( iBetaIndex == m_pMainHeader->iBetaPoints - 1 ) && ( m_pMainHeader->fBetaEnd == 180.0f ) ) // Hit north pole
				iRecordIndex = 1 + ( iBetaIndex - 1 ) * m_pMainHeader->iAlphaPoints;
			else
				iRecordIndex = 1 + ( iBetaIndex - 1 ) * m_pMainHeader->iAlphaPoints + iAlphaIndex;
		}
	}
	else
	{
		if( ( iBetaIndex == m_pMainHeader->iBetaPoints - 1 ) && ( m_pMainHeader->fBetaEnd == 180.0f ) ) // Hit north pole
			iRecordIndex = iBetaIndex * m_pMainHeader->iAlphaPoints;
		else
			iRecordIndex = iBetaIndex * m_pMainHeader->iAlphaPoints + iAlphaIndex;
	}

	return;
}

void DAFFReaderImpl::getCell( int iView, const float fAngle1, const float fAngle2, DAFFQuad& qIndices ) const {

	//switch to data view
	float fAlpha = fAngle1;
	float fBeta = fAngle2;
	if( iView == DAFF_OBJECT_VIEW )
		transformAnglesO2D( fAngle1, fAngle2, fAlpha, fBeta );
	DAFFUtils::NormalizeDirection( DAFF_DATA_VIEW, fAlpha, fBeta, fAlpha, fBeta );

	// south pole with full sphere covered is a problem (but algorithm holds for north pole)
	if( fBeta == 0.0f && coversFullSphere() ) { // return the south pole 4 times
		getNearestNeighbour( DAFF_DATA_VIEW, 0.0f, 0.0f, qIndices.iIndex1 );
		qIndices.iIndex2 = qIndices.iIndex1;
		qIndices.iIndex3 = qIndices.iIndex1;
		qIndices.iIndex4 = qIndices.iIndex1;
		return;
	}

	// grid angles
	float fAlpha1 = 0, fAlpha2 = 0, fAlpha3 = 0, fAlpha4 = 0;
	float fBeta1 = 0, fBeta2 = 0, fBeta3 = 0, fBeta4 = 0;

	fAlpha1 = fAlpha - fmodf( fAlpha, getAlphaResolution() );
	fAlpha2 = fAlpha1;
	fAlpha3 = fAlpha + getAlphaResolution() - fmodf( fAlpha, getAlphaResolution() );
	fAlpha4 = fAlpha3;

	fBeta1 = fBeta - fmodf( fBeta, getBetaResolution() );
	fBeta2 = fBeta + getBetaResolution() - fmodf( fBeta, getBetaResolution() );
	fBeta3 = fBeta2;
	fBeta4 = fBeta1;

	// Upper beta angles are not allowed to overrun the north pole
	if( fBeta2 > 180.0f ) {
		fBeta2 = 180.0f;
		fBeta3 = 180.0f;
	}

	// Normalize & get indices
	DAFFUtils::NormalizeDirection( DAFF_DATA_VIEW, fAlpha1, fBeta1, fAlpha1, fBeta1 );
	DAFFUtils::NormalizeDirection( DAFF_DATA_VIEW, fAlpha2, fBeta2, fAlpha2, fBeta2 );
	DAFFUtils::NormalizeDirection( DAFF_DATA_VIEW, fAlpha3, fBeta3, fAlpha3, fBeta3 );
	DAFFUtils::NormalizeDirection( DAFF_DATA_VIEW, fAlpha4, fBeta4, fAlpha4, fBeta4 );

	getNearestNeighbour( DAFF_DATA_VIEW, fAlpha1, fBeta1, qIndices.iIndex1 );
	getNearestNeighbour( DAFF_DATA_VIEW, fAlpha2, fBeta2, qIndices.iIndex2 );
	getNearestNeighbour( DAFF_DATA_VIEW, fAlpha3, fBeta3, qIndices.iIndex3 );
	getNearestNeighbour( DAFF_DATA_VIEW, fAlpha4, fBeta4, qIndices.iIndex4 );
}

void DAFFReaderImpl::transformAnglesD2O( const float fAlpha, const float fBeta, float& fAzimuth, float& fElevation ) const {
	m_tTrans.transformDSC2OSC( fAlpha, fBeta, fAzimuth, fElevation );
}

void DAFFReaderImpl::transformAnglesO2D( const float fAzimuth, const float fElevation, float& fAlpha, float& fBeta ) const {
	m_tTrans.transformOSC2DSC( fAzimuth, fElevation, fAlpha, fBeta );
}

double DAFFReaderImpl::getSamplerate() const {
	if( m_pMainHeader->iContentType == DAFF_IMPULSE_RESPONSE )
		return m_pContentHeaderIR->fSamplerate;
	if( m_pMainHeader->iContentType == DAFF_DFT_SPECTRUM )
		return m_pContentHeaderDFT->fSamplerate;
	return 0; // error
}

int DAFFReaderImpl::getFilterLength() const {
	return m_pMainHeader->iElementsPerRecord;
}

int DAFFReaderImpl::getMinEffectiveFilterOffset() const {
	return m_pContentHeaderIR->iMinFilterOffset;
}

int DAFFReaderImpl::getMaxEffectiveFilterLength() const {
	return m_pContentHeaderIR->iMaxEffectiveFilterLength;
}

int DAFFReaderImpl::getFilterCoeffs( int iRecordIndex, int iChannel, float* pfDest, float fGain ) const
{
	int iOffset;
	int iEffectiveLength;
	int iError;

	iError = getEffectiveFilterBounds( iRecordIndex, iChannel, iOffset, iEffectiveLength );
	if( iError != DAFF_NO_ERROR ) return iError;

	// Place zeros before the data
	for( int i = 0; i < iOffset; i++ )
		pfDest[ i ] = 0;

	// Insert the data
	iError = getEffectiveFilterCoeffs( iRecordIndex, iChannel, pfDest + iOffset, fGain );
	if( iError != DAFF_NO_ERROR ) return iError;

	// Place zeros behind the data
	for( int i = iOffset + iEffectiveLength; i < getFilterLength(); i++ )
		pfDest[ i ] = 0;

	return DAFF_NO_ERROR;
}

int DAFFReaderImpl::addFilterCoeffs( int iRecordIndex, int iChannel, float* pfDest, float fGain ) const
{
	int iOffset;
	int iEffectiveLength;
	int iError;

	iError = getEffectiveFilterBounds( iRecordIndex, iChannel, iOffset, iEffectiveLength );
	if( iError != DAFF_NO_ERROR )
		return iError;

	// Insert the data
	iError = addEffectiveFilterCoeffs( iRecordIndex, iChannel, pfDest + iOffset, fGain );
	if( iError != DAFF_NO_ERROR )
		return iError;

	return DAFF_NO_ERROR;
}

int DAFFReaderImpl::getEffectiveFilterBounds( int iRecordIndex, int iChannel, int& iOffset, int& iLength ) const
{
	assert( ( iRecordIndex >= 0 ) && ( iRecordIndex < m_pMainHeader->iNumRecords ) );
	assert( ( iChannel >= 0 ) && ( iChannel < m_pMainHeader->iNumChannels ) );

	if( ( iRecordIndex < 0 ) || ( iRecordIndex >= m_pMainHeader->iNumRecords ) ||
		( iChannel < 0 ) || ( iChannel >= m_pMainHeader->iNumChannels ) )
		return DAFF_INVALID_INDEX;

	DAFFRecordChannelDescIR* pDesc = reinterpret_cast< DAFFRecordChannelDescIR* >( getRecordChannelDescPtr( iRecordIndex, iChannel ) );
	iOffset = pDesc->iLeadingZeros;
	iLength = pDesc->iElementLength;

	return DAFF_NO_ERROR;
}

int DAFFReaderImpl::getEffectiveFilterCoeffs( int iRecordIndex, int iChannel, float* pfDest, float fGain ) const
{
	assert( ( iRecordIndex >= 0 ) && ( iRecordIndex < m_pMainHeader->iNumRecords ) );
	assert( ( iChannel >= 0 ) && ( iChannel < m_pMainHeader->iNumChannels ) );

	if( ( iRecordIndex < 0 ) || ( iRecordIndex >= m_pMainHeader->iNumRecords ) ||
		( iChannel < 0 ) || ( iChannel >= m_pMainHeader->iNumChannels ) )
		return DAFF_INVALID_INDEX;

	if( pfDest == NULL )
		return DAFF_NO_ERROR;

	DAFFRecordChannelDescIR* pDesc = reinterpret_cast< DAFFRecordChannelDescIR* >( getRecordChannelDescPtr( iRecordIndex, iChannel ) );
	// Check data offset for buffer overruns
	assert( pDesc->ui64DataOffset < m_pDataFileBlock->ui64Size );
	void* pData = reinterpret_cast< char* >( m_pDataBlock ) + pDesc->ui64DataOffset;

	// Data type conversion
	switch( m_pMainHeader->iQuantization )
	{
	case DAFF_INT16:
		DAFF::stc_sint16_to_float( pfDest, ( const short* ) pData, pDesc->iElementLength, 1, 1, fGain );
		break;

	case DAFF_INT24:
		DAFF::stc_sint24_to_float( pfDest, pData, pDesc->iElementLength, 1, 1, fGain );
		break;

	case DAFF_FLOAT32:
		if( fGain == 1 )
		{	// Direct copy
			memcpy( pfDest, pData, pDesc->iElementLength * sizeof( float ) );
		}
		else
		{
			// Copy with gain multiplication
			float* pfData = ( float* ) pData;
			for( int i = 0; i < pDesc->iElementLength; i++ )
				pfDest[ i ] = pfData[ i ] * fGain;
		}
		break;
	}

	return DAFF_NO_ERROR;
}

int DAFFReaderImpl::addEffectiveFilterCoeffs( int iRecordIndex, int iChannel, float* pfDest, float fGain ) const
{
	assert( ( iRecordIndex >= 0 ) && ( iRecordIndex < m_pMainHeader->iNumRecords ) );
	assert( ( iChannel >= 0 ) && ( iChannel < m_pMainHeader->iNumChannels ) );

	if( ( iRecordIndex < 0 ) || ( iRecordIndex >= m_pMainHeader->iNumRecords ) ||
		( iChannel < 0 ) || ( iChannel >= m_pMainHeader->iNumChannels ) )
		return DAFF_INVALID_INDEX;

	if( pfDest == NULL ) return DAFF_NO_ERROR;

	DAFFRecordChannelDescIR* pDesc = reinterpret_cast< DAFFRecordChannelDescIR* >( getRecordChannelDescPtr( iRecordIndex, iChannel ) );
	// Check data offset for buffer overruns
	assert( pDesc->ui64DataOffset < m_pDataFileBlock->ui64Size );
	void* pData = reinterpret_cast< char* >( m_pDataBlock ) +pDesc->ui64DataOffset;

	// Data type conversion
	switch( m_pMainHeader->iQuantization ) {
	case DAFF_INT16:
		DAFF::stc_sint16_to_float_add( pfDest, ( const short* ) pData, pDesc->iElementLength, 1, 1, fGain );
		break;

	case DAFF_INT24:
		DAFF::stc_sint24_to_float_add( pfDest, pData, pDesc->iElementLength, 1, 1, fGain );
		break;

	case DAFF_FLOAT32:
		float* pfData = ( float* ) pData;
		for( int i = 0; i < pDesc->iElementLength; i++ )
			pfDest[ i ] += pfData[ i ] * fGain;
		break;
	}

	return DAFF_NO_ERROR;
}

int DAFFReaderImpl::getNumFrequencies() const {
	switch( m_pMainHeader->iContentType )
	{
	case DAFF_MAGNITUDE_SPECTRUM:
		return m_pContentHeaderMS->iNumFreqs;
	case DAFF_PHASE_SPECTRUM:
		return m_pContentHeaderPS->iNumFreqs;
	case DAFF_MAGNITUDE_PHASE_SPECTRUM:
		return m_pContentHeaderMPS->iNumFreqs;
	}
	return 0; // error!
}

const std::vector<float>& DAFFReaderImpl::getFrequencies() const
{
	return m_vfFreqs;
}

float DAFFReaderImpl::getOverallMagnitudeMaximum() const {
	switch( m_pMainHeader->iContentType )
	{
	case DAFF_MAGNITUDE_SPECTRUM:
		return m_pContentHeaderMS->fMax;
	case DAFF_MAGNITUDE_PHASE_SPECTRUM:
		return m_pContentHeaderMPS->fMax;
	case DAFF_DFT_SPECTRUM:
		return m_pContentHeaderDFT->fMax;
	}
	return 0.0; // error!
}

float DAFFReaderImpl::getOverallPeak()
{
	if( !m_bOverallPeakInitialized )
	{
		// lazy initialization
		float *pBuf;
		int iOffs = 0, iLen = 0, iSize = getMaxEffectiveFilterLength();
		pBuf = new float[ iSize ];
		for( int i = 0; i < m_pMainHeader->iNumRecords; i++ )
			for( int j = 0; j < m_pMainHeader->iNumChannels; j++ ) {
			getEffectiveFilterCoeffs( i, j, pBuf );
			getEffectiveFilterBounds( i, j, iOffs, iLen );
			for( int k = 0; k<iLen - iOffs; k++ )
				if( fabs( pBuf[ k ] ) > m_fOverallPeak )
					m_fOverallPeak = fabs( pBuf[ k ] );
			}
		delete pBuf;
		m_bOverallPeakInitialized = true;
	}
	return m_fOverallPeak;
}

int DAFFReaderImpl::getMagnitudes( int iRecordIndex, int iChannel, float* pfData ) const {
	assert( ( iRecordIndex >= 0 ) && ( iRecordIndex < m_pMainHeader->iNumRecords ) );
	assert( ( iChannel >= 0 ) && ( iChannel < m_pMainHeader->iNumChannels ) );

	if( ( iRecordIndex < 0 ) || ( iRecordIndex >= m_pMainHeader->iNumRecords ) ||
		( iChannel < 0 ) || ( iChannel >= m_pMainHeader->iNumChannels ) )
		return DAFF_INVALID_INDEX;

	if( pfData == NULL ) return DAFF_NO_ERROR;

	DAFFRecordChannelDescDefault* pDesc = reinterpret_cast< DAFFRecordChannelDescDefault* >( getRecordChannelDescPtr( iRecordIndex, iChannel ) );
	float* pfSrc = 0;

	switch( m_pMainHeader->iContentType ) {
	case DAFF_MAGNITUDE_SPECTRUM:
		pfSrc = reinterpret_cast< float* >( reinterpret_cast< char* >( m_pDataBlock ) +pDesc->ui64DataOffset );
		memcpy( pfData, pfSrc, m_pContentHeaderMS->iNumFreqs*sizeof( float ) );
		break;
	case DAFF_MAGNITUDE_PHASE_SPECTRUM:
		pfSrc = reinterpret_cast< float* >( reinterpret_cast< char* >( m_pDataBlock ) +pDesc->ui64DataOffset );
		// TODO: maybe find a better way to do this
		for( int i = 0; i <= m_pContentHeaderMPS->iNumFreqs; i++ )
			pfData[ i ] = pfSrc[ 2 * i ];
		break;
	default:
		return DAFF_MODAL_ERROR;
	}

	return DAFF_NO_ERROR;
}

int DAFFReaderImpl::getMagnitude( int iRecordIndex, int iChannel, int iFreqIndex, float& fMag ) const
{
	assert( ( iRecordIndex >= 0 ) && ( iRecordIndex < m_pMainHeader->iNumRecords ) );
	assert( ( iChannel >= 0 ) && ( iChannel < m_pMainHeader->iNumChannels ) );

	if( ( iRecordIndex < 0 ) || ( iRecordIndex >= m_pMainHeader->iNumRecords ) ||
		( iChannel < 0 ) || ( iChannel >= m_pMainHeader->iNumChannels ) )
		return DAFF_INVALID_INDEX;

	DAFFRecordChannelDescDefault* pDesc = reinterpret_cast< DAFFRecordChannelDescDefault* >( getRecordChannelDescPtr( iRecordIndex, iChannel ) );
	float* pfSrc = NULL;

	switch( m_pMainHeader->iContentType )
	{
	case DAFF_MAGNITUDE_SPECTRUM:
		if( ( iFreqIndex < 0 ) || ( iFreqIndex >= m_pContentHeaderMS->iNumFreqs ) )
			return DAFF_INVALID_INDEX;

		pfSrc = reinterpret_cast< float* >( reinterpret_cast< char* >( m_pDataBlock ) + pDesc->ui64DataOffset );
		fMag = pfSrc[ iFreqIndex ];
		break;
	case DAFF_MAGNITUDE_PHASE_SPECTRUM:
		if( ( iFreqIndex < 0 ) || ( iFreqIndex >= m_pContentHeaderMPS->iNumFreqs ) )
			return DAFF_INVALID_INDEX;

		pfSrc = reinterpret_cast< float* >( reinterpret_cast< char* >( m_pDataBlock ) + pDesc->ui64DataOffset );
		fMag = pfSrc[ 2 * iFreqIndex ];
		break;
	default:
		return DAFF_MODAL_ERROR;
	}

	return DAFF_NO_ERROR;
}

int DAFFReaderImpl::getPhases( int iRecordIndex, int iChannel, float* pfData ) const
{
	assert( ( iRecordIndex >= 0 ) && ( iRecordIndex < m_pMainHeader->iNumRecords ) );
	assert( ( iChannel >= 0 ) && ( iChannel < m_pMainHeader->iNumChannels ) );

	if( ( iRecordIndex < 0 ) || ( iRecordIndex >= m_pMainHeader->iNumRecords ) ||
		( iChannel < 0 ) || ( iChannel >= m_pMainHeader->iNumChannels ) )
		return DAFF_INVALID_INDEX;

	if( pfData == NULL ) return DAFF_NO_ERROR;

	DAFFRecordChannelDescDefault* pDesc = reinterpret_cast< DAFFRecordChannelDescDefault* >( getRecordChannelDescPtr( iRecordIndex, iChannel ) );
	float* pfSrc = 0;

	switch( m_pMainHeader->iContentType )
	{
	case DAFF_PHASE_SPECTRUM:
		pfSrc = reinterpret_cast< float* >( reinterpret_cast< char* >( m_pDataBlock ) +pDesc->ui64DataOffset );
		memcpy( pfData, pfSrc, m_pContentHeaderPS->iNumFreqs * sizeof( float ) );
		break;
	case DAFF_MAGNITUDE_PHASE_SPECTRUM:
		pfSrc = reinterpret_cast< float* >( reinterpret_cast< char* >( m_pDataBlock ) +pDesc->ui64DataOffset );
		for( int i = 0; i <= m_pContentHeaderMPS->iNumFreqs; i++ )
			pfData[ i ] = pfSrc[ 2 * i + 1 ];
		break;
	default:
		return DAFF_MODAL_ERROR;
	}
	return DAFF_NO_ERROR;
}

int DAFFReaderImpl::getPhase( int iRecordIndex, int iChannel, int iFreqIndex, float& fPhase ) const
{
	assert( ( iRecordIndex >= 0 ) && ( iRecordIndex < m_pMainHeader->iNumRecords ) );
	assert( ( iChannel >= 0 ) && ( iChannel < m_pMainHeader->iNumChannels ) );

	if( ( iRecordIndex < 0 ) || ( iRecordIndex >= m_pMainHeader->iNumRecords ) ||
		( iChannel < 0 ) || ( iChannel >= m_pMainHeader->iNumChannels ) )
		return DAFF_INVALID_INDEX;

	DAFFRecordChannelDescDefault* pDesc = reinterpret_cast< DAFFRecordChannelDescDefault* >( getRecordChannelDescPtr( iRecordIndex, iChannel ) );
	float* pfSrc = 0;

	switch( m_pMainHeader->iContentType ) {
	case DAFF_PHASE_SPECTRUM:
		if( ( iFreqIndex < 0 ) || ( iFreqIndex >= m_pContentHeaderPS->iNumFreqs ) )
			return DAFF_INVALID_INDEX;

		pfSrc = reinterpret_cast< float* >( reinterpret_cast< char* >( m_pDataBlock ) +pDesc->ui64DataOffset );
		fPhase = pfSrc[ iFreqIndex ];
		break;
	case DAFF_MAGNITUDE_PHASE_SPECTRUM:
		if( ( iFreqIndex < 0 ) || ( iFreqIndex >= m_pContentHeaderMPS->iNumFreqs ) )
			return DAFF_INVALID_INDEX;

		pfSrc = reinterpret_cast< float* >( reinterpret_cast< char* >( m_pDataBlock ) +pDesc->ui64DataOffset );
		fPhase = pfSrc[ 2 * iFreqIndex + 1 ];
		break;
	default:
		return DAFF_MODAL_ERROR;
	}

	return DAFF_NO_ERROR;
}
int DAFFReaderImpl::getCoefficientsMP( int iRecordIndex, int iChannel, float* pfDest ) const {
	assert( ( iRecordIndex >= 0 ) && ( iRecordIndex < m_pMainHeader->iNumRecords ) );
	assert( ( iChannel >= 0 ) && ( iChannel < m_pMainHeader->iNumChannels ) );

	if( ( iRecordIndex < 0 ) || ( iRecordIndex >= m_pMainHeader->iNumRecords ) ||
		( iChannel < 0 ) || ( iChannel >= m_pMainHeader->iNumChannels ) )
		return DAFF_INVALID_INDEX;

	if( pfDest == NULL ) return DAFF_NO_ERROR;

	DAFFRecordChannelDescDefault* pDesc = reinterpret_cast< DAFFRecordChannelDescDefault* >( getRecordChannelDescPtr( iRecordIndex, iChannel ) );
	float* pfSrc = reinterpret_cast< float* >( reinterpret_cast< char* >( m_pDataBlock ) +pDesc->ui64DataOffset );

	//TODO: find a better way to do this
	for( int i = 0; i <= m_pContentHeaderMPS->iNumFreqs; i++ ) {
		//Magnitude
		pfDest[ 2 * i ] = DAFF::cabs( pfSrc[ 2 * i ], pfSrc[ 2 * i + 1 ] );
		//Phase
		pfDest[ 2 * i + 1 ] = DAFF::carg( pfSrc[ 2 * i ], pfSrc[ 2 * i + 1 ] );
	}

	return DAFF_NO_ERROR;
}

int DAFFReaderImpl::getCoefficientsRI( int iRecordIndex, int iChannel, float* pfDest ) const {
	assert( ( iRecordIndex >= 0 ) && ( iRecordIndex < m_pMainHeader->iNumRecords ) );
	assert( ( iChannel >= 0 ) && ( iChannel < m_pMainHeader->iNumChannels ) );

	if( ( iRecordIndex < 0 ) || ( iRecordIndex >= m_pMainHeader->iNumRecords ) ||
		( iChannel < 0 ) || ( iChannel >= m_pMainHeader->iNumChannels ) )
		return DAFF_INVALID_INDEX;

	if( pfDest == NULL ) return DAFF_NO_ERROR;

	DAFFRecordChannelDescDefault* pDesc = reinterpret_cast< DAFFRecordChannelDescDefault* >( getRecordChannelDescPtr( iRecordIndex, iChannel ) );
	float* pfSrc = reinterpret_cast< float* >( reinterpret_cast< char* >( m_pDataBlock ) +pDesc->ui64DataOffset );
	memcpy( pfDest, pfSrc, 2 * m_pContentHeaderMPS->iNumFreqs*sizeof( float ) );

	return DAFF_NO_ERROR;
}

int DAFFReaderImpl::getTransformSize() const {
	return m_pContentHeaderDFT->iTransformSize;
}

int DAFFReaderImpl::getNumDFTCoeffs() const {
	return m_pContentHeaderDFT->iNumDFTCoeffs;
}

bool DAFFReaderImpl::isSymmetric() const
{
	return ( m_pContentHeaderDFT->iNumDFTCoeffs != m_pContentHeaderDFT->iTransformSize );
}

double DAFFReaderImpl::getFrequencyBandwidth() const {
	return getSamplerate() / ( double ) getTransformSize();
}

int DAFFReaderImpl::getDFTCoeff( int iRecordIndex, int iChannel, int iDFTCoeff, float& fReal, float& fImag ) const {
	assert( ( iRecordIndex >= 0 ) && ( iRecordIndex < m_pMainHeader->iNumRecords ) );
	assert( ( iChannel >= 0 ) && ( iChannel < m_pMainHeader->iNumChannels ) );
	assert( ( iDFTCoeff >= 0 ) && ( iDFTCoeff < m_pContentHeaderDFT->iNumDFTCoeffs ) );

	// TODO: Wrap complex-conjugate symmetric range

	DAFFRecordChannelDescDefault* pDesc = reinterpret_cast< DAFFRecordChannelDescDefault* >( getRecordChannelDescPtr( iRecordIndex, iChannel ) );
	float* pfSrc = reinterpret_cast< float* >( reinterpret_cast< char* >( m_pDataBlock ) +pDesc->ui64DataOffset );

	fReal = pfSrc[ 2 * iDFTCoeff + 0 ];
	fImag = pfSrc[ 2 * iDFTCoeff + 1 ];

	return DAFF_NO_ERROR;
}

int DAFFReaderImpl::getDFTCoeffs( int iRecordIndex, int iChannel, float* pfDest ) const {
	assert( ( iRecordIndex >= 0 ) && ( iRecordIndex < m_pMainHeader->iNumRecords ) );
	assert( ( iChannel >= 0 ) && ( iChannel < m_pMainHeader->iNumChannels ) );

	if( ( iRecordIndex < 0 ) || ( iRecordIndex >= m_pMainHeader->iNumRecords ) ||
		( iChannel < 0 ) || ( iChannel >= m_pMainHeader->iNumChannels ) )
		return DAFF_INVALID_INDEX;

	if( pfDest == NULL ) return DAFF_NO_ERROR;

	DAFFRecordChannelDescDefault* pDesc = reinterpret_cast< DAFFRecordChannelDescDefault* >( getRecordChannelDescPtr( iRecordIndex, iChannel ) );
	float* pfSrc = reinterpret_cast< float* >( reinterpret_cast< char* >( m_pDataBlock ) +pDesc->ui64DataOffset );
	memcpy( pfDest, pfSrc, 2 * m_pContentHeaderDFT->iNumDFTCoeffs*sizeof( float ) );

	return DAFF_NO_ERROR;
}

void* DAFFReaderImpl::getRecordChannelDescPtr( int iRecord, int iChannel ) const
{
	// Relative to beginning of record descriptor block in bytes
	uint64_t uiRecordDescriptorOffset = iRecord * ( m_pMainHeader->iNumChannels * m_iRecordChannelDescSize ) + iChannel * m_iRecordChannelDescSize;

	// Check buffer overruns
	assert( uiRecordDescriptorOffset < m_pRecordDescriptorTable->ui64Size );

	// Return absolute position in memory by using pointer of record descriptor block pointer
	void* pPtr = ( ( char* ) m_pRecordDescriptorBlock ) + uiRecordDescriptorOffset;
	return pPtr;
}

int* DAFFReaderImpl::getRecordMetadataIndexPtr( int iRecord ) const
{
	char* p = ( char* ) ( getRecordChannelDescPtr( iRecord, 0 ) );
	int* piMetaDataIndex = ( int* ) ( p ); // Index is at first position of descriptor struct

	return piMetaDataIndex;
}
