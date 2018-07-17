#include "DAFFCSWrapper.h"
#include <DAFF.h>

// Disable MSVC security warning for unsafe fopen
#ifdef _MSC_VER
#pragma warning(disable: 4996)
#endif // _MSC_VER

class CUnmanagedDAFFHandle
{
public:
	inline CUnmanagedDAFFHandle()
		: pReader( nullptr )
	{
	};

	inline ~CUnmanagedDAFFHandle()
	{
		delete pReader;
	};

	DAFFReader* pReader;
};

CUnmanagedDAFFHandle* NativeDAFFCreate()
{
	CUnmanagedDAFFHandle* pH = new CUnmanagedDAFFHandle();
	pH->pReader = DAFFReader::create();
	return pH;
}

void NativeDAFFDispose( CUnmanagedDAFFHandle* pDAFFHandle )
{
	delete pDAFFHandle;
}

bool NativeDAFFLoad( CUnmanagedDAFFHandle* pDAFFHandle, const char* pcFilePath )
{
	// Read entire file into a buffer
	FILE* hDAFFFile = fopen( pcFilePath, "rb" );
	if( !hDAFFFile )
		return false;

	fseek( hDAFFFile, 0L, SEEK_END );
	size_t nBytes = ftell( hDAFFFile );
	rewind( hDAFFFile );

	std::vector< char > vcDAFFData( nBytes );
	char* pDAFFDataBuffer = &( vcDAFFData[ 0 ] );
	if( fread( pDAFFDataBuffer, 1, nBytes, hDAFFFile ) != nBytes )
	{
		fclose( hDAFFFile );
		return false;
	}

	fclose( hDAFFFile );

	pDAFFHandle->pReader->deserialize( pDAFFDataBuffer );

	return true;
}

int NativeDAFFGetContentType( CUnmanagedDAFFHandle*  pDAFFHandle )
{
	if( !pDAFFHandle->pReader->isValid() )
		return -1;
	return pDAFFHandle->pReader->getContentType();
}

DAFFContentIR* NativeDAFFGetContentIR( CUnmanagedDAFFHandle* pDAFFHandle )
{
	if( !pDAFFHandle->pReader->isValid() )
		return nullptr;
	if( pDAFFHandle->pReader->getContentType() != DAFF_CONTENT_TYPES::DAFF_IMPULSE_RESPONSE )
		return nullptr;

	return dynamic_cast< DAFFContentIR* >( pDAFFHandle->pReader->getContent() );
}

int NativeDAFFContentIRGetNearestNeighbourRecordIndex( DAFFContentIR* pDAFFContent, double dAzimuth, double dElevation )
{
	if( pDAFFContent == nullptr )
		return -1;

	int iIndex;
	bool bOutOfBounds;
	pDAFFContent->getNearestNeighbour( DAFF_OBJECT_VIEW, float( dAzimuth ), float( dElevation ), iIndex, bOutOfBounds );
	return iIndex;
}

void NativeDAFFContentIRGetRecordCoords( DAFFContentIR* pDAFFContent, const int iIndex, double* pdAzimuth, double* pdElevation )
{
	if( pDAFFContent == nullptr )
		return;

	float fAzimuth, fElevation;
	pDAFFContent->getRecordCoords( iIndex, DAFF_OBJECT_VIEW, fAzimuth, fElevation );
	*pdAzimuth = double( fAzimuth );
	*pdElevation = double( fElevation );
}

bool NativeDAFFContentIRGetRecordData( DAFFContentIR* pDAFFContent, const int iIndex, const int iChannel, float* pcSamples )
{
	if( pDAFFContent == nullptr )
		return false;

	pDAFFContent->getFilterCoeffs( iIndex, iChannel, pcSamples );
	return true;
}

int NativeDAFFContentIRGetLength( DAFFContentIR* pDAFFContent )
{
	if( pDAFFContent == nullptr )
		return false;
	return pDAFFContent->getFilterLength();
}

DAFFContentMS* NativeDAFFGetContentMS( CUnmanagedDAFFHandle* pDAFFHandle )
{
	if( !pDAFFHandle->pReader->isValid() )
		return nullptr;
	if( pDAFFHandle->pReader->getContentType() != DAFF_CONTENT_TYPES::DAFF_MAGNITUDE_SPECTRUM )
		return nullptr;

	return dynamic_cast< DAFFContentMS* >( pDAFFHandle->pReader->getContent() );
}

int NativeDAFFContentMSGetNearestNeighbourRecordIndex( DAFFContentMS* pDAFFContent, double dAzimuth, double dElevation )
{
	if( pDAFFContent == nullptr )
		return -1;

	int iIndex;
	bool bOutOfBounds;
	pDAFFContent->getNearestNeighbour( DAFF_OBJECT_VIEW, float( dAzimuth ), float( dElevation ), iIndex, bOutOfBounds );
	return iIndex;
}

void NativeDAFFContentMSGetRecordCoords( DAFFContentMS* pDAFFContent, const int iIndex, double* pdAzimuth, double* pdElevation )
{
	if( pDAFFContent == nullptr )
		return;

	float fAzimuth, fElevation;
	pDAFFContent->getRecordCoords( iIndex, DAFF_OBJECT_VIEW, fAzimuth, fElevation );
	*pdAzimuth = double( fAzimuth );
	*pdElevation = double( fElevation );
}

bool NativeDAFFContentMSGetRecordData( DAFFContentMS* pDAFFContent, const int iIndex, const int iChannel, float* pcSamples )
{
	if( pDAFFContent == nullptr )
		return false;

	pDAFFContent->getMagnitudes( iIndex, iChannel, pcSamples );
	return true;
}

int NativeDAFFContentMSGetLength( DAFFContentMS* pDAFFContent )
{
	if( pDAFFContent == nullptr )
		return false;
	return pDAFFContent->getNumFrequencies();
}
