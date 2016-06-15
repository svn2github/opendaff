/*
* -------------------------------------------------------------------------------------
*
*  OpenDAFF - A free, open source software package for directional audio data
*
*  ------------------------------------------------------------------------------------
*
*  Visit the OpenDAFF website: http://www.opendaff.org
*
*  ------------------------------------------------------------------------------------
*
*  Copyright 2016 Institute of Technical Acoustics, RWTH Aachen University
*
*  Licensed under the Apache License, Version 2.0 (the "License");
*  you may not use the OpenDAFF software package except in compliance with the License.
*  You may obtain a copy of the License at
*
*    http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS,
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
*
*  ------------------------------------------------------------------------------------
*
*/


// Matlab includes
#include <mex.h>
#include <matrix.h>

// DAFF includes
#include <DAFF.h>

// STL includes
#include <algorithm>
#include <map>

// Project includes
#include "Helpers.h"

// Command forward declarations
void Open(int, mxArray**, int, const mxArray**);
void Close(int, mxArray**, int, const mxArray**);
void GetMetadata(int, mxArray**, int, const mxArray**);
void GetRecordMetadata(int, mxArray**, int, const mxArray**);
void GetProperties(int, mxArray**, int, const mxArray**);
void GetRecordCoords(int, mxArray**, int, const mxArray**);
void GetRecordByIndex(int, mxArray**, int, const mxArray**);
void GetNearestNeighbourRecord(int, mxArray**, int, const mxArray**);
void GetCellRecords(int, mxArray**, int, const mxArray**);
void GetNearestNeighbourIndex(int, mxArray**, int, const mxArray**);
void GetCell(int, mxArray**, int, const mxArray**);

// Handle datatype
typedef int32_t HANDLE;

// Reader wrapper/adapter
class TARGET
{
public:
	DAFFReader* pReader;		// Associated reader
	float* pfBuffer;			// Help buffer for conversions

	inline TARGET( DAFFReader* pReader )
	{
		this->pReader = pReader;

		// Allocate the conversion buffer
		const DAFFProperties* pProps = pReader->getProperties();
		int iContentType = pProps->getContentType();
		
		if (iContentType == DAFF_IMPULSE_RESPONSE) {
			DAFFContentIR* pContent = dynamic_cast<DAFFContentIR*>( pReader->getContent() );
			pfBuffer = new float[pContent->getFilterLength()];
		}

		if (iContentType == DAFF_MAGNITUDE_SPECTRUM) {
			DAFFContentMS* pContent = dynamic_cast<DAFFContentMS*>( pReader->getContent() );
			pfBuffer = new float[pContent->getNumFrequencies()];
		}

		if (iContentType == DAFF_PHASE_SPECTRUM) {
			DAFFContentPS* pContent = dynamic_cast<DAFFContentPS*>( pReader->getContent() );
			pfBuffer = new float[pContent->getNumFrequencies()];
		}
		
		if (iContentType == DAFF_MAGNITUDE_PHASE_SPECTRUM) {
			DAFFContentMPS* pContent = dynamic_cast<DAFFContentMPS*>( pReader->getContent() );
			pfBuffer = new float[2*pContent->getNumFrequencies()];
		}
		
		if (iContentType == DAFF_DFT_SPECTRUM) {
			DAFFContentDFT* pContent = dynamic_cast<DAFFContentDFT*>( pReader->getContent() );
			pfBuffer = new float[2*pContent->getNumDFTCoeffs()];
		}
	}

	inline ~TARGET()
	{
		delete pReader;
		delete[] pfBuffer;
	}
};

// Maps handle => reader
typedef std::pair<HANDLE, TARGET*> HANDLE_PAIR;
typedef std::map<HANDLE, TARGET*> HANDLE_MAP;
const mxClassID HANDLE_CLASS_ID = mxINT32_CLASS;

HANDLE hHandleCount=1;	// Global handle counter
HANDLE_MAP mHandles;	// Map handle->reader


/* +------------------------------------------------------+
   |                                                      |
   |   Global functions                                   |
   |                                                      |
   +------------------------------------------------------+ */

// Search for a handle and return the map iterator
HANDLE_MAP::iterator GetHandleIterator(const mxArray *pArray) {
	// 1st make sure that the parameter has a valid type (scalar + handle datatype)
	if (!isScalar(pArray) || (mxGetClassID(pArray) != HANDLE_CLASS_ID))
		mexErrMsgTxt("Invalid handle");

	// 2nd search the handle in the map
	HANDLE hHandle = *((HANDLE*) mxGetData(pArray));
	HANDLE_MAP::iterator it = mHandles.find(hHandle);

	if (it == mHandles.end())
		mexErrMsgTxt("Invalid handle");

	return it;
}

// Search for a handle and return the associated DAFF reader
TARGET* GetHandleTarget(const mxArray *pArray) {
	HANDLE_MAP::iterator it = GetHandleIterator(pArray);
	return it->second;
}


/* +------------------------------------------------------+
   |                                                      |
   |   MEX Entry function                                 |
   |                                                      |
   +------------------------------------------------------+ */

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	DAFFVersion version;
	DAFFUtils::getLibraryVersion( version );

	// At least one argument must be specified
	if (nrhs < 1) mexErrMsgTxt("Command expected");

	std::string sCommand;
	if (!getString(prhs[0], sCommand))
		mexErrMsgTxt("Command must be a string");

	// Convert mode string to uppercase
	std::transform(sCommand.begin(), sCommand.end(), sCommand.begin(), tolower);
	bool bMatch=false;

	if( sCommand == "getversion" )
	{
		std::stringstream ss;
		ss << "OpenDAFF Matlab executable version " << version.sVersion;
		plhs[ 0 ] = mxCreateString( ss.str().c_str() );
		bMatch = true;
	}

	if (sCommand == "help")
	{
		mexPrintf( "\n  OpenDAFF Matlab Extension (MEX) Version %s\n", version.sVersion.c_str() );
		mexPrintf("  (c) Copyright 2016 Institut für Technische Akustik (ITA), RWTH Aachen University\n\n");
		mexPrintf("  The OpenDAFF Matlab executable is part of the OpenDAFF software package\n");
		mexPrintf("  For more information visit the OpenDAFF website: http://www.opendaff.org\n\n");
		mexPrintf("  Note: A full description of this module can be found in the HTML documentation\n\n");

		mexPrintf("  Syntax: DAFF('<COMMAND>' [, <OPTIONS>])\n\n");

		mexPrintf("  Command \"help\"\n\n");
		mexPrintf("      Shows this information\n\n\n");

		mexPrintf("  Command \"open\"\n\n");
		mexPrintf("      Open a DAFF file\n\n");
		mexPrintf("      Syntax:     [handle] = DAFF('open', filename)\n\n");
		mexPrintf("      Parameters: filename	char		Filename of the DAFF file\n\n");
		mexPrintf("      Returns:    handle     1x1 int32	Handle\n\n\n");
	
		mexPrintf("  Command \"close\"\n\n");
		mexPrintf("      Closes an opened DAFF file\n\n");
		mexPrintf("      Syntax:     [] = DAFF('close', handle)\n\n");
		mexPrintf("      Parameters: handle		1x1 int32	Handle of the opened DAFF file\n\n");
		mexPrintf("      Returns:    nothing\n\n\n");

		mexPrintf("  Command \"getMetadata\"\n\n");
		mexPrintf("      Returns the metadata of an opened DAFF file\n\n");
		mexPrintf("      Syntax:     [metadata] = DAFF('getMetadata', handle)\n\n");
		mexPrintf("      Parameters: handle		1x1 int32	Handle of the opened DAFF file\n\n");
		mexPrintf("      Returns:    metadata	struct		Metadata\n\n\n");
		
		mexPrintf("  Command \"getRecordMetadata\"\n\n");
		mexPrintf("      Returns the record metadata of an opened DAFF file\n\n");
		mexPrintf("      Syntax:     [metadata, empty] = DAFF('getRecordMetadata', handle, index)\n\n");
		mexPrintf("      Parameters: handle		1x1 int32	Handle of the opened DAFF file\n\n");
		mexPrintf("					 index		1x1 integer	Record index\n");
		mexPrintf("      Returns:    metadata	struct		Metadata\n\n\n");
		mexPrintf("					 empty		logical		Empty metadata flag\n\n\n");
		
		mexPrintf("  Command \"getProperties\"\n\n");
		mexPrintf("      Returns the properties of an opened DAFF file\n\n");
		mexPrintf("      Syntax:     [props] = DAFF('getProperties', handle)\n\n");
		mexPrintf("      Parameters: handle		1x1 int32	Handle of the opened DAFF file\n\n");
		mexPrintf("      Returns:    props		struct		Properties structure\n\n\n");

		mexPrintf("  Command \"getRecordCoords\"\n\n");
		mexPrintf("      Returns the coordinates of a grid point\n\n");
		mexPrintf("      Syntax:     [coords] = DAFF('getRecordCoords', handle, view, index)\n\n");
		mexPrintf("      Parameters: handle		1x1 int32	Handle of the opened DAFF file\n");
		mexPrintf("                  view		char		View ('data' => Data spherical coordinates, 'object' => Object spherical coordinates\n");
		mexPrintf("                  index		1x1 integer	Record index\n");
		mexPrintf("      Returns:    data		2x1 double	Column vector containing the two angles\n\n\n");

		mexPrintf("  Command \"getRecordByIndex\"\n\n");
		mexPrintf("      Returns the data at a grid point of the given index\n\n");
		mexPrintf("      Syntax:     [data] = DAFF('getRecordByIndex', handle, index)\n\n");
		mexPrintf("      Parameters: handle		1x1 int32	Handle of the opened DAFF file\n");
		mexPrintf("                  index		1x1 integer	Record index\n");
		mexPrintf("      Returns:    data		MxN double	Matrix containing the record data (rows => channel data)\n\n\n");

		mexPrintf("  Command \"getNearestNeighbourRecord\"\n\n");
		mexPrintf("      Returns the data at the nearest neighbour grid point to the given direction\n\n");
		mexPrintf("      Syntax:     [data, oob] = DAFF('getNearestNeighbourRecord', handle, view, angle1, angle2)\n\n");
		mexPrintf("      Parameters: handle		1x1 int32	Handle of the opened DAFF file\n");
		mexPrintf("                  view		char		View ('data' => Data spherical coordinates, 'object' => Object spherical coordinates\n");
		mexPrintf("                  angle1		1x1 real	First angle (For 'data' alpha; for 'object' azimuth)\n");
		mexPrintf("                  angle2		1x1 real	Second angle (For 'data' beta; for 'object' elevation)\n");
		mexPrintf("      Returns:    data		MxN double	Matrix containing the record data (rows => channel data)\n");
		mexPrintf("                  oob 		1x1 logical Out-of-bounds indicator\n");
		

		mexPrintf("  Command \"getCellRecords\"\n\n");
		mexPrintf("      Returns the data of all four records of the surrounding cell to the given direction\n\n");
		mexPrintf("      Syntax:     [data, oob] = DAFF('getCellRecords', handle, view, angle1, angle2)\n\n");
		mexPrintf("      Parameters: handle		1x1 int32	           Handle of the opened DAFF file\n");
		mexPrintf("                  view		char		           View ('data' => Data spherical coordinates, 'object' => Object spherical coordinates\n");
		mexPrintf("                  angle1		1x1 real	           First angle (For 'data' alpha; for 'object' azimuth)\n");
		mexPrintf("                  angle2		1x1 real	           Second angle (For 'data' beta; for 'object' elevation)\n");
		mexPrintf("      Returns:    data		cell(4) of MxN double  Matrix containing the record data (rows => channel data)\n");
		mexPrintf("                  oob 		1x1 logical            Out-of-bounds indicator\n");		
		
        mexPrintf("  Command \"getNearestNeighbourIndex\"\n\n");
        mexPrintf("      Returns the data at the nearest neighbour grid point to the given direction\n\n");
        mexPrintf("      Syntax:     [index, oob] = DAFF('getNearestNeighbour', handle, view, angle1, angle2)\n\n");
        mexPrintf("      Parameters: handle		1x1 int32	Handle of the opened DAFF file\n");
        mexPrintf("                  view		char		View ('data' => Data spherical coordinates, 'object' => Object spherical coordinates\n");
        mexPrintf("                  angle1		1x1 real	First angle (For 'data' alpha; for 'object' azimuth)\n");
        mexPrintf("                  angle2		1x1 real	Second angle (For 'data' beta; for 'object' elevation)\n");
        mexPrintf("      Returns:    index		1x1 integer	Nearest neighbour's record index\n");
        mexPrintf("                  oob		1x1 logical	Out of bounds flag\n\n\n");
		
        mexPrintf("  Command \"getCell\"\n\n");
        mexPrintf("      Returns the data at the nearest neighbour grid point to the given direction\n\n");
        mexPrintf("      Syntax:     [index1, index2, index3, index4] = DAFF('getCell', handle, view, angle1, angle2)\n\n");
        mexPrintf("      Parameters: handle		1x1 int32	Handle of the opened DAFF file\n");
        mexPrintf("                  view		char		View ('data' => Data spherical coordinates, 'object' => Object spherical coordinates\n");
        mexPrintf("                  angle1		1x1 real	First angle (For 'data' alpha; for 'object' azimuth)\n");
        mexPrintf("                  angle2		1x1 real	Second angle (For 'data' beta; for 'object' elevation)\n");
        mexPrintf("      Returns:    index1-4	1x1 integer	record indices of the cell points\n");

		mexPrintf("  Command \"getVersion\"\n\n");
		mexPrintf("      Returns the OpenDAFF version\n\n");
		mexPrintf("      Syntax:     [version] = DAFF('getVersion')\n\n");
		mexPrintf("      Parameters: None\n\n");
		mexPrintf("      Returns:    version     char	Version string\n\n\n");
	
		bMatch = true;
	}

	if( sCommand == "open" )
	{
		Open(nlhs, plhs, nrhs, prhs);
		bMatch = true;
	}

	if (sCommand == "close") {
		Close(nlhs, plhs, nrhs, prhs);
		bMatch = true;
	}

	if (sCommand == "getrecordmetadata") {
		GetRecordMetadata(nlhs, plhs, nrhs, prhs);
		bMatch = true;
	}

	if (sCommand == "getmetadata") {
		GetMetadata(nlhs, plhs, nrhs, prhs);
		bMatch = true;
	}

	if (sCommand == "getproperties") {
		GetProperties(nlhs, plhs, nrhs, prhs);
		bMatch = true;
	}

	if (sCommand == "getrecordcoords") {
		GetRecordCoords(nlhs, plhs, nrhs, prhs);
		bMatch = true;
	}

	if (sCommand == "getrecordbyindex") {
		GetRecordByIndex(nlhs, plhs, nrhs, prhs);
		bMatch = true;
	}

    if (sCommand == "getnearestneighbourrecord") {
            GetNearestNeighbourRecord(nlhs, plhs, nrhs, prhs);
            bMatch = true;
    }

	if (sCommand == "getcellrecords") {
		GetCellRecords(nlhs, plhs, nrhs, prhs);
		bMatch = true;
	}
	
    if (sCommand == "getnearestneighbourindex") {
            GetNearestNeighbourIndex(nlhs, plhs, nrhs, prhs);
            bMatch = true;
    }
	
    if (sCommand == "getcell") {
            GetCell(nlhs, plhs, nrhs, prhs);
            bMatch = true;
    }

	if (!bMatch)
		mexErrMsgTxt("Invalid command");
}


/* +------------------------------------------------------+
   |                                                      |
   |   Command implementations                            |
   |                                                      |
   +------------------------------------------------------+ */

void Open(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	int iError;
	std::string sError;

	if (nrhs != 2)
		mexErrMsgTxt("This command requires one argument");

	std::string sFilename;
	if (!getString(prhs[1], sFilename))
		mexErrMsgTxt("Parameter FILENAME must be a string");

	DAFFReader* pReader = DAFFReader::create();
	iError = pReader->openFile(sFilename);
	if (iError != DAFF_NO_ERROR) {
		sError = DAFFUtils::StrError(iError);
		delete pReader;
		mexPrintf("Errorcode %d\n", iError);
		mexErrMsgTxt(sError.c_str());
	}

	HANDLE hHandle = hHandleCount++;
	mHandles.insert(std::pair<int, TARGET*>(hHandle, new TARGET(pReader)));

	// Return the handle
	plhs[0] = mxCreateNumericMatrix(1, 1, HANDLE_CLASS_ID, mxREAL);
	*((HANDLE*) mxGetData(plhs[0])) = hHandle;
}

void Close(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	if (nrhs != 2)
		mexErrMsgTxt("This command requires one argument");

	HANDLE_MAP::iterator it = GetHandleIterator(prhs[1]);
	delete it->second;
	mHandles.erase(it);
}

void GetFilename(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	if (nrhs != 2)
		mexErrMsgTxt("This command requires one argument");

	DAFFReader* pReader = GetHandleTarget(prhs[1])->pReader;
	
	std::string sFilename = pReader->getFilename();

	// Return the string
	plhs[0] = mxCreateString(sFilename.c_str());
}

void GetFileFormatVersion(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	if (nrhs != 2)
		mexErrMsgTxt("This command requires one argument");

	DAFFReader* pReader = GetHandleTarget(prhs[1])->pReader;
	
	int iVersion = pReader->getFileFormatVersion();

	// Return as double
	plhs[0] = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
	*((int32_t*) mxGetData(plhs[0])) = iVersion;
}

void GetContentType(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	if (nrhs != 2)
		mexErrMsgTxt("This command requires one argument");

	DAFFReader* pReader = GetHandleTarget(prhs[1])->pReader;
	
	int iType = pReader->getContentType();
	std::string sType = DAFFUtils::StrShortContentType(iType);

	// Return the string
	plhs[0] = mxCreateString(sType.c_str());
}

// internal: convert DAFFMetadata to matlab struct matrix
void GetMetadataMatlab(const DAFFMetadata* pMetadata, mxArray *&plhs){
	// Get all the metadata keys
	std::vector<std::string> vsKeys;
	pMetadata->getKeys(vsKeys);

	// Create an unpopulated struct with the keynames
	int iNumFields = (int) vsKeys.size();
	const char** ppszFieldNames = new const char*[iNumFields];
	for (int i=0; i<iNumFields; i++) {
		// Return field names in lowercase
		std::transform(vsKeys[i].begin(), vsKeys[i].end(), vsKeys[i].begin(), tolower);
		ppszFieldNames[i] = vsKeys[i].c_str();
	}

	plhs = mxCreateStructMatrix(1, 1, iNumFields, ppszFieldNames);

	// Set the field values
	for (int i=0; i<iNumFields; i++) {
		mxArray* pValue;
		
		switch (pMetadata->getKeyType(vsKeys[i])) {
			case DAFFMetadata::DAFF_BOOL:
				pValue = mxCreateLogicalScalar( pMetadata->getKeyBool(vsKeys[i]) );
				break;

			case DAFFMetadata::DAFF_INT:
				pValue = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
				*((int32_t*) mxGetData(pValue)) = pMetadata->getKeyInt(vsKeys[i]);
				break;

			case DAFFMetadata::DAFF_FLOAT:
				pValue = mxCreateDoubleScalar( pMetadata->getKeyFloat(vsKeys[i]) );
				break;

			case DAFFMetadata::DAFF_STRING:
				pValue = mxCreateString( pMetadata->getKeyString(vsKeys[i]).c_str());
				break;
		}

		mxSetFieldByNumber(plhs, 0, i, pValue);
	}
}

void GetMetadata(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	if (nrhs != 2)
		mexErrMsgTxt("This command requires one argument");

	DAFFReader* pReader = GetHandleTarget(prhs[1])->pReader;
	const DAFFMetadata* pMetadata = pReader->getMetadata();
	
	GetMetadataMatlab(pMetadata, plhs[0]);
}

void GetRecordMetadata(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	if (nrhs != 3)
		mexErrMsgTxt("This command requires three arguments");

	bool bEmptyMetadata=false;
	int iRecordIndex=0;
	DAFFContent* pContent = GetHandleTarget(prhs[1])->pReader->getContent();
	const DAFFProperties* pProps = GetHandleTarget(prhs[1])->pReader->getProperties();
	
	if (!getIntegerScalar(prhs[2], iRecordIndex))
		mexErrMsgTxt("Invalid index");

	// Important: We use Matlab indexing here (starting with 1, 2, 3 ...)
	if ((iRecordIndex < 1) || (iRecordIndex > pProps->getNumberOfRecords()))
		mexErrMsgTxt("Invalid index");
	
	const DAFFMetadata* pMetadata = pContent->getRecordMetadata(iRecordIndex-1);

	GetMetadataMatlab(pMetadata, plhs[0]);

	if (nlhs == 2) {
		plhs[1] = mxCreateLogicalScalar( pMetadata->isEmpty() );
	}
}

void GetProperties(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	if (nrhs != 2)
		mexErrMsgTxt("This command requires one argument");

	DAFFReader* pReader = GetHandleTarget(prhs[1])->pReader;
	const DAFFProperties* pProps = pReader->getProperties();
	int iContentType = pReader->getContentType();

	// --= 1st compile a list of all field names =--------------

	std::vector<std::string> vsFields;

	// TODO: Decision how field names should look like ...

	// Global properties
	vsFields.push_back("filename");
	vsFields.push_back("fileFormatVersion");
	vsFields.push_back("contentType");
	vsFields.push_back("quantization");
	vsFields.push_back("numChannels");
	vsFields.push_back("numRecords");
	vsFields.push_back("channelLabels");
	vsFields.push_back("alphaPoints");
	vsFields.push_back("alphaResolution");
	vsFields.push_back("alphaRange");
	vsFields.push_back("betaPoints");
	vsFields.push_back("betaResolution");
	vsFields.push_back("betaRange");
	vsFields.push_back("orientation");
	vsFields.push_back("orientationDefault");
	vsFields.push_back("fullSphere");

	switch (iContentType) {
		case DAFF_IMPULSE_RESPONSE:
			vsFields.push_back("samplerate");
			vsFields.push_back("filterLength");
			break;
		case DAFF_MAGNITUDE_SPECTRUM:
		case DAFF_PHASE_SPECTRUM:
		case DAFF_MAGNITUDE_PHASE_SPECTRUM:
			vsFields.push_back("numFreqs");
			vsFields.push_back("freqs");
			break;
		case DAFF_DFT_SPECTRUM:
			vsFields.push_back("numDFTCoeffs");
			vsFields.push_back("TransformSize");
			break;
	}


	// --= 2nd create a Matlab struct of the fields =----------

	// TODO: Decision: Integral datatypes (e.g. num records) returned as double?!

	// Create an unpopulated struct with the keynames
	int iNumFields = (int) vsFields.size();
	const char** ppszFieldNames = new const char*[iNumFields];
	for (int i=0; i<iNumFields; i++) ppszFieldNames[i] = vsFields[i].c_str();

	mxArray* pStruct = mxCreateStructMatrix(1, 1, iNumFields, ppszFieldNames);
	mxArray* pValue;
	double* pdData;
	DAFFOrientationYPR orient;

	// Filename
	mxSetField(pStruct, 0, "filename", mxCreateString( pReader->getFilename().c_str() ));
	
	// File format version
	pValue = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
	*((int32_t*) mxGetData(pValue)) = pReader->getFileFormatVersion();
	mxSetField(pStruct, 0, "fileFormatVersion", pValue);

	// Content type
	std::string sContentType;
	sContentType = DAFFUtils::StrShortContentType( iContentType );
	mxSetField( pStruct, 0, "contentType", mxCreateString( sContentType.c_str() ) );

	// Quantization
	int iQuantization = pProps->getQuantization();
	std::string sQuantization;
	if( iQuantization == DAFF_INT16 ) sQuantization = "int16";
	if( iQuantization == DAFF_INT24 ) sQuantization = "int24";
	if( iQuantization == DAFF_FLOAT32 ) sQuantization = "float32";
	mxSetField(pStruct, 0, "quantization", mxCreateString( sQuantization.c_str() ));

	// Number of channels
	mxSetField( pStruct, 0, "numChannels", mxCreateDoubleScalar( pProps->getNumberOfChannels() ) );

	// Number of records
	mxSetField( pStruct, 0, "numRecords", mxCreateDoubleScalar( pProps->getNumberOfRecords() ) );

	// Channel labels (cell array of strings)
	int iChannels = pProps->getNumberOfChannels();
	pValue = mxCreateCellMatrix(1, iChannels);
	for (int c=0; c<iChannels; c++) {
		if (!pProps->getChannelLabel(c).empty())
			mxSetCell(pValue, c, mxCreateString( pProps->getChannelLabel(c).c_str() ));
	}
	mxSetField(pStruct, 0, "channelLabels", pValue);

	// Alpha points 
	mxSetField(pStruct, 0, "alphaPoints", mxCreateDoubleScalar( pProps->getAlphaPoints() ));

	// Alpha resolution
	mxSetField(pStruct, 0, "alphaResolution", mxCreateDoubleScalar( pProps->getAlphaResolution() ));

	// Alpha range
	pValue = mxCreateDoubleMatrix(1, 2, mxREAL);
	pdData = mxGetPr(pValue);
	pdData[0] = pProps->getAlphaStart();
	pdData[1] = pProps->getAlphaEnd();
	mxSetField(pStruct, 0, "alphaRange", pValue);

	// Beta points 
	mxSetField(pStruct, 0, "betaPoints", mxCreateDoubleScalar( pProps->getBetaPoints() ));

	// Beta resolution
	mxSetField(pStruct, 0, "betaResolution", mxCreateDoubleScalar( pProps->getBetaResolution() ));

	// Beta range
	pValue = mxCreateDoubleMatrix(1, 2, mxREAL);
	pdData = mxGetPr(pValue);
	pdData[0] = pProps->getBetaStart();
	pdData[1] = pProps->getBetaEnd();
	mxSetField(pStruct, 0, "betaRange", pValue);

	// Orientation
	pValue = mxCreateDoubleMatrix(1, 3, mxREAL);
	pdData = mxGetPr(pValue);
	pProps->getOrientation(orient);
	pdData[0] = orient.fYawAngleDeg;
	pdData[1] = orient.fPitchAngleDeg;
	pdData[2] = orient.fRollAngleDeg;
	mxSetField(pStruct, 0, "orientation", pValue);

	// Default orientation
	pValue = mxCreateDoubleMatrix(1, 3, mxREAL);
	pdData = mxGetPr(pValue);
	pProps->getDefaultOrientation(orient);
	pdData[0] = orient.fYawAngleDeg;
	pdData[1] = orient.fPitchAngleDeg;
	pdData[2] = orient.fRollAngleDeg;
	mxSetField(pStruct, 0, "orientationDefault", pValue);

	// Full sphere
	mxSetField(pStruct, 0, "fullSphere", mxCreateLogicalScalar( pProps->coversFullSphere() ));

	if (iContentType == DAFF_IMPULSE_RESPONSE) {
		DAFFContentIR* pContent = dynamic_cast<DAFFContentIR*>( pReader->getContent() );
		
		// Samplerate
		mxSetField(pStruct, 0, "samplerate", mxCreateDoubleScalar( pContent->getSamplerate() ));
		
		// Filter length
		mxSetField(pStruct, 0, "filterLength", mxCreateDoubleScalar( pContent->getFilterLength() ));
	}

	if (iContentType == DAFF_MAGNITUDE_SPECTRUM)
	{
		DAFFContentMS* pContent = dynamic_cast<DAFFContentMS*>( pReader->getContent() );

		// Number of frequencies
		int iNumFreqs = pContent->getNumFrequencies();
		mxSetField(pStruct, 0, "numFreqs", mxCreateDoubleScalar( iNumFreqs ));

		// Frequencies
		pValue = mxCreateDoubleMatrix(1, iNumFreqs, mxREAL);
		pdData = mxGetPr(pValue);
		const std::vector<float>& vfFreqs = pContent->getFrequencies();
		for (int i=0; i<iNumFreqs; i++) pdData[i] = (double) vfFreqs[i];
		mxSetField(pStruct, 0, "freqs", pValue);
	}

	if (iContentType == DAFF_PHASE_SPECTRUM)
	{
		DAFFContentPS* pContent = dynamic_cast<DAFFContentPS*>( pReader->getContent() );

		// Number of frequencies
		int iNumFreqs = pContent->getNumFrequencies();
		mxSetField(pStruct, 0, "numFreqs", mxCreateDoubleScalar( iNumFreqs ));

		// Frequencies
		pValue = mxCreateDoubleMatrix(1, iNumFreqs, mxREAL);
		pdData = mxGetPr(pValue);
		const std::vector<float>& vfFreqs = pContent->getFrequencies();
		for (int i=0; i<iNumFreqs; i++) pdData[i] = (double) vfFreqs[i];
		mxSetField(pStruct, 0, "freqs", pValue);
	}

	if (iContentType == DAFF_MAGNITUDE_PHASE_SPECTRUM)
	{
		DAFFContentMPS* pContent = dynamic_cast<DAFFContentMPS*>( pReader->getContent() );

		// Number of frequencies
		int iNumFreqs = pContent->getNumFrequencies();
		mxSetField(pStruct, 0, "numFreqs", mxCreateDoubleScalar( iNumFreqs ));

		// Frequencies
		pValue = mxCreateDoubleMatrix(1, iNumFreqs, mxREAL);
		pdData = mxGetPr(pValue);
		const std::vector<float>& vfFreqs = pContent->getFrequencies();
		for (int i=0; i<iNumFreqs; i++) pdData[i] = (double) vfFreqs[i];
		mxSetField(pStruct, 0, "freqs", pValue);
	}
	if (iContentType == DAFF_DFT_SPECTRUM) {
		DAFFContentDFT* pContent = dynamic_cast<DAFFContentDFT*>( pReader->getContent() );

		mxSetField(pStruct, 0, "transformSize", mxCreateDoubleScalar( pContent->getTransformSize() ));
		mxSetField(pStruct, 0, "numDFTCoeffs", mxCreateDoubleScalar( pContent->getNumDFTCoeffs() ));
		mxSetField(pStruct, 0, "isSymmetric", mxCreateLogicalScalar( pContent->isSymmetric() ));
		mxSetField(pStruct, 0, "samplerate", mxCreateDoubleScalar( pContent->getSamplerate() ));
		mxSetField(pStruct, 0, "frequencyBandwidth", mxCreateDoubleScalar( pContent->getFrequencyBandwidth() ));
	}

	plhs[0] = pStruct;
}

void GetRecordCoords(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
	if (nrhs != 4)
		mexErrMsgTxt("This command requires three arguments");

	DAFFReader* pReader = GetHandleTarget(prhs[1])->pReader;

	std::string sView;
	if (!getString(prhs[2], sView))
		mexErrMsgTxt("Parameter VIEW must be a string");
	std::transform(sView.begin(), sView.end(), sView.begin(), tolower);

	int iView = -1;
	if (sView == "data") iView = DAFF_DATA_VIEW;
	if (sView == "object") iView = DAFF_OBJECT_VIEW;
	if (iView == -1)
		mexErrMsgTxt("Invalid view");

	int iRecordIndex;
	if (!getIntegerScalar(prhs[3], iRecordIndex))
		mexErrMsgTxt("Invalid index");

	// Important: We use Matlab indexing here (starting with 1, 2, 3 ...)
	if ((iRecordIndex < 1) || (iRecordIndex > pReader->getProperties()->getNumberOfRecords()))
		mexErrMsgTxt("Invalid index");

	float fAngle1, fAngle2;
	pReader->getContent()->getRecordCoords(iRecordIndex-1, iView, fAngle1, fAngle2);

	// Return the result as a 2x1 vector
	plhs[0] = mxCreateDoubleMatrix(2, 1, mxREAL);
	double* pdData = mxGetPr(plhs[0]);
	pdData[0] = (double) fAngle1;
	pdData[1] = (double) fAngle2;
}

// Internal function: Fetch a record from the content,
// convert it to double and return it as a Matlab matrix
void GetRecordMatlab(DAFFReader* pReader, float* pConvBuffer, int iRecordIndex, mxArray*& pResult) {

	int iContentType = pReader->getContentType();
	int iChannels = pReader->getProperties()->getNumberOfChannels();

	if (iContentType == DAFF_IMPULSE_RESPONSE) {
		DAFFContentIR* pContent = dynamic_cast<DAFFContentIR*>( pReader->getContent() );
		int iFilterLength = pContent->getFilterLength();
	
		// Create the result matrix (num rows = num of channels, num cols = element size)
		pResult = mxCreateDoubleMatrix(iChannels, iFilterLength, mxREAL);
		double* pdData = mxGetPr(pResult);

		// Copy the data
		for (int c=0; c<iChannels; c++) {
			// Get the impulse response
			pContent->getFilterCoeffs(iRecordIndex, c, pConvBuffer);

			// Convert it to double precision (64-bit)
			for (int i=0; i<iFilterLength; i++)
				pdData[i*iChannels+c] = (double) pConvBuffer[i];
		}
	}

	if (iContentType == DAFF_MAGNITUDE_SPECTRUM) 
	{
		DAFFContentMS* pContent = dynamic_cast<DAFFContentMS*>( pReader->getContent() );
		int iNumFreqs = pContent->getNumFrequencies();
	
		// Create the result matrix (num rows = num of channels, num cols = num freqs)
		pResult = mxCreateDoubleMatrix(iChannels, iNumFreqs, mxREAL);
		double* pdData = mxGetPr(pResult);

		// Copy the data
		for (int c=0; c<iChannels; c++) {
			// Get the magnitude spectrum
			pContent->getMagnitudes(iRecordIndex, c, pConvBuffer);

			// Convert it to double precision (64-bit)
			for (int i=0; i<iNumFreqs; i++)
				pdData[i*iChannels+c] = (double) pConvBuffer[i];
		}
	}
	
	if (iContentType == DAFF_PHASE_SPECTRUM) 
	{
		DAFFContentPS* pContent = dynamic_cast<DAFFContentPS*>( pReader->getContent() );
		int iNumFreqs = pContent->getNumFrequencies();
	
		// Create the result matrix (num rows = num of channels, num cols = num freqs)
		pResult = mxCreateDoubleMatrix(iChannels, iNumFreqs, mxREAL);
		double* pdData = mxGetPr(pResult);

		// Copy the data
		for (int c=0; c<iChannels; c++) {
			// Get the magnitude spectrum
			pContent->getPhases(iRecordIndex, c, pConvBuffer);

			// Convert it to double precision (64-bit)
			for (int i=0; i<iNumFreqs; i++)
				pdData[i*iChannels+c] = (double) pConvBuffer[i];
		}
	}

	if (iContentType == DAFF_MAGNITUDE_PHASE_SPECTRUM) {
		DAFFContentMPS* pContent = dynamic_cast<DAFFContentMPS*>( pReader->getContent() );
		int iNumFreqs = pContent->getNumFrequencies();
	
		// Create the result matrix (num rows = num of channels, num cols = the element size)
		pResult = mxCreateDoubleMatrix(iChannels, iNumFreqs, mxCOMPLEX);
		double* pdRealData = mxGetPr(pResult);
		double* pdImagData = mxGetPi(pResult);

		// Copy the data
		for (int c=0; c<iChannels; c++) {
			// Get the impulse response
			pContent->getCoefficientsRI(iRecordIndex, c, pConvBuffer);

			// Convert it to double precision (64-bit) complex values
			for (int i=0; i<iNumFreqs; i++) {
				pdRealData[i*iChannels+c] = (double) pConvBuffer[2*i];
				pdImagData[i*iChannels+c] = (double) pConvBuffer[2*i+1];
			}
		}
	}

	if (iContentType == DAFF_DFT_SPECTRUM) {
		DAFFContentDFT* pContent = dynamic_cast<DAFFContentDFT*>( pReader->getContent() );
		int iNumDFTCoeffs = pContent->getNumDFTCoeffs();
	
		// Create the result matrix (num rows = num of channels, num cols = the element size)
		pResult = mxCreateDoubleMatrix(iChannels, iNumDFTCoeffs, mxCOMPLEX);
		double* pdRealData = mxGetPr(pResult);
		double* pdImagData = mxGetPi(pResult);

		// Copy the data
		for (int c=0; c<iChannels; c++) {
			// Get the impulse response
			pContent->getDFTCoeffs(iRecordIndex, c, pConvBuffer);

			// Convert it to double precision (64-bit) complex values
			for (int i=0; i<iNumDFTCoeffs; i++) {
				pdRealData[i*iChannels+c] = (double) pConvBuffer[2*i];
				pdImagData[i*iChannels+c] = (double) pConvBuffer[2*i+1];
			}
		}
	}
}

void GetRecordByIndex(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
	if (nrhs != 3)
		mexErrMsgTxt("This command requires two arguments");

	TARGET* pTarget = GetHandleTarget(prhs[1]);
	DAFFReader* pReader = pTarget->pReader;

	int iRecordIndex;
	if (!getIntegerScalar(prhs[2], iRecordIndex))
		mexErrMsgTxt("Invalid index");

	// Important: We use Matlab indexing here (starting with 1, 2, 3 ...)
	if ((iRecordIndex < 1) || (iRecordIndex > pReader->getProperties()->getNumberOfRecords()))
		mexErrMsgTxt("Invalid index");

	GetRecordMatlab(pReader, pTarget->pfBuffer, iRecordIndex-1, plhs[0]);
}

void GetNearestNeighbourRecord(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	if (nrhs != 5)
		mexErrMsgTxt("This command requires four arguments");

	TARGET* pTarget = GetHandleTarget(prhs[1]);
	DAFFReader* pReader = pTarget->pReader;

	std::string sView;
	if (!getString(prhs[2], sView))
		mexErrMsgTxt("Parameter VIEW must be a string");
	std::transform(sView.begin(), sView.end(), sView.begin(), tolower);

	int iView = -1;
	if (sView == "data") iView = DAFF_DATA_VIEW;
	if (sView == "object") iView = DAFF_OBJECT_VIEW;
	if (iView == -1)
		mexErrMsgTxt("Invalid view");

	double dAngle1, dAngle2;
	if (!getRealScalar(prhs[3], dAngle1))
		mexErrMsgTxt("Parameter ANGLE1 must be a real-valued scalar");
	if (!getRealScalar(prhs[4], dAngle2))
		mexErrMsgTxt("Parameter ANGLE2 must be a real-valued scalar");

	int iRecordIndex;
	bool bOutOfBounds;
	pReader->getContent()->getNearestNeighbour(iView, (float)dAngle1, (float)dAngle2, iRecordIndex, bOutOfBounds);

	GetRecordMatlab(pReader, pTarget->pfBuffer, iRecordIndex, plhs[0]);
	
	if (nlhs == 2){
		plhs[1] = mxCreateLogicalScalar(bOutOfBounds);
	}
}

void GetCellRecords(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	if (nrhs != 5)
		mexErrMsgTxt("This command requires four arguments");

	TARGET* pTarget = GetHandleTarget(prhs[1]);
	DAFFReader* pReader = pTarget->pReader;

	std::string sView;
	if (!getString(prhs[2], sView))
		mexErrMsgTxt("Parameter VIEW must be a string");
	std::transform(sView.begin(), sView.end(), sView.begin(), tolower);

	int iView = -1;
	if (sView == "data") iView = DAFF_DATA_VIEW;
	if (sView == "object") iView = DAFF_OBJECT_VIEW;
	if (iView == -1)
		mexErrMsgTxt("Invalid view");

	double dAngle1, dAngle2;
	if (!getRealScalar(prhs[3], dAngle1))
		mexErrMsgTxt("Parameter ANGLE1 must be a real-valued scalar");
	if (!getRealScalar(prhs[4], dAngle2))
		mexErrMsgTxt("Parameter ANGLE2 must be a real-valued scalar");

	DAFFQuad q;
	pReader->getContent()->getCell(iView, (float) dAngle1, (float) dAngle2, q);
	
	mxArray* pResult = mxCreateCellMatrix(1, 4);
	mxArray* pRecord;
	
	GetRecordMatlab(pReader, pTarget->pfBuffer, q.iIndex1, pRecord);
	mxSetCell(pResult, 0, pRecord);
	GetRecordMatlab(pReader, pTarget->pfBuffer, q.iIndex2, pRecord);
	mxSetCell(pResult, 1, pRecord);
	GetRecordMatlab(pReader, pTarget->pfBuffer, q.iIndex3, pRecord);
	mxSetCell(pResult, 2, pRecord);
	GetRecordMatlab(pReader, pTarget->pfBuffer, q.iIndex4, pRecord);
	mxSetCell(pResult, 3, pRecord);

	plhs[0] = pResult;
}

void GetNearestNeighbourIndex(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

        if (nrhs != 5)
                mexErrMsgTxt("This command requires four arguments");

        TARGET* pTarget = GetHandleTarget(prhs[1]);
        DAFFReader* pReader = pTarget->pReader;

        std::string sView;
        if (!getString(prhs[2], sView))
                mexErrMsgTxt("Parameter VIEW must be a string");
        std::transform(sView.begin(), sView.end(), sView.begin(), tolower);

        int iView = -1;
        if (sView == "data") iView = DAFF_DATA_VIEW;
        if (sView == "object") iView = DAFF_OBJECT_VIEW;
        if (iView == -1)
                mexErrMsgTxt("Invalid view");

        double dAngle1, dAngle2;
        if (!getRealScalar(prhs[3], dAngle1))
                mexErrMsgTxt("Parameter ANGLE1 must be a real-valued scalar");
        if (!getRealScalar(prhs[4], dAngle2))
                mexErrMsgTxt("Parameter ANGLE2 must be a real-valued scalar");

        int iRecordIndex;
        bool bOutOfBounds;
        pReader->getContent()->getNearestNeighbour(iView, (float)dAngle1, (float)dAngle2, iRecordIndex, bOutOfBounds);

        plhs[0] = mxCreateDoubleScalar(iRecordIndex+1); //cave: matlab indexing
        if (nlhs == 2){
                plhs[1] = mxCreateLogicalScalar(bOutOfBounds);
        }
}

void GetCell(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

        if (nrhs != 5)
                mexErrMsgTxt("This command requires four arguments");

        TARGET* pTarget = GetHandleTarget(prhs[1]);
        DAFFReader* pReader = pTarget->pReader;

        std::string sView;
        if (!getString(prhs[2], sView))
                mexErrMsgTxt("Parameter VIEW must be a string");
        std::transform(sView.begin(), sView.end(), sView.begin(), tolower);

        int iView = -1;
        if (sView == "data") iView = DAFF_DATA_VIEW;
        if (sView == "object") iView = DAFF_OBJECT_VIEW;
        if (iView == -1)
                mexErrMsgTxt("Invalid view");

        double dAngle1, dAngle2;
        if (!getRealScalar(prhs[3], dAngle1))
                mexErrMsgTxt("Parameter ANGLE1 must be a real-valued scalar");
        if (!getRealScalar(prhs[4], dAngle2))
                mexErrMsgTxt("Parameter ANGLE2 must be a real-valued scalar");

        DAFFQuad qIndices;
        pReader->getContent()->getCell(iView, (float)dAngle1, (float)dAngle2, qIndices);

		// +1 because of Matlab indexing
		plhs[0] = mxCreateDoubleScalar(qIndices.iIndex1+1);
		plhs[1] = mxCreateDoubleScalar(qIndices.iIndex2+1);
		plhs[2] = mxCreateDoubleScalar(qIndices.iIndex3+1);
		plhs[3] = mxCreateDoubleScalar(qIndices.iIndex4+1);
}
