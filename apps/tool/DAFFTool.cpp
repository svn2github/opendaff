/*
* -------------------------------------------------------------------------------------
*
*  OpenDAFF - A free, open source software package for directional audio data
*  OpenDAFF is distributed under the terms of the GNU Lesser Public License (LGPL)
*
*  Copyright (c) Institute of Technical Acoustics, RWTH Aachen University, 2009-2016
*
*  ------------------------------------------------------------------------------------
*
*  Visit the OpenDAFF homepage: http://www.opendaff.org
*
*  ------------------------------------------------------------------------------------
*
*  License and warranty notes
*
*  OpenDAFF is free software, distributed under the terms of the
*  GNU Lesser General Public License (LGPL) version 3.
*  You can redistribute it and/or modify it under the terms of the
*  GNU Lesser General Public License (LGPL) version 3,
*  as published by the Free Software Foundation.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*  See the GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*  ------------------------------------------------------------------------------------
*
*/

#include <DAFF.h>

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <iomanip>

#if defined(WIN32)
#include <windows.h>
#else
#include <sys/stat.h>
#endif

#include <sndfile.h>

#ifndef __GNU_LIBRARY__
#define __GNU_LIBRARY__
#endif
#include "getopt/getopt.h"

#include "Export.h"

using namespace std;

#define PROGRAM_NAME "DAFFTool"
#define PROGRAM_BLANK "        "
#define EXECUTABLE_NAME "dafftool"
#define VERSION "v1.7"
#define SYNTAX "MODE [OPTIONS] FILENAME"
#define COPYRIGHT_YEARS "2008-2016"
#define SEPARATOR "-------------------------------------------------------------------"

// Access rights when creating directories with Posix 'mkdir'
#if !defined(WIN32)
const int DIRECTORY_MASK = S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH;
#endif

// Define necessary roundf for Microsoft compilers
#ifdef _MSC_VER 
#define roundf(x) (x<0?ceil((x)-0.5f):floor((x)+0.5f));
#pragma warning(disable: 4996)
#endif // _MSC_VER

#if !defined(WIN32)
std::string sPathSeparator = "/";
#else
std::string sPathSeparator = "\\";
#endif

// Global variables
DAFFReader* g_pDAFFReader = NULL;

// Forward declaration of mode-dependent main functions
int main_info(int argc, char* argv[]);
int main_dump(int argc, char* argv[]);
int main_query(int argc, char* argv[]);

/* +-----------------------------------------------+
   |                                               |
   |    Utility functions                          |
   |                                               |
   +-----------------------------------------------+ */

std::string stripFilename(const std::string& sFilename) {
	int i = (int) sFilename.length()-1;
	if (sFilename.find('.') != string::npos) 
		i = (int) sFilename.find_last_of('.');

	size_t j = 0;
	if (sFilename.find(sPathSeparator.c_str()) != string::npos)
		j = (int) sFilename.find_last_of(sPathSeparator.c_str()) + 1;

	return ((i == -1) || (i==((int) sFilename.length()-1)) ? "" : sFilename.substr(j, i-j));
}

bool isDirectory(const std::string& sPath) {
#if !defined(WIN32)
	struct stat status;
	return !sPath.empty() && (::stat(sPath.c_str(), &status)==0) && S_ISDIR(status.st_mode);
#else
	DWORD atts;
	return !sPath.empty() && ((atts=GetFileAttributesA(sPath.c_str())) != 0xFFFFFFFF) && (atts & FILE_ATTRIBUTE_DIRECTORY);
#endif
}

bool doesPathExist(const std::string& sDir) {
	// Inspiration by FOX
#if !defined(WIN32)
	struct stat status;
	return !sDir.empty() && (::stat(sDir.c_str(), &status)==0);
#else
	return !sDir.empty() && (GetFileAttributesA(sDir.c_str()) != 0xFFFFFFFF);
#endif
}


/* +-----------------------------------------------+
   |                                               |
   |    Global functions                           |
   |                                               |
   +-----------------------------------------------+ */

void syntax() {
	fprintf(stderr, "Syntax: %s %s\n", EXECUTABLE_NAME, SYNTAX);
	printf("Try '%s -h' or '%s MODE -h' for more information\n", EXECUTABLE_NAME, EXECUTABLE_NAME);
}

void version() {
	printf("%s %s\n", PROGRAM_NAME, VERSION);
}

void help() {
	printf("\n%s\n\n", SEPARATOR);
	printf("  %s is a swiss army knife for DAFF directional audio files\n", PROGRAM_NAME);
	printf("  %s is a part of the OpenDAFF software package\n\n", PROGRAM_NAME);
	printf("  (c) Copyright Institute for Technical Acoustics (ITA),\n");
	printf("                RWTH Aachen University, Germany, %s\n\n", COPYRIGHT_YEARS );
	printf("%s\n\n\n", SEPARATOR);

	printf("Syntax:  \t%s %s\n", EXECUTABLE_NAME, SYNTAX);
	printf("         \t%s -h\n", EXECUTABLE_NAME);
	printf("         \t%s -v\n\n", EXECUTABLE_NAME);

	printf("Modes:   \tinfo \tDisplay information\n");
	printf("         \tdump \tDump complete contents\n");
	printf("         \tquery\tQuery single records\n\n");
	
	printf("Options: \t-h   \tDisplay this information\n");
	printf("         \t-v   \tDisplay the program version\n\n\n");

	printf("Examples:\t%s info trombone.daff\n", EXECUTABLE_NAME);
	printf("         \t%s dump hrir.daff\n", EXECUTABLE_NAME);
	printf("         \t%s query loudspeaker.daff P10 T-84\n\n\n", EXECUTABLE_NAME);

	printf( "Version: \tThis is %s %s\n\n", PROGRAM_NAME, VERSION );

	printf("%s\n\n", SEPARATOR);
	printf("  You can find more information in the OpenDAFF User Guide\n");
	printf("  and on the OpenDAFF website: http://www.opendaff.org\n\n");
	printf("%s\n", SEPARATOR);
}

void help_info() {
	printf("\n%s\n", SEPARATOR);
	printf(" Info mode - Print information on a DAFF file\n");
	printf("%s\n\n", SEPARATOR);

	printf("Syntax:  \t%s info DAFFFILENAME\n\n", EXECUTABLE_NAME);
	printf("Options: \tThere are no options available\n\n");
	printf("Examples:\t%s info trombone.daff\n\n", EXECUTABLE_NAME);
}

// Info query functions
void help_query() {
	printf("\n%s\n", SEPARATOR);
	printf(" Query mode - Query a single record from a DAFF file\n");
	printf("%s\n\n", SEPARATOR);
	
	printf("Syntax:  \t%s query [OPTIONS] DAFFFILENAME DIRECTION\n\n", EXECUTABLE_NAME);

	printf("Directions:\tA### B###\tData view direction (alpha and beta)\n");
	printf("           \tP### T###\tObject view direction (azimuth and elevation)\n\n");

	printf("Options: \t-c	     \tPrint data (discards -d, -o, -p, -r)\n");
	printf("         \t-d DIR    \tOutput target directory\n");
	printf("         \t-f        \tForce mode, overwrite without prompt\n");
	printf("         \t-o        \tOutput file name (discards -p, -r)\n");
	printf("         \t-p PREFIX \tSet output prefix\n");
	printf("         \t-q        \tQuiet output (discards -v)\n");
	printf("         \t-r        \tReverse angle order of output filenames\n");
	printf("         \t          \t(first Theta/Beta, then Phi/Alpha)\n");
	printf("         \t-v        \tVerbose output\n\n");

	printf("Examples:\t%s query -v -d TRBN2 -p TRBN trombone.daff P+91.0 T-33.5\n", EXECUTABLE_NAME);
	printf("         \t%s query -rf -d HRIRNearFront -p NF hrir_near.daff A0 B90\n", EXECUTABLE_NAME);
	printf("         \t%s query -qf -o HRIRNearFront.wav hrir_near.daff P0 T0\n", EXECUTABLE_NAME);
	printf("         \t%s query -qc trombone.daff P0 T0\n", EXECUTABLE_NAME);
}

void help_dump() {
	printf("\n%s\n", SEPARATOR);
	printf(" Dump mode - Dump the whole set of records from a DAFF file\n");
	printf("%s\n\n", SEPARATOR);
	
	printf("Syntax:  \t%s dump [OPTIONS] DAFFFILENAME\n\n", EXECUTABLE_NAME);

	printf("Options: \t-c        \tUse object view angles for filenames\n");
	printf("         \t-d DIR	 \tSet output target directory\n");
	printf("         \t-f        \tForce mode, overwrite without prompt\n");
	printf("         \t-p PREFIX \tSet output prefix\n");
	printf("         \t-q        \tQuiet output (discards -v)\n");
	printf("         \t-r        \tReverse angle order of output filenames\n");
	printf("         \t          \t(first Theta/Beta, then Phi/Alpha)\n");
	printf("         \t-v        \tVerbose output\n\n");

	printf("Examples:\t%s dump -d TromboneDirectivity -p TRBN -v trombone.daff\n", EXECUTABLE_NAME);
	printf("         \t%s dump -cqrw hrir_near.daff\n", EXECUTABLE_NAME);
}

// Closes all open or allocated resources (memory, files, etc.)
void tidyup() {
	delete g_pDAFFReader;
}

// Global main function
int main(int argc, char* argv[]) {

	// At least we need one argument or option
	if (argc < 2) {
		syntax();

		return 255;
	}

	std::string sMode = "";

	// Find first argument without a leading minus, this must be the mode argument
	for (int i=1; i<argc; i++)
		if (argv[i][0] != '-') {
			sMode = argv[i];
			std::transform(sMode.begin(), sMode.end(), sMode.begin(), ::toupper);
			break;
		}

	// Non mode calls
	if (sMode == "") {
		std::string arg = argv[1];

		// Help dialog requested
		if (arg.compare("-h") == 0) {
			help();

			return 0;
		}

		// Version dialog requested
		if (arg.compare("-v") == 0) {
			version();

			return 0;
		}

		// Anything else is syntactically incorrect
		syntax();

		return 255;
	}

	// Switch to mode specific main function
	if (sMode == "INFO") return main_info(argc, argv);
	else if (sMode == "DUMP") return main_dump(argc, argv);
	else if (sMode == "QUERY") return main_query(argc, argv);
	else syntax();

	tidyup();

	return 255;
}

/* +-----------------------------------------------+
   |                                               |
   |    Info mode                                  |
   |                                               |
   +-----------------------------------------------+ */

int main_info(int argc, char* argv[]) {

	int c;
	while ((c = getopt(argc, argv, "hv")) != -1) {
		switch (c) {
		case 'h':
			help_info();
			return 0;

		case '?':
			fprintf(stderr, "Error: Unknown option. Use '%s info -h' for help\n", EXECUTABLE_NAME);
			return 255;
		
		default:
			fprintf(stderr, "Error: Internal error\n");
			return 255;
		}
	}

	// Remaining number of non-option parameters
	int iArgs = argc-optind-1;

	if (iArgs != 1) {
		syntax();
		return 255;
	}

	string sInputFile = argv[optind+1];

	g_pDAFFReader = DAFFReader::create();
	int iError = g_pDAFFReader->openFile(sInputFile);
	if (iError != 0) {
		if (iError == DAFF_FILE_NOT_FOUND)
			fprintf(stderr, "Error: %s (\"%s\")\n", DAFFUtils::StrError(iError).c_str(), sInputFile.c_str());
		else
			fprintf(stderr, "Error: %s\n", DAFFUtils::StrError(iError).c_str());
		return iError;
	}

	const DAFFProperties* pProps = g_pDAFFReader->getProperties();
	DAFFOrientationYPR o;
	g_pDAFFReader->getProperties()->getOrientation(o);
	int iVersion = int( g_pDAFFReader->getFileFormatVersion() );

	printf("\n--= Properties =--\n\n");

	printf("File format version: %d\n", iVersion, 3 );
	printf("Content type:        %s\n", DAFFUtils::StrContentType( pProps->getContentType() ).c_str());
	printf("Quantization:        %s\n\n", DAFFUtils::StrQuantizationType( pProps->getQuantization() ).c_str());
	printf("Number of channels:  %i\n", pProps->getNumberOfChannels());
	printf("Number of records:   %i\n\n", pProps->getNumberOfRecords());
	printf("Alpha points:        %i\n", pProps->getAlphaPoints());
	printf("Alpha range:         [%s\xF8, %s\xF8]\n",
		   DAFFUtils::Float2StrNice(pProps->getAlphaStart(), 3, false).c_str(),
		   DAFFUtils::Float2StrNice(pProps->getAlphaEnd(), 3, false).c_str());
	printf("Beta points:         %i\n", pProps->getBetaPoints());
	printf("Beta range:          [%s\xF8, %s\xF8]\n",
		   DAFFUtils::Float2StrNice(pProps->getBetaStart(), 3, false).c_str(),
		   DAFFUtils::Float2StrNice(pProps->getBetaEnd(), 3, false).c_str());
	printf("Full sphere:         %s\n\n", pProps->coversFullSphere() ? "yes" : "no");
	printf("Orientation:         %s\n\n", o.toString().c_str());
	
	DAFFContentIR* pContentIR;
	DAFFContentMS* pContentMS;
	DAFFContentDFT* pContentDFT;

	switch (g_pDAFFReader->getContentType()) {
		case DAFF_IMPULSE_RESPONSE:
			pContentIR = dynamic_cast<DAFFContentIR*>(g_pDAFFReader->getContent());
			printf("Samplerate:          %.1f Hz\n", pContentIR->getSamplerate());
			printf("Filter length:       %i\n", pContentIR->getFilterLength());
			break;

		case DAFF_MAGNITUDE_SPECTRUM:
			pContentMS = dynamic_cast<DAFFContentMS*>(g_pDAFFReader->getContent());
			printf("Number of frequencies: %i\n", pContentMS->getNumFrequencies());
			printf("Frequency support:     ");
			for (size_t i=0; i<pContentMS->getFrequencies().size(); i++) {
				printf("%s", DAFFUtils::Float2StrNice(pContentMS->getFrequencies()[i], 3, false).c_str());
				if (i < (pContentMS->getFrequencies().size()-1)) printf(", ");
			}
			printf(" Hz\n");
			break;

		case DAFF_DFT_SPECTRUM:
			pContentDFT = dynamic_cast<DAFFContentDFT*>(g_pDAFFReader->getContent());
			printf("Samplerate:          %.1f Hz\n", pContentDFT->getSamplerate());
			printf("Number of coefficients: %i\n", pContentDFT->getNumDFTCoeffs());
			if (pContentDFT->isSymmetric())
				printf("Spectrum is symmetric\n");
			break;

		// TODO: Implement new content types ...
	}

	printf("\n\n--= Metadata =--\n\n");
	std::vector<std::string> vsKeyList;
	g_pDAFFReader->getMetadata()->getKeys(vsKeyList);
	
	for (std::vector<std::string>::const_iterator cit=vsKeyList.begin(); cit != vsKeyList.end(); ++cit) {
		std::string sType = std::string("[") + DAFFUtils::StrMetadataKeyType( g_pDAFFReader->getMetadata()->getKeyType(*cit) ) + std::string("]");
		printf("%-10s %s: %s\n", sType.c_str(),
			   (*cit).c_str(),
			   g_pDAFFReader->getMetadata()->getKeyString(*cit).c_str());
	}

	if (vsKeyList.empty()) printf("< No metadata defined >\n");
	printf("\n");

	tidyup();

	return 0;
}

/* +-----------------------------------------------+
   |                                               |
   |    Dump mode                                  |
   |                                               |
   +-----------------------------------------------+ */

int main_dump(int argc, char* argv[]) {
	bool bForce = false,
		 bObjectView = false,
		 bQuiet = false,
		 bReverseAngles = false,
		 bVerbose = false;
	std::string sFilePrefix, sTargetDirectory;

	int c;
	while ((c = getopt(argc, argv, "cd:fhp:qrv")) != -1)
		switch (c) {
		case 'c':
			bObjectView = true;
			break;

		case 'd':
			sTargetDirectory = optarg;
			break;

		case 'f':
			bForce = true;
			break;

		case 'h':
			help_dump();
			return 0;

		case 'p':
			sFilePrefix = optarg;
			break;

		case 'q':
			bQuiet = true;
			break;

		case 'r':
			bReverseAngles = true;
			break;

		case 'v':
			bVerbose = true;
			break;

		case '?':
			fprintf(stderr, "Error: Unknown option. Use '%s query -h' for help\n", EXECUTABLE_NAME);
			return 255;
		
		default:
			fprintf(stderr, "Error: Internal error\n");
			return 255;
	}

	// Remaining number of non-option parameters
	int iArgs = argc-optind-1;

	if (iArgs != 1) {
		syntax();
		return 255;
	}

	// Scanning input parameters
	string sInputFile = argv[optind+1];

	bVerbose = bQuiet ? false : bVerbose;

	// Open DAFF file
	g_pDAFFReader = DAFFReader::create();
	int iError = g_pDAFFReader->openFile(sInputFile);
	if (iError != 0) {
		if (iError == DAFF_FILE_NOT_FOUND)
			fprintf(stderr, "Error: %s (\"%s\")\n", DAFFUtils::StrError(iError).c_str(), sInputFile.c_str());
		else
			fprintf(stderr, "Error: %s\n", DAFFUtils::StrError(iError).c_str());
		return iError;
	}

	int iRecordIndex = 0;
	int iView = bObjectView ? DAFF_OBJECT_VIEW : DAFF_DATA_VIEW;
	
	float fAngle1;
	float fAngle2;

	char cAngle1ViewFlag, cAngle2ViewFlag;
	if (iView == DAFF_OBJECT_VIEW) {
		cAngle1ViewFlag = 'P';
		cAngle2ViewFlag = 'T';
	} else {
		cAngle1ViewFlag = 'A';
		cAngle2ViewFlag = 'B';
	}

	// DIRECTORY
	if (sTargetDirectory.length() == 0) {
		if (sFilePrefix.length() > 0)
			sTargetDirectory = sFilePrefix;
		else
			sTargetDirectory = stripFilename(sInputFile);
	}

	if (doesPathExist(sTargetDirectory)) {
		if (!isDirectory(sTargetDirectory)) {
			fprintf(stderr, "Error: \"%s\" is not a directory\n", sTargetDirectory.c_str());
			return 255;
		}
	} else {
		if (bVerbose) printf("Creating directory \"%s\"\n", sTargetDirectory.c_str());
		// Create target directory
#if !defined(WIN32)
		mkdir(sTargetDirectory.c_str(), DIRECTORY_MASK);
#else
		CreateDirectoryA(sTargetDirectory.c_str(), NULL);
#endif
	}
	if (bVerbose) printf("Using output directory directory \"%s\"\n", sTargetDirectory.c_str());

	// OUTFILE
	std::string sOutFileName;
	stringstream ssOutFileName;

	DAFFContentIR* pContentIR;
	DAFFContentMS* pContentMS;
	DAFFContentPS* pContentPS;
	DAFFContentMPS* pContentMPS;
	DAFFContentDFT* pContentDFT;

	int iResult = 0;

	switch (g_pDAFFReader->getContentType()) {

	/* ************************************** IMPULSE RESPONSE ************************************** */

	case DAFF_IMPULSE_RESPONSE:
		if (bVerbose) printf("Detected impulse response content type\n");
		pContentIR = dynamic_cast<DAFFContentIR*>(g_pDAFFReader->getContent());

		if (!bForce) {
			std::string sInput;
			printf("This will write %i files. Continue? [Y,n]: ", pContentIR->getProperties()->getNumberOfRecords());
			std::cin >> sInput;
			std::transform(sInput.begin(), sInput.end(), sInput.begin(), ::toupper);
			if ((sInput.compare("N") == 0)) {
				printf("Aborting\n");
				tidyup();
				return 255;
			}
		}

		for (iRecordIndex=0; iRecordIndex<g_pDAFFReader->getProperties()->getNumberOfRecords(); iRecordIndex++) {

			// Derive angles
			pContentIR->getRecordCoords(iRecordIndex, iView, fAngle1, fAngle2);

			// Generate output filename
			ssOutFileName.str("");
			ssOutFileName << sTargetDirectory << sPathSeparator;
			if (sFilePrefix.length() > 0)
				ssOutFileName << sFilePrefix << "_";
			else
				ssOutFileName << stripFilename(g_pDAFFReader->getFilename()) << "_";

			if (bReverseAngles) 
				ssOutFileName << cAngle2ViewFlag << DAFFUtils::Double2StrNice(fAngle2, 3, bObjectView, 3) << "_" << cAngle1ViewFlag << DAFFUtils::Double2StrNice(fAngle1, 3, bObjectView, 3);
			else
				ssOutFileName << cAngle1ViewFlag << DAFFUtils::Double2StrNice(fAngle1, 3, bObjectView, 3) << "_" << cAngle2ViewFlag << DAFFUtils::Double2StrNice(fAngle2, 3, bObjectView, 3);
			
			ssOutFileName << ".wav";
			sOutFileName = ssOutFileName.str();

			if (doesPathExist(sOutFileName) && !bForce) {
				std::string sInput;
				printf("File \"%s\" already exists, overwrite (yes, no, all)? [y,N,a]: ", sOutFileName.c_str());
				std::cin >> sInput;
				std::transform(sInput.begin(), sInput.end(), sInput.begin(), ::toupper);
				if (sInput.compare("Y") && sInput.compare("A")) {
					printf("Aborting ... consider using the query mode to pull specific data\n");
					tidyup();
					return 0;
				}
				if (sInput.compare("A") == 0) bForce = true;
			}
			iResult = exportIR(pContentIR, iRecordIndex, sOutFileName, iView, bQuiet, bVerbose);
			if (iResult != 0) {
				tidyup();
				return iResult;
			}
		}

		break;

	/* ************************************** MAGNITUDE SPECTRUM ************************************** */

	case DAFF_MAGNITUDE_SPECTRUM:
		if (bVerbose) printf("Detected magnitude spectrum content type\n");
		pContentMS = dynamic_cast<DAFFContentMS*>(g_pDAFFReader->getContent());

		// Write TEXTFILE
		ssOutFileName.str("");
		ssOutFileName << sTargetDirectory << sPathSeparator;

		if (sFilePrefix.length() > 0)
			ssOutFileName << sFilePrefix << ".txt";
		else
			ssOutFileName << stripFilename(g_pDAFFReader->getFilename()) << ".txt";

		sOutFileName = ssOutFileName.str();

		if (doesPathExist(sOutFileName) && !bForce) {
			std::string sInput;
			printf("File \"%s\" already exists, overwrite? [y,N]: ", sOutFileName.c_str());
			std::cin >> sInput;
			std::transform(sInput.begin(), sInput.end(), sInput.begin(), ::toupper);
			if (sInput.compare("Y") != 0) {
				printf("Leaving ...\n");
				tidyup();
				return 0;
			}
		}

		iResult = exportMS(pContentMS, sOutFileName, -1, 0, 0, iView, bQuiet, bVerbose);
		if (iResult != 0) {
			tidyup();
			return iResult;
		}

		if (!bQuiet) printf("Exported magnitude spectrum to file \"%s\"\n", sOutFileName.c_str());

		break;


	/* ************************************** PHASE SPECTRUM ************************************** */

	case DAFF_PHASE_SPECTRUM:
		if (bVerbose) printf("Detected phase spectrum content type\n");
		pContentPS = dynamic_cast<DAFFContentPS*>(g_pDAFFReader->getContent());

		// Write TEXTFILE
		ssOutFileName.str("");
		ssOutFileName << sTargetDirectory << sPathSeparator;

		if (sFilePrefix.length() > 0)
			ssOutFileName << sFilePrefix << ".txt";
		else
			ssOutFileName << stripFilename(g_pDAFFReader->getFilename()) << ".txt";

		sOutFileName = ssOutFileName.str();

		if (doesPathExist(sOutFileName) && !bForce) {
			std::string sInput;
			printf("File \"%s\" already exists, overwrite? [y,N]: ", sOutFileName.c_str());
			std::cin >> sInput;
			std::transform(sInput.begin(), sInput.end(), sInput.begin(), ::toupper);
			if (sInput.compare("Y") != 0) {
				printf("Leaving ...\n");
				tidyup();
				return 0;
			}
		}

		iResult = exportPS(pContentPS, sOutFileName, -1, 0, 0, iView, bQuiet, bVerbose);
		if (iResult != 0) {
			tidyup();
			return iResult;
		}

		if (!bQuiet) printf("Exported phase spectrum to file \"%s\"\n", sOutFileName.c_str());

		break;
	

	/* ************************************** MAGNITUDE PHASE SPECTRUM ************************************** */

	case DAFF_MAGNITUDE_PHASE_SPECTRUM:
		if (bVerbose) printf("Detected magnitude-phase spectrum content type\n");
		pContentMPS = dynamic_cast<DAFFContentMPS*>(g_pDAFFReader->getContent());

		// Write TEXTFILE
		ssOutFileName.str("");
		ssOutFileName << sTargetDirectory << sPathSeparator;

		if (sFilePrefix.length() > 0)
			ssOutFileName << sFilePrefix << ".txt";
		else
			ssOutFileName << stripFilename(g_pDAFFReader->getFilename()) << ".txt";

		sOutFileName = ssOutFileName.str();

		if (doesPathExist(sOutFileName) && !bForce) {
			std::string sInput;
			printf("File \"%s\" already exists, overwrite? [y,N]: ", sOutFileName.c_str());
			std::cin >> sInput;
			std::transform(sInput.begin(), sInput.end(), sInput.begin(), ::toupper);
			if (sInput.compare("Y") != 0) {
				printf("Leaving ...\n");
				tidyup();
				return 0;
			}
		}

		iResult = exportMPS(pContentMPS, sOutFileName, -1, 0, 0, iView, bQuiet, bVerbose);
		if (iResult != 0) {
			tidyup();
			return iResult;
		}

		if (!bQuiet) printf("Exported magnitude-phase spectrum to file \"%s\"\n", sOutFileName.c_str());

		break;
		

	/* ************************************** DFT SPECTRUM ************************************** */

	case DAFF_DFT_SPECTRUM:
		if (bVerbose) printf("Detected discrete fourier spectrum content type\n");
		pContentDFT = dynamic_cast<DAFFContentDFT*>(g_pDAFFReader->getContent());

		// Write TEXTFILE
		ssOutFileName.str("");
		ssOutFileName << sTargetDirectory << sPathSeparator;

		if (sFilePrefix.length() > 0)
			ssOutFileName << sFilePrefix << ".txt";
		else
			ssOutFileName << stripFilename(g_pDAFFReader->getFilename()) << ".txt";

		sOutFileName = ssOutFileName.str();

		if (doesPathExist(sOutFileName) && !bForce) {
			std::string sInput;
			printf("File \"%s\" already exists, overwrite? [y,N]: ", sOutFileName.c_str());
			std::cin >> sInput;
			std::transform(sInput.begin(), sInput.end(), sInput.begin(), ::toupper);
			if (sInput.compare("Y") != 0) {
				printf("Leaving ...\n");
				tidyup();
				return 0;
			}
		}

		iResult = exportDFT(pContentDFT, sOutFileName, -1, 0, 0, iView, bQuiet, bVerbose);
		if (iResult != 0) {
			tidyup();
			return iResult;
		}

		if (!bQuiet) printf("Exported discrete fourier spectrum to file \"%s\"\n", sOutFileName.c_str());

		break;
	}

	tidyup();

	return 0;
}

/* +-----------------------------------------------+
   |                                               |
   |    Query mode                                 |
   |                                               |
   +-----------------------------------------------+ */

int main_query(int argc, char* argv[]) {
	bool bConsoleOutput = false,
		 bForce = false,
		 bReverseAngles = false,
		 bQuiet = false,
		 bVerbose = false;
	std::string sFilePrefix,
				sFilename,
				sTargetDirectory;

	int c;
	while ((c = getopt(argc, argv, "cd:fho:p:qrv")) != -1)
		switch (c) {
		case 'c':
			bConsoleOutput = true;
			break;

		case 'd':
			sTargetDirectory = optarg;
			break;

		case 'f':
			bForce = true;
			break;

		case 'h':
			help_query();
			
			return 0;

		case 'o':
			sFilename = optarg;
			break;

		case 'p':
			sFilePrefix = optarg;
			break;

		case 'q':
			bQuiet = true;
			break;

		case 'r':
			bReverseAngles = true;
			break;

		case 'v':
			bVerbose = true;
			break;

		case '?':
			fprintf(stderr, "Error: Unknown option. Use '%s query -h' for help\n", EXECUTABLE_NAME);
			return 255;
		
		default:
			fprintf(stderr, "Error: Internal error\n");
			return 255;
	}
	
	// Remaining number of non-option parameters
	int iArgs = argc-optind-1;

	if (iArgs != 3) {
		syntax();

		return 255;
	}

	// Scanning input parameters
	string sInputFile = argv[optind+1];

	float fAngle1, fAngle2, fAngle1_projected, fAngle2_projected;
	const char cAngle1ViewFlag = toupper(argv[optind+2][0]);
	const char cAngle2ViewFlag = toupper(argv[optind+3][0]);
	const char* pcAngle1 = argv[optind+2]+1;
	const char* pcAngle2 = argv[optind+3]+1;
	
	int iView = -1;
	if ( (cAngle1ViewFlag == 'P') &&
		 (cAngle2ViewFlag == 'T') )
		 iView = DAFF_OBJECT_VIEW;
	if ( (cAngle1ViewFlag == 'A') &&
		 (cAngle2ViewFlag == 'B') )
		 iView = DAFF_DATA_VIEW;
	if (iView == -1) {
		fprintf(stderr, "Error: Could not determine whether the requested coordinates are of data or of object view perspective\n.");
		printf("Use '%s query -h' for help\n", EXECUTABLE_NAME);
		syntax();
		return 255;
	}
	if (bVerbose) iView == DAFF_DATA_VIEW ? printf("Using data view\n") : printf("Using object view\n");

	sscanf(pcAngle1, "%f", &fAngle1);
	sscanf(pcAngle2, "%f", &fAngle2);

	// Validations and prerequisites
	bVerbose = bQuiet ? false : bVerbose;

	// Angle projection - for output and filename
	DAFFUtils::NormalizeDirection(iView, fAngle1, fAngle2, fAngle1_projected, fAngle2_projected);
	if (bVerbose) {
		if ((fAngle1 != fAngle1_projected) || (fAngle2 != fAngle2_projected))
			printf("Normalized angles to %s\n", DAFFUtils::StrDirection(iView, fAngle1_projected, fAngle2_projected, 3, 3).c_str());
	}

	// DIRECTORY
	if (sTargetDirectory.length() == 0) {
		if (sFilePrefix.length() > 0)
			sTargetDirectory = sFilePrefix;
		else
			sTargetDirectory = stripFilename(sInputFile);
	}

	if (doesPathExist(sTargetDirectory)) {
		if (!isDirectory(sTargetDirectory)) {
			fprintf(stderr, "Error: \"%s\" is not a directory\n", sTargetDirectory.c_str());
			return 255;
		}
	} else {
		if (bVerbose) printf("Creating directory \"%s\"\n", sTargetDirectory.c_str());
		// Create target directory
#if !defined(WIN32)
		mkdir(sTargetDirectory.c_str(), DIRECTORY_MASK);
#else
		CreateDirectoryA(sTargetDirectory.c_str(), NULL);
#endif
	}
	if (bVerbose) printf("Using output directory \"%s\"\n", sTargetDirectory.c_str());
	
	// Open DAFF file
	g_pDAFFReader = DAFFReader::create();
	int iError = g_pDAFFReader->openFile(sInputFile);
	if (iError != 0) {
		if (iError == DAFF_FILE_NOT_FOUND)
			fprintf(stderr, "Error: %s (\"%s\")\n", DAFFUtils::StrError(iError).c_str(), sInputFile.c_str());
		else
			fprintf(stderr, "Error: %s\n", DAFFUtils::StrError(iError).c_str());

		return iError;
	}

	int iRecordIndex = 0;
	bool bOutOfBounds = false;

	// OUTFILE
	std::string sOutFileName;
	stringstream ssOutFileName;
	ssOutFileName << sTargetDirectory << sPathSeparator;
	if (sFilename.length() > 0 ) {
		sOutFileName = sTargetDirectory + sPathSeparator + sFilename;
	} else {
		if (sFilePrefix.length() > 0)
			ssOutFileName << sFilePrefix << "_";
		else
			ssOutFileName << stripFilename(g_pDAFFReader->getFilename()) << "_";
	}

	DAFFContentIR* pContentIR;
	DAFFContentMS* pContentMS;
	DAFFContentPS* pContentPS;
	DAFFContentMPS* pContentMPS;
	DAFFContentDFT* pContentDFT;

	int iResult = 0;

	/* ************************************** IMPULSE RESPONSE ************************************** */
	switch (g_pDAFFReader->getContentType()) {
	case DAFF_IMPULSE_RESPONSE:
		if (bVerbose) printf("Detected impulse response content type\n");

		pContentIR = dynamic_cast<DAFFContentIR*>(g_pDAFFReader->getContent());

		pContentIR->getNearestNeighbour(iView, fAngle1, fAngle2, iRecordIndex, bOutOfBounds);

		if (!bQuiet && bOutOfBounds) printf("Requested angle %s is out of bounds\n", DAFFUtils::StrDirection(iView, fAngle1, fAngle2, 3, 3).c_str());
		if (bVerbose) printf("Nearest neighbouring record index is %i\n", iRecordIndex);

		// Generate filename
		if (sOutFileName.length() == 0) {
			if (bReverseAngles) {
				if (iView == DAFF_DATA_VIEW)
					ssOutFileName << cAngle2ViewFlag << DAFFUtils::Double2StrNice(fAngle2_projected, 3, false, 3) << "_" << cAngle1ViewFlag << DAFFUtils::Double2StrNice(fAngle1_projected, 3, false, 3);
				else
					ssOutFileName << cAngle2ViewFlag << DAFFUtils::Double2StrNice(fAngle2_projected, 3, true, 3) << "_" << cAngle1ViewFlag << DAFFUtils::Double2StrNice(fAngle1_projected, 3, true, 3);
			} else {
				if (iView == DAFF_DATA_VIEW)
					ssOutFileName << cAngle1ViewFlag << DAFFUtils::Double2StrNice(fAngle1_projected, 3, false, 3) << "_" << cAngle2ViewFlag << DAFFUtils::Double2StrNice(fAngle2_projected, 3, false, 3);
				else
					ssOutFileName << cAngle1ViewFlag << DAFFUtils::Double2StrNice(fAngle1_projected, 3, true, 3) << "_" << cAngle2ViewFlag << DAFFUtils::Double2StrNice(fAngle2_projected, 3, true, 3);
			}
			ssOutFileName << ".wav";
			sOutFileName = ssOutFileName.str();
		}

		if (doesPathExist(sOutFileName) && !bForce) {
			std::string sInput;
			printf("File \"%s\" already exists, overwrite? [y,N]: ", sOutFileName.c_str());
			std::cin >> sInput;
			std::transform(sInput.begin(), sInput.end(), sInput.begin(), ::toupper);
			if (sInput.compare("Y") != 0){
				tidyup();
				return 0;
			}
		}

		// Write SOUNDFILE
		iResult = exportIR(pContentIR, iRecordIndex, sOutFileName, iView, bQuiet, bVerbose);
		if (iResult != 0) {
			tidyup();
			return iResult;
		}

		break;

	/* ************************************** MAGNITUDE SPECTRUM ************************************** */
	case DAFF_MAGNITUDE_SPECTRUM:
		if (bVerbose) printf("Detected magnitude spectrum content type\n");
		pContentMS = dynamic_cast<DAFFContentMS*>(g_pDAFFReader->getContent());

		pContentMS->getNearestNeighbour(iView, fAngle1, fAngle2, iRecordIndex, bOutOfBounds);

		if (!bQuiet && bOutOfBounds) printf("Requested angle %s is out of bounds\n", DAFFUtils::StrDirection(iView, fAngle1, fAngle2,3).c_str());
		if (bVerbose) printf("Nearest neighbouring record index is %i\n", iRecordIndex);

		// CONSOLE output
		if (bConsoleOutput) {
			printf("\n%s", convertMS2dat(pContentMS, iRecordIndex, fAngle1_projected, fAngle2_projected, iView, bQuiet, bVerbose).c_str());
			return 0;
		}

		// Generate filename
		if (sOutFileName.length() == 0) {
			if (iView == DAFF_DATA_VIEW)
				ssOutFileName << cAngle1ViewFlag << DAFFUtils::Double2StrNice(fAngle1_projected, 3, false, 3) << "_" << cAngle2ViewFlag << DAFFUtils::Double2StrNice(fAngle2_projected, 3, false, 3);
			else
				ssOutFileName << cAngle1ViewFlag << DAFFUtils::Double2StrNice(fAngle1_projected, 3, true, 3) << "_" << cAngle2ViewFlag << DAFFUtils::Double2StrNice(fAngle2_projected, 3, true, 3);
			ssOutFileName << ".txt";
			sOutFileName = ssOutFileName.str();
		}

		if (doesPathExist(sOutFileName) && !bForce) {
			std::string sInput;
			printf("File \"%s\" already exists, overwrite? [y,N]: ", sOutFileName.c_str());
			std::cin >> sInput;
			std::transform(sInput.begin(), sInput.end(), sInput.begin(), ::toupper);
			if (sInput.compare("Y") != 0){
				tidyup();
				return 0;
			}
		}
		
		// Write TEXTFILE
		iResult = exportMS(pContentMS, sOutFileName, iRecordIndex, fAngle1_projected, fAngle2_projected, iView, bQuiet, bVerbose);
		if (iResult != 0) {
			tidyup();
			return iResult;
		}

		if (!bQuiet) printf("Exported magnitude spectrum to file \"%s\"\n", sOutFileName.c_str());

		break;


	/* ************************************** PHASE SPECTRUM ************************************** */
	case DAFF_PHASE_SPECTRUM:
		if (bVerbose) printf("Detected phase spectrum content type\n");
		pContentPS = dynamic_cast<DAFFContentPS*>(g_pDAFFReader->getContent());

		pContentPS->getNearestNeighbour(iView, fAngle1, fAngle2, iRecordIndex, bOutOfBounds);

		if (!bQuiet && bOutOfBounds) printf("Requested angle %s is out of bounds\n", DAFFUtils::StrDirection(iView, fAngle1, fAngle2,3).c_str());
		if (bVerbose) printf("Nearest neighbouring record index is %i\n", iRecordIndex);

		// CONSOLE output
		if (bConsoleOutput) {
			printf("\n%s", convertPS2dat(pContentPS, iRecordIndex, fAngle1_projected, fAngle2_projected, iView, bQuiet, bVerbose).c_str());
			return 0;
		}

		// Generate filename
		if (sOutFileName.length() == 0) {
			if (iView == DAFF_DATA_VIEW)
				ssOutFileName << cAngle1ViewFlag << DAFFUtils::Double2StrNice(fAngle1_projected, 3, false, 3) << "_" << cAngle2ViewFlag << DAFFUtils::Double2StrNice(fAngle2_projected, 3, false, 3);
			else
				ssOutFileName << cAngle1ViewFlag << DAFFUtils::Double2StrNice(fAngle1_projected, 3, true, 3) << "_" << cAngle2ViewFlag << DAFFUtils::Double2StrNice(fAngle2_projected, 3, true, 3);
			ssOutFileName << ".txt";
			sOutFileName = ssOutFileName.str();
		}

		if (doesPathExist(sOutFileName) && !bForce) {
			std::string sInput;
			printf("File \"%s\" already exists, overwrite? [y,N]: ", sOutFileName.c_str());
			std::cin >> sInput;
			std::transform(sInput.begin(), sInput.end(), sInput.begin(), ::toupper);
			if (sInput.compare("Y") != 0){
				tidyup();
				return 0;
			}
		}
		
		// Write TEXTFILE
		iResult = exportPS(pContentPS, sOutFileName, iRecordIndex, fAngle1_projected, fAngle2_projected, iView, bQuiet, bVerbose);
		if (iResult != 0) {
			tidyup();
			return iResult;
		}

		if (!bQuiet) printf("Exported phase spectrum to file \"%s\"\n", sOutFileName.c_str());

		break;
		

	/* ************************************** MAGNITUDE PHASE SPECTRUM ************************************** */
	case DAFF_MAGNITUDE_PHASE_SPECTRUM:
		if (bVerbose) printf("Detected magnitude-phase spectrum content type\n");
		pContentMPS = dynamic_cast<DAFFContentMPS*>(g_pDAFFReader->getContent());

		pContentMPS->getNearestNeighbour(iView, fAngle1, fAngle2, iRecordIndex, bOutOfBounds);

		if (!bQuiet && bOutOfBounds) printf("Requested angle %s is out of bounds\n", DAFFUtils::StrDirection(iView, fAngle1, fAngle2,3).c_str());
		if (bVerbose) printf("Nearest neighbouring record index is %i\n", iRecordIndex);

		// CONSOLE output
		if (bConsoleOutput) {
			printf("\n%s", convertMPS2dat(pContentMPS, iRecordIndex, fAngle1_projected, fAngle2_projected, iView, bQuiet, bVerbose).c_str());
			return 0;
		}

		// Generate filename
		if (sOutFileName.length() == 0) {
			if (iView == DAFF_DATA_VIEW)
				ssOutFileName << cAngle1ViewFlag << DAFFUtils::Double2StrNice(fAngle1_projected, 3, false, 3) << "_" << cAngle2ViewFlag << DAFFUtils::Double2StrNice(fAngle2_projected, 3, false, 3);
			else
				ssOutFileName << cAngle1ViewFlag << DAFFUtils::Double2StrNice(fAngle1_projected, 3, true, 3) << "_" << cAngle2ViewFlag << DAFFUtils::Double2StrNice(fAngle2_projected, 3, true, 3);
			ssOutFileName << ".txt";
			sOutFileName = ssOutFileName.str();
		}

		if (doesPathExist(sOutFileName) && !bForce) {
			std::string sInput;
			printf("File \"%s\" already exists, overwrite? [y,N]: ", sOutFileName.c_str());
			std::cin >> sInput;
			std::transform(sInput.begin(), sInput.end(), sInput.begin(), ::toupper);
			if (sInput.compare("Y") != 0){
				tidyup();
				return 0;
			}
		}
		
		// Write TEXTFILE
		iResult = exportMPS(pContentMPS, sOutFileName, iRecordIndex, fAngle1_projected, fAngle2_projected, iView, bQuiet, bVerbose);
		if (iResult != 0) {
			tidyup();
			return iResult;
		}

		if (!bQuiet) printf("Exported magnitude-phase spectrum to file \"%s\"\n", sOutFileName.c_str());

		break;
		

	/* ************************************** DFT SPECTRUM ************************************** */
	case DAFF_DFT_SPECTRUM:
		if (bVerbose) printf("Detected DFT spectrum content type\n");
		pContentDFT = dynamic_cast<DAFFContentDFT*>(g_pDAFFReader->getContent());

		pContentDFT->getNearestNeighbour(iView, fAngle1, fAngle2, iRecordIndex, bOutOfBounds);

		if (!bQuiet && bOutOfBounds) printf("Requested angle %s is out of bounds\n", DAFFUtils::StrDirection(iView, fAngle1, fAngle2,3).c_str());
		if (bVerbose) printf("Nearest neighbouring record index is %i\n", iRecordIndex);

		// CONSOLE output
		if (bConsoleOutput) {
			printf("\n%s", convertDFT2dat(pContentDFT, iRecordIndex, fAngle1_projected, fAngle2_projected, iView, bQuiet, bVerbose).c_str());
			return 0;
		}

		// Generate filename
		if (sOutFileName.length() == 0) {
			if (iView == DAFF_DATA_VIEW)
				ssOutFileName << cAngle1ViewFlag << DAFFUtils::Double2StrNice(fAngle1_projected, 3, false, 3) << "_" << cAngle2ViewFlag << DAFFUtils::Double2StrNice(fAngle2_projected, 3, false, 3);
			else
				ssOutFileName << cAngle1ViewFlag << DAFFUtils::Double2StrNice(fAngle1_projected, 3, true, 3) << "_" << cAngle2ViewFlag << DAFFUtils::Double2StrNice(fAngle2_projected, 3, true, 3);
			ssOutFileName << ".txt";
			sOutFileName = ssOutFileName.str();
		}

		if (doesPathExist(sOutFileName) && !bForce) {
			std::string sInput;
			printf("File \"%s\" already exists, overwrite? [y,N]: ", sOutFileName.c_str());
			std::cin >> sInput;
			std::transform(sInput.begin(), sInput.end(), sInput.begin(), ::toupper);
			if (sInput.compare("Y") != 0){
				tidyup();
				return 0;
			}
		}
		
		// Write TEXTFILE
		iResult = exportDFT(pContentDFT, sOutFileName, iRecordIndex, fAngle1_projected, fAngle2_projected, iView, bQuiet, bVerbose);
		if (iResult != 0) {
			tidyup();
			return iResult;
		}

		if (!bQuiet) printf("Exported DFT spectrum to file \"%s\"\n", sOutFileName.c_str());

		break;
	}

	tidyup();

	return 0;
}
