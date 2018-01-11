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

#include <iostream>
#include <math.h>

#include <DAFF.h>

// Disable MSVC security warning for unsafe fopen
#ifdef _MSC_VER
#pragma warning(disable: 4996)
#endif // _MSC_VER

using namespace std;

int main( int iNumArgs, char** args )
{
	if( iNumArgs != 2 )
	{
		cerr << "Please specify DAFF file path as first argument" << endl;
		return 255;
	}

	DAFFReader* r = DAFFReader::create();

	// Read entire file into a buffer
	string sFilePath = args[1];
	FILE* hDAFFFile = fopen( sFilePath.c_str(), "rb" );
	if( !hDAFFFile )
		return DAFF_FILE_NOT_FOUND;

	fseek( hDAFFFile, 0L, SEEK_END );
	size_t nBytes = ftell( hDAFFFile );
	rewind( hDAFFFile );

	std::vector< char > vcDAFFData( nBytes );
	char* pDAFFDataBuffer = &(vcDAFFData[ 0 ]);
	if( fread( pDAFFDataBuffer, 1, nBytes, hDAFFFile ) != nBytes )
	{
		fclose( hDAFFFile );
		cerr << "Error: could not read file" << endl;
		return 255;
	}

	fclose( hDAFFFile );

	// Now load as DAFF file using the deserializer ( instead of openFile() )
	int ec = r->deserialize( pDAFFDataBuffer );
	if( ec != 0 )
	{
		cerr << "Error: " << DAFFUtils::StrError( ec ) << endl;
		return 255;
	}

	cout << r->toString() << endl;
	
	delete r;

	return 0;
}
