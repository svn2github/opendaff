/*
 * -------------------------------------------------------------------------------------
 *
 *  OpenDAFF - A free, open source software package for directional audio data
 *  Copyright 2016-2018 Institute of Technical Acoustics (ITA), RWTH Aachen University
 *  OpenDAFF is distributed under the Apache License Version 2.0.
 *
 *  ------------------------------------------------------------------------------------
 *
 */

/*
 *  In this file packaged data structures are defined,
 *  so basically with identical member memory layout as in the DAFF files
 *
 */

#ifndef IW_DAFF_HEADER
#define IW_DAFF_HEADER

#include "Utils.h"

#ifdef _MSC_VER
// No packing attribute on Microsoft Compilers
#define DAFF_PACK_ATTR
#else
// Packing attribute requires for the GCC
#define DAFF_PACK_ATTR __attribute__((__packed__))
// #define HANDLE_PRAGMA_PACK_PUSH_POP // try this with your gcc compiler, if packing errors occur
#endif

/* +---------------------------------------------------+
   |                                                   |
   |   DAFF file block idenfiticators (IDs)            |
   |                                                   |
   +---------------------------------------------------+ */

//! DAFF Version 1: Main header
static const int FILEBLOCK_DAFF1_MAIN_HEADER_ID = 0x0001;

//! DAFF Version 1: Content header
static const int FILEBLOCK_DAFF1_CONTENT_HEADER_ID = 0x0002;

//! DAFF Version 1: Record descriptor block
static const int FILEBLOCK_DAFF1_RECORD_DESC_ID = 0x0003;

//! DAFF Version 1: Data block
static const int FILEBLOCK_DAFF1_DATA_ID = 0x0004;

//! DAFF Version 1: Metadata block
static const int FILEBLOCK_DAFF1_METADATA_ID = 0x0005;


/* +---------------------------------------------------+
   |                                                   |
   |   DAFF file format header                         |
   |                                                   |
   +---------------------------------------------------+ */

//! This is the (pre) header of DAFF files
struct DAFFFileHeader
{
#pragma pack(push,1)
	uint8_t pcSignature[ 2 ];			//!@ The magical signature
	int32_t iFileFormatVersion;		//!@ Version of file format
	//!@ (digits 0-2 => minor version, other digits => major version)
	int32_t iNumFileBlocks;			//!@ Number of file blocks

	// << From here file block entries succeed >>
#pragma pack(pop)

	//! Convert the little-endian file format into the systems endianness
	inline void fixEndianness()
	{
		DAFF::le2se_4byte( &iFileFormatVersion, 1 );
		DAFF::le2se_4byte( &iNumFileBlocks, 1 );
	};
} DAFF_PACK_ATTR;


//! A file block entry
struct DAFFFileBlockEntry
{
#pragma pack(push,1)
	int32_t iID;			// File block identifier
	uint64_t ui64Offset;	// Start offset of the block within the file
	uint64_t ui64Size;		// Size of the block [Bytes]
#pragma pack(pop)

	//! Convert the little-endian file format into the systems endianness
	inline void fixEndianness()
	{
		DAFF::le2se_4byte( &iID, 1 );
		DAFF::le2se_8byte( &ui64Offset, 1 );
		DAFF::le2se_8byte( &ui64Size, 1 );
	};
} DAFF_PACK_ATTR;

/* +---------------------------------------------------+
   |                                                   |
   |   DAFF file format main header                    |
   |                                                   |
   +---------------------------------------------------+ */

struct DAFFMainHeader
{
#pragma pack(push,1)
	// Common definitions
	int32_t iContentType;			//!@ Identificator of the database content type
	int32_t iQuantization;			//!@ Quantization of samples/coefficients
	int32_t iNumChannels;			//!@ Number of channels
	int32_t iNumRecords;			//!@ Overall number of records
	int32_t iElementsPerRecord;		//!@ Number of elements per records (number of taps/DFT coefficients)
	int32_t iMetadataIndex;			//!@ Index of global metadata

	int32_t iAlphaPoints;			//!@ Number of measurement points in alpha range [0&deg;, 360&deg;)
	float fAlphaStart, fAlphaEnd;	//!@ Alpha range boundaries
	int32_t iBetaPoints;			//!@ Number of measurement points in beta range [0&deg;, 180&deg;]
	float fBetaStart, fBetaEnd;		//!@ Beta range boundaries

	float fOrientYaw;				//!@ Yaw angle of the orientation [&deg;]
	float fOrientPitch;				//!@ Pitch angle of the orientation [&deg;]
	float fOrientRoll;				//!@ Roll angle of the orientation [&deg;]

#pragma pack(pop)

	//! Convert the little-endian file format into the systems endianness
	inline void fixEndianness()
	{
		DAFF::le2se_4byte( &iContentType, 1 );
		DAFF::le2se_4byte( &iQuantization, 1 );
		DAFF::le2se_4byte( &iNumChannels, 1 );
		DAFF::le2se_4byte( &iNumRecords, 1 );
		DAFF::le2se_4byte( &iElementsPerRecord, 1 );
		DAFF::le2se_4byte( &iMetadataIndex, 1 );

		DAFF::le2se_4byte( &iAlphaPoints, 1 );
		DAFF::le2se_4byte( &fAlphaStart, 1 );
		DAFF::le2se_4byte( &fAlphaEnd, 1 );
		DAFF::le2se_4byte( &iBetaPoints, 1 );
		DAFF::le2se_4byte( &fBetaStart, 1 );
		DAFF::le2se_4byte( &fBetaEnd, 1 );

		DAFF::le2se_4byte( &fOrientYaw, 1 );
		DAFF::le2se_4byte( &fOrientPitch, 1 );
		DAFF::le2se_4byte( &fOrientRoll, 1 );
	};
} DAFF_PACK_ATTR;

//! DAFF file format content header for impulse responses
struct DAFFContentHeaderIR
{
#pragma pack(push,1)
	float fSamplerate;					//!@ Samplingrate [in Hertz]
	int32_t iMinFilterOffset;			//!@ Minimum offset (number of leading zeros) among all IRs
	int32_t iMaxEffectiveFilterLength;	//!@ Maximum effective Filterlength among all IRs
#pragma pack(pop)

	//! Convert the little-endian file format into the systems endianness
	inline void fixEndianness()
	{
		DAFF::le2se_4byte( &fSamplerate, 1 );
		DAFF::le2se_4byte( &iMinFilterOffset, 1 );
		DAFF::le2se_4byte( &iMaxEffectiveFilterLength, 1 );
	};
} DAFF_PACK_ATTR;

//! DAFF file format content header for magnitude spectra
struct DAFFContentHeaderMS
{
#pragma pack(push,1)
	float fMax;				//!@ Maximum magnitude value over all records/channels/frequencies
	int32_t iNumFreqs;		//!@ Number of support frequencies

	// Next: Support frequency values [in Hertz] (float)
#pragma pack(pop)

	//! Convert the little-endian file format into the systems endianness
	inline void fixEndianness()
	{
		DAFF::le2se_4byte( &fMax, 1 );
		DAFF::le2se_4byte( &iNumFreqs, 1 );
	};
} DAFF_PACK_ATTR;

//! DAFF file format content header for phase spectra
struct DAFFContentHeaderPS
{
#pragma pack(push,1)
	int32_t iNumFreqs;		//!@ Number of support frequencies

#pragma pack(pop)

	//! Convert the little-endian file format into the systems endianness
	inline void fixEndianness()
	{
		DAFF::le2se_4byte( &iNumFreqs, 1 );
	};
} DAFF_PACK_ATTR;

//! DAFF file format content header for magnitude-phase spectra
struct DAFFContentHeaderMPS
{
#pragma pack(push,1)
	float fMax;				//!@ Maximum magnitude value over all records/channels/frequencies
	int32_t iNumFreqs;		//!@ Number of support frequencies

#pragma pack(pop)

	//! Convert the little-endian file format into the systems endianness
	inline void fixEndianness()
	{
		DAFF::le2se_4byte( &fMax, 1 );
		DAFF::le2se_4byte( &iNumFreqs, 1 );
	};
} DAFF_PACK_ATTR;

//! DAFF file format content header for discrete Fourier spectra
struct DAFFContentHeaderDFT
{
#pragma pack(push,1)
	int32_t iNumDFTCoeffs;		//!@ Number of (stored) DFT coefficients (= complex elements per record)
	int32_t iTransformSize;		//!@ DFT transform size
	float fSamplerate;			//!@ Samplingrate [in Hertz]
	float fMax;					//!@ Maximum magnitude value over all records/channels/frequencies
#pragma pack(pop)

	//! Convert the little-endian file format into the systems endianness
	inline void fixEndianness()
	{
		DAFF::le2se_4byte( &iNumDFTCoeffs, 1 );
		DAFF::le2se_4byte( &iTransformSize, 1 );
		DAFF::le2se_4byte( &fSamplerate, 1 );
		DAFF::le2se_4byte( &fMax, 1 );
	};
} DAFF_PACK_ATTR;

/* +---------------------------------------------------+
   |                                                   |
   |   DAFF record descriptors                         |
   |                                                   |
   +---------------------------------------------------+ */

// Default record channel descriptor for records with constant size.
// (Used for: MS, PS, MPS and DFT content)
struct DAFFRecordChannelDescDefault
{
#pragma pack(push,1)
	int32_t iMetaDataIndex;		//!@ Index in metadata table (C-style indexing: beginning with 0)
	uint64_t ui64DataOffset;	//!@ Position inside the file where samples/coefficients reside
#pragma pack(pop)

	//! Convert the little-endian file format into the systems endianness
	inline void fixEndianness()
	{
		DAFF::le2se_4byte( &iMetaDataIndex, 1 );
		DAFF::le2se_8byte( &ui64DataOffset, 1 );
	};
} DAFF_PACK_ATTR;

//! Record channel descriptor: impulse responses content
struct DAFFRecordChannelDescIR
{
#pragma pack(push,1)
	int32_t iMetaDataIndex;		//!@ Index in metadata table (C-style indexing: beginning with 0)
	uint64_t ui64DataOffset;	//!@ Position inside the file where samples/coefficients reside (in bytes)
	int32_t iLeadingZeros;		//!@ Offset of actual data within impulse response (leading zeros that are not included in DAFF data)
	int32_t iElementLength;		//!@ Number of data values (length of element of record channel for a single channel, only)
#pragma pack(pop)

	//! Convert the little-endian file format into the systems endianness
	inline void fixEndianness()
	{
		DAFF::le2se_4byte( &iMetaDataIndex, 1 );
		DAFF::le2se_8byte( &ui64DataOffset, 1 );
		DAFF::le2se_4byte( &iLeadingZeros, 1 );
		DAFF::le2se_4byte( &iElementLength, 1 );
	};
} DAFF_PACK_ATTR;

#endif // IW_DAFF_HEADER
