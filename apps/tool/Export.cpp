#include "Export.h"

#include <DAFF.h>
#include <sndfile.h>

// Disable MSVC security warning for unsafe fopen
#ifdef _MSC_VER
#pragma warning(disable: 4996)
#endif // _MSC_VER

int exportIR(DAFFContentIR* pContentIR, int iRecordIndex, const std::string& sFilename, int iView, bool bQuiet, bool bVerbose) {

	int iChannels = pContentIR->getProperties()->getNumberOfChannels();
	int iFilterLength = pContentIR->getFilterLength();

	// Fetch the filter data and rearrange it into interleaved format as sndfile requires

	float* pfData = new float[iChannels * iFilterLength];
	float* pfDataInterleaved = new float[iChannels * iFilterLength];

	for (int iChannel=0; iChannel<iChannels; iChannel++)
		pContentIR->getFilterCoeffs(iRecordIndex, iChannel, pfData + iChannel * iFilterLength);
	
	int iCursor=0;
	for (int iFrame=0; iFrame<iFilterLength; iFrame++)
		for (int iChannel=0; iChannel<iChannels; iChannel++)
			pfDataInterleaved[iCursor++] = pfData[iChannel*iFilterLength + iFrame];

	delete[] pfData;

	// Write the data into the audiofile

	SNDFILE* pSndOutFile(NULL);
	SF_INFO sfOutFileInfo;

	std::string sOutfileQuantization;
	switch( pContentIR->getProperties()->getQuantization() )
	{
		case DAFF_INT16:
			sfOutFileInfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
			sOutfileQuantization = DAFFUtils::StrQuantizationType( DAFF_INT16 );
			break;

		case DAFF_INT24:
			sfOutFileInfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_24;
			sOutfileQuantization = DAFFUtils::StrQuantizationType( DAFF_INT24 );
			break;

		case DAFF_FLOAT32:
			sfOutFileInfo.format = SF_FORMAT_WAV | SF_FORMAT_FLOAT;
			sOutfileQuantization = DAFFUtils::StrQuantizationType( DAFF_FLOAT32 );
			break;

		default:
			fprintf( stderr, "Error: Failed to write output file \"%s\", unrecognized quantization.\n", sFilename.c_str() );
			return 255;
	}

	sfOutFileInfo.channels = iChannels;
	sfOutFileInfo.samplerate = (int) pContentIR->getSamplerate();

	if (!(pSndOutFile = sf_open(sFilename.c_str(), SFM_WRITE, &sfOutFileInfo))) {
		fprintf(stderr, "Error: Could not open file \"%s\" for writing\n", sFilename.c_str());
		delete[] pfDataInterleaved;
		return 255;
	}

	if (sf_writef_float(pSndOutFile, pfDataInterleaved, iFilterLength) != iFilterLength) {
		fprintf(stderr, "Error: Failed to write output file \"%s\"\n", sFilename.c_str());
		sf_close(pSndOutFile);
		delete[] pfDataInterleaved;
		return 255;
	}

	sf_close(pSndOutFile);

	if (!bQuiet) {
		printf("Exported impulse response to \"%s\" (%i samples | %i channels | %s Hz | %s)\n", 
			   sFilename.c_str(),
			   iFilterLength,
			   iChannels,
			   DAFFUtils::Float2StrNice((float)pContentIR->getSamplerate(),1,false).c_str(),
			   sOutfileQuantization.c_str());
	}

	delete[] pfDataInterleaved;

	return 0;
}

int exportMS(DAFFContentMS* pContentMS, const std::string& sFilename, int iRecordIndex, float fAngle1, float fAngle2, int iView, bool bQuiet, bool bVerbose) {
	FILE* file = fopen(sFilename.c_str(), "w");

	if (file == NULL) {
		fprintf(stderr, "Could not open file \"%s\" for writing\n", sFilename.c_str());
		return 255;
	}

	if (fprintf(file, "%s", convertMS2dat(pContentMS, iRecordIndex, fAngle1, fAngle2, iView, bQuiet, bVerbose).c_str()) < 0) {
		fprintf(stderr, "Error: Failed to write output file \"%s\"\n", sFilename.c_str());
		fclose(file);
		return 255;
	}

	fclose(file);

	return 0;
}

int exportPS(DAFFContentPS* pContentPS, const std::string& sFilename, int iRecordIndex, float fAngle1, float fAngle2, int iView, bool bQuiet, bool bVerbose) {
	FILE* file = fopen(sFilename.c_str(), "w");

	if (file == NULL) {
		fprintf(stderr, "Could not open file \"%s\" for writing\n", sFilename.c_str());
		return 255;
	}

	if (fprintf(file, "%s", convertPS2dat(pContentPS, iRecordIndex, fAngle1, fAngle2, iView, bQuiet, bVerbose).c_str()) < 0) {
		fprintf(stderr, "Error: Failed to write output file \"%s\"\n", sFilename.c_str());
		fclose(file);
		return 255;
	}

	fclose(file);

	return 0;
}

int exportMPS(DAFFContentMPS* pContent, const std::string& sFilename, int iRecordIndex, float fAngle1, float fAngle2, int iView, bool bQuiet, bool bVerbose) {
	FILE* file = fopen(sFilename.c_str(), "w");

	if (file == NULL) {
		fprintf(stderr, "Could not open file \"%s\" for writing\n", sFilename.c_str());
		return 255;
	}

	if (fprintf(file, "%s", convertMPS2dat(pContent, iRecordIndex, fAngle1, fAngle2, iView, bQuiet, bVerbose).c_str()) < 0) {
		fprintf(stderr, "Error: Failed to write output file \"%s\"\n", sFilename.c_str());
		fclose(file);
		return 255;
	}

	fclose(file);

	return 0;
}

int exportDFT(DAFFContentDFT* pContent, const std::string& sFilename, int iRecordIndex, float fAngle1, float fAngle2, int iView, bool bQuiet, bool bVerbose) {
	FILE* file = fopen(sFilename.c_str(), "w");

	if (file == NULL) {
		fprintf(stderr, "Could not open file \"%s\" for writing\n", sFilename.c_str());
		return 255;
	}

	if (fprintf(file, "%s", convertDFT2dat(pContent, iRecordIndex, fAngle1, fAngle2, iView, bQuiet, bVerbose).c_str()) < 0) {
		fprintf(stderr, "Error: Failed to write output file \"%s\"\n", sFilename.c_str());
		fclose(file);
		return 255;
	}

	fclose(file);

	return 0;
}

std::string convertMS2dat(DAFFContentMS* pContentMS, int iRecordIndex, float fAngle1, float fAngle2, int iView, bool bQuiet, bool bVerbose) {
	
	std::stringstream ss;

	ss << "# Support frequencies [Hz]" << std::endl
	   << "freqs = ";

	std::vector<float> vsFrequencies = pContentMS->getFrequencies();
	for (size_t i=0; i < vsFrequencies.size(); i++) {
		if (i>0) ss << ", ";
		ss << DAFFUtils::Float2StrNice(vsFrequencies[i], 3, false);
	}
	ss << std::endl;
	
	const DAFFMetadata* metadata = pContentMS->getParent()->getMetadata();
	ss << std::endl;
	ss << "# Metadata " << std::endl << metadata->toString() << std::endl << std::endl;

	ss << "# Magnitude spectrum data" << std::endl << std::endl;

	std::vector<float> vfData( pContentMS->getNumFrequencies() );

	int iStart = (iRecordIndex == -1 ? 0 : iRecordIndex);
	int iEnd = (iRecordIndex == -1 ? pContentMS->getProperties()->getNumberOfRecords()-1 : iRecordIndex);

	for (int iIndex=iStart; iIndex<=iEnd; iIndex++) {

		if (iRecordIndex == -1)
		{
			pContentMS->getRecordCoords(iIndex, iView, fAngle1, fAngle2);
		}

		/*
		ssLine.str("");
		ssLine << "# ";
		if (bReverseAngles) 
			ssLine << cAngle2ViewFlag << DAFFUtils::Double2StrNice(fBeta, 3, bObjectView, 3) << " " << cAngle1ViewFlag << DAFFUtils::Double2StrNice(fAlpha, 3, bObjectView, 3);
		else
			ssLine << cAngle1ViewFlag << DAFFUtils::Double2StrNice(fAlpha, 3, bObjectView, 3) << " " << cAngle2ViewFlag << DAFFUtils::Double2StrNice(fBeta, 3, bObjectView, 3);
		ss << ssLine.str() << std::endl;
		*/
		
		if (bVerbose) printf("Exporting magnitude spectrum for direction %s\n", DAFFUtils::StrDirection(iView, fAngle1, fAngle2).c_str());
		
			
		for (int iChannel=0; iChannel<pContentMS->getProperties()->getNumberOfChannels(); iChannel++) {
			float* pfDest = &vfData[0];

			pContentMS->getMagnitudes(iIndex, iChannel, pfDest);
			
			ss << "direction: " << DAFFUtils::StrDirectionCompact(iView, fAngle1, fAngle2) << ", channel: " << (iChannel+1) << " = ";

			for (int i=0; i < pContentMS->getNumFrequencies(); i++) {
				if (i>0) ss << ", ";
				ss << DAFFUtils::Float2StrNice( vfData[i], 9, false );
			}
			ss << std::endl;
		}

		metadata = pContentMS->getRecordMetadata(iIndex);
		if (!metadata->isEmpty())
			ss << "metadata: " << std::endl << metadata->toString() << std::endl << std::endl;
	}

	return ss.str();
}

std::string convertPS2dat(DAFFContentPS* pContentPS, int iRecordIndex, float fAngle1, float fAngle2, int iView, bool bQuiet, bool bVerbose) {
	
	std::stringstream ss;
	ss << "# Support frequencies [Hz]" << std::endl
	   << "freqs = ";

	std::vector<float> vsFrequencies = pContentPS->getFrequencies();
	for (size_t i=0; i < vsFrequencies.size(); i++) {
		if (i>0) ss << ", ";
		ss << DAFFUtils::Float2StrNice(vsFrequencies[i], 3, false);
	}
	ss << std::endl << std::endl;
	ss << "# Phase spectrum data" << std::endl << std::endl;

	std::vector<float> vfData( pContentPS->getNumFrequencies() );

	int iStart = (iRecordIndex == -1 ? 0 : iRecordIndex);
	int iEnd = (iRecordIndex == -1 ? pContentPS->getProperties()->getNumberOfRecords()-1 : iRecordIndex);

	for (int iIndex=iStart; iIndex<=iEnd; iIndex++) {

		if (iRecordIndex == -1)
		{
			pContentPS->getRecordCoords(iIndex, iView, fAngle1, fAngle2);
		}

		/*
		ssLine.str("");
		ssLine << "# ";
		if (bReverseAngles) 
			ssLine << cAngle2ViewFlag << DAFFUtils::Double2StrNice(fBeta, 3, bObjectView, 3) << " " << cAngle1ViewFlag << DAFFUtils::Double2StrNice(fAlpha, 3, bObjectView, 3);
		else
			ssLine << cAngle1ViewFlag << DAFFUtils::Double2StrNice(fAlpha, 3, bObjectView, 3) << " " << cAngle2ViewFlag << DAFFUtils::Double2StrNice(fBeta, 3, bObjectView, 3);
		ss << ssLine.str() << std::endl;
		*/
		
		if (bVerbose) printf("Exporting phase spectrum for direction %s\n", DAFFUtils::StrDirection(iView, fAngle1, fAngle2).c_str());
		
			
		for (int iChannel=0; iChannel<pContentPS->getProperties()->getNumberOfChannels(); iChannel++) {
			float* pfDest = &vfData[0];

			pContentPS->getPhases(iIndex, iChannel, pfDest);

			ss << "direction: " << DAFFUtils::StrDirectionCompact(iView, fAngle1, fAngle2) << ", channel: " << (iChannel+1) << " = ";

			for (int i=0; i < pContentPS->getNumFrequencies(); i++) {
				if (i>0) ss << ", ";
				ss << DAFFUtils::Float2StrNice( vfData[i], 9, false );
			}
			ss << std::endl;

		}

		const DAFFMetadata* metadata = pContentPS->getRecordMetadata(iIndex);
		if (!metadata->isEmpty())
			ss << "metadata: " << std::endl << metadata->toString() << std::endl << std::endl;
	}

	return ss.str();
}


std::string convertMPS2dat(DAFFContentMPS* pContent, int iRecordIndex, float fAngle1, float fAngle2, int iView, bool bQuiet, bool bVerbose) {
	
	std::stringstream ss;
	ss << "# Support frequencies [Hz]" << std::endl
	   << "freqs = ";

	std::vector<float> vsFrequencies = pContent->getFrequencies();
	for (size_t i=0; i < vsFrequencies.size(); i++) {
		if (i>0) ss << ", ";
		ss << DAFFUtils::Float2StrNice(vsFrequencies[i], 3, false);
	}
	ss << std::endl << std::endl;
	ss << "# Magnitude-phase spectrum data" << std::endl << std::endl;

	int iStart = (iRecordIndex == -1 ? 0 : iRecordIndex);
	int iEnd = (iRecordIndex == -1 ? pContent->getProperties()->getNumberOfRecords()-1 : iRecordIndex);

	for (int iIndex=iStart; iIndex<=iEnd; iIndex++) {

		if (iRecordIndex == -1)
		{
			pContent->getRecordCoords(iIndex, iView, fAngle1, fAngle2);
		}
		
		if (bVerbose) printf("Exporting magnitude-phase spectrum for direction %s\n", DAFFUtils::StrDirection(iView, fAngle1, fAngle2).c_str());
		
			
		for (int iChannel=0; iChannel < pContent->getProperties()->getNumberOfChannels(); iChannel++) {
			float* pfDest = new float[2*pContent->getNumFrequencies()];

			pContent->getCoefficientsRI(iIndex, iChannel, pfDest);

			ss << "direction: " << DAFFUtils::StrDirectionCompact(iView, fAngle1, fAngle2) << ", channel: " << (iChannel+1) << " = ";

			for (int i=0; i < 2*pContent->getNumFrequencies(); i++) {
				if (i>0) ss << ", ";
				ss << "(" << DAFFUtils::Float2StrNice( pfDest[2*i], 9, false ) << ", " << DAFFUtils::Float2StrNice( pfDest[2*i+1], 9, false ) << ")";
			}
			ss << std::endl;

			delete[] pfDest;
		}

		const DAFFMetadata* metadata = pContent->getRecordMetadata(iIndex);
		if (!metadata->isEmpty())
			ss << "metadata: " << std::endl << metadata->toString() << std::endl << std::endl;
	}

	return ss.str();
}

std::string convertDFT2dat(DAFFContentDFT* pContent, int iRecordIndex, float fAngle1, float fAngle2, int iView, bool bQuiet, bool bVerbose) {
	
	std::stringstream ss;
	ss << "# DFT spectrum data" << std::endl << std::endl;

	int iStart = (iRecordIndex == -1 ? 0 : iRecordIndex);
	int iEnd = (iRecordIndex == -1 ? pContent->getProperties()->getNumberOfRecords()-1 : iRecordIndex);

	for (int iIndex=iStart; iIndex<=iEnd; iIndex++) {

		if (iRecordIndex == -1)
		{
			pContent->getRecordCoords(iIndex, iView, fAngle1, fAngle2);
		}
		
		if (bVerbose) printf("Exporting DFT spectrum for direction %s\n", DAFFUtils::StrDirection(iView, fAngle1, fAngle2).c_str());
		
			
		for (int iChannel=0; iChannel < pContent->getProperties()->getNumberOfChannels(); iChannel++) {
			float* pfDest = new float[2*pContent->getNumDFTCoeffs()];

			pContent->getDFTCoeffs(iIndex, iChannel, pfDest);

			ss << "direction: " << DAFFUtils::StrDirectionCompact(iView, fAngle1, fAngle2) << ", channel: " << (iChannel+1) << " = ";

			for (int i=0; i < pContent->getNumDFTCoeffs(); i++) {
				if (i>0) ss << ", ";
				ss << "(" << DAFFUtils::Float2StrNice( pfDest[2*i], 9, false ) << ", " << DAFFUtils::Float2StrNice( pfDest[2*i+1], 9, false ) << ")";
			}
			ss << std::endl;

			delete[] pfDest;
		}

		const DAFFMetadata* metadata = pContent->getRecordMetadata(iIndex);
		if (!metadata->isEmpty())
			ss << "metadata: " << std::endl << metadata->toString() << std::endl << std::endl;
	}

	return ss.str();
}
