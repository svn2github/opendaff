/*
 * -------------------------------------------------------------------------------------
 *
 *  OpenDAFF - A free, open source software package for directional audio data
 *  Copyright 2016 Institute of Technical Acoustics, RWTH Aachen University
 *  OpenDAFF is distributed under the Apache License Version 2.0.
 *
 *  ------------------------------------------------------------------------------------
 *
 */

#include <QFile>
#include <DAFF.h>

#include <iostream>

int main( int argc, char *argv[] )
{
	QString sFilePath = ":/ExampleUnityOmni.v17.ms.daff"; // or with alias ":/daff/unity_ms"
	QFile oDAFFFileFromResource( sFilePath );

	if( !oDAFFFileFromResource.open( QIODevice::ReadOnly ) )
	{
		std::cerr << "Could not open file '" << sFilePath.toStdString() << "' from resource." << std::endl;
		return 255;
	}

	QByteArray oDAFFContentRaw = oDAFFFileFromResource.readAll();

	oDAFFFileFromResource.close();

	DAFFReader* pReader = DAFFReader::create();

	int iErr = pReader->deserialize( oDAFFContentRaw.data() );
	if( iErr == DAFF_NO_ERROR )
		std::cout << pReader->toString() << std::endl;
	else
		std::cerr << DAFFUtils::StrError( iErr );

	delete pReader;

    return 0;
}
