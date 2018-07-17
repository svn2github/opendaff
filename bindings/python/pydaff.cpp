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
 *  Copyright 2016-2018 Institute of Technical Acoustics (ITA), RWTH Aachen University
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

 /*
  * Documentation this Python module was build upon can be found here:
  * https://docs.python.org/2/extending/extending.html
  */

#include <Python.h>
#include <DAFF.h>

#include <string>
#include <map>

#include "pydaffdoc.hpp"

static std::map< int, DAFFReader* > g_mReader; //!< Global reader instance map
static int g_iLastHandle = 0; //!< Stores ID of last used handle identifier
static PyObject* g_pDAFFError = nullptr; //!< Static pointer to error instance


void GetRecordPython(DAFFReader* pReader, int iRecordIndex, PyObject* &listResult);

bool ValidHandle( const int iHandle )
{
	if( g_mReader.find( iHandle ) == g_mReader.end() )
		return false;
	else
		return true;
}

static PyObject* daff_open( PyObject*, PyObject** ppArgs, Py_ssize_t nArgs, PyObject* pKeywordNames )
{
	static const char * const _keywords[] = { "open", NULL };
	static _PyArg_Parser _parser = { "s:open", _keywords, 0 };
	char* pcFilePath = nullptr;

	if( !_PyArg_ParseStack( ppArgs, nArgs, pKeywordNames, &_parser, &pcFilePath ) )
		return NULL;

	std::string sFilePath( pcFilePath );
	DAFFReader* pReader = DAFFReader::create();

	if( pReader->openFile( sFilePath ) == DAFF_NO_ERROR )
	{
		int iNewHandle = ++g_iLastHandle;
		g_mReader[ iNewHandle ] = pReader;
		return PyLong_FromLong( iNewHandle );
	}

	delete pReader;

	PyErr_SetString( PyExc_ConnectionError, std::string( "Could not open " + sFilePath ).c_str() );
	return NULL;
}

static PyObject* daff_close( PyObject*, PyObject** ppArgs, Py_ssize_t nArgs, PyObject* pKeywordNames )
{
	static const char * const _keywords[] = { "close", NULL };
	static _PyArg_Parser _parser = { "i:close", _keywords, 0 };
	int iHandle = -1;

	if( !_PyArg_ParseStack( ppArgs, nArgs, pKeywordNames, &_parser, &iHandle ) )
		return NULL;

	if( !ValidHandle( iHandle ) )
	{
		PyErr_SetString( PyExc_ConnectionError, std::string( "Invalid DAFF handle" ).c_str() );
		return NULL;
	}
	else
	{
		DAFFReader* pReader = g_mReader[ iHandle ];
		g_mReader.erase( iHandle );
		pReader->closeFile();
		return Py_None;
	}
}

static PyObject* daff_nearest_neighbour_index( PyObject*, PyObject** ppArgs, Py_ssize_t nArgs, PyObject* pKeywordNames )
{
	
	// Get the index of the nearest neighbour
	static const char * const _keywords[] = { "index", "view", "angle1", "angle2", NULL };
	static _PyArg_Parser _parser = { "iidd:nearest_neighbour_index", _keywords, 0 };
	int iHandle = -1;
	int iView;
	double dAngle1Deg;
	double dAngle2Deg;
	

	if( !_PyArg_ParseStack( ppArgs, nArgs, pKeywordNames, &_parser, &iHandle, &iView, &dAngle1Deg, &dAngle2Deg) )
		return NULL;

	if( !ValidHandle( iHandle ) )
	{
		PyErr_SetString( PyExc_ConnectionError, std::string( "Invalid DAFF handle" ).c_str() );
		return NULL;
	}
	else
	{
		DAFFReader* pReader = g_mReader[ iHandle ];
		int iRecordIndex;
		bool bOutOfBounds;

		pReader->getContent()->getNearestNeighbour(iView, dAngle1Deg, dAngle2Deg, iRecordIndex, bOutOfBounds );

		//Python list which contains the Record index and the OutOfBounds Indicator
		PyObject* pyNearestNeighbour = PyList_New(0);
		PyList_Append(pyNearestNeighbour, PyLong_FromLong(iRecordIndex));
		PyList_Append(pyNearestNeighbour, PyBool_FromLong(bOutOfBounds));


		//Return Python list
		return pyNearestNeighbour;
	}
	
	return Py_None;
}

static PyObject* daff_nearest_neighbour_record( PyObject*, PyObject** ppArgs, Py_ssize_t nArgs, PyObject* pKeywordNames )
{
	// Get the record of the nearest neighbour
	static const char * const _keywords[] = { "index", "view", "angle1", "angle2", NULL };
	static _PyArg_Parser _parser = { "iidd:nearest_neighbour_record", _keywords, 0 };
	int iHandle = -1;
	int iView;
	double dAngle1Deg;
	double dAngle2Deg;

	// Initialize a new list
	PyObject* record = PyList_New(0);

	if (!_PyArg_ParseStack(ppArgs, nArgs, pKeywordNames, &_parser, &iHandle, &iView, &dAngle1Deg, &dAngle2Deg))
		return NULL;

	if (!ValidHandle(iHandle))
	{
		PyErr_SetString(PyExc_ConnectionError, std::string("Invalid DAFF handle").c_str());
		return NULL;
	}
	else
	{
		DAFFReader* pReader = g_mReader[iHandle];
		int iRecordIndex;
		pReader->getContent()->getNearestNeighbour(iView, dAngle1Deg, dAngle2Deg, iRecordIndex);

		//Get the record and set the record list
		GetRecordPython(pReader, iRecordIndex, record);

		//Return the record of the nearest neighbour
		return record;

	}
	
	return record;
}

static PyObject* daff_record(PyObject*, PyObject** ppArgs, Py_ssize_t nArgs, PyObject* pKeywordNames)
{
	// Get the record of the given record index
	static const char * const _keywords[] = { "index", "recordIndex", NULL };
	static _PyArg_Parser _parser = { "ii:record", _keywords, 0 };
	int iHandle = -1;
	int iRecordIndex;


	// Initialize a new list
	PyObject* record = PyList_New(0);

	if (!_PyArg_ParseStack(ppArgs, nArgs, pKeywordNames, &_parser, &iHandle, &iRecordIndex))
		return NULL;

	if (!ValidHandle(iHandle))
	{
		PyErr_SetString(PyExc_ConnectionError, std::string("Invalid DAFF handle").c_str());
		return NULL;
	}
	else
	{
		DAFFReader* pReader = g_mReader[iHandle];

		//Get the record and set the record list
		GetRecordPython(pReader, iRecordIndex, record);

		//Return the record of the nearest neighbour
		return record;

	}

	return record;
}

// Fetch a record from the content, convert and return it as a python list
static void GetRecordPython(DAFFReader* pReader, int iRecordIndex, PyObject* &listResult)
{
	int iContentType = pReader->getContentType();
	int iChannels = pReader->getProperties()->getNumberOfChannels();

	// Create the result 
	listResult = PyList_New(0);
	
	if (iContentType == DAFF_IMPULSE_RESPONSE)
	{
		DAFFContentIR* pContent = dynamic_cast< DAFFContentIR* >(pReader->getContent());
		int iFilterLength = pContent->getFilterLength();


		// Copy the data
		for (int c = 0; c < iChannels; c++)
		{
			//Create python list for current channel
			PyObject* listChannel = PyList_New(0);

			// Get the impulse response
			float* fImpulseResponse = NULL;;
			fImpulseResponse = new float[iFilterLength];
			pContent->getFilterCoeffs(iRecordIndex, c, fImpulseResponse);

			// Append it to current channel list
			for (int i = 0; i < iFilterLength; i++)
				PyList_Append(listChannel, PyFloat_FromDouble(fImpulseResponse[i]));

			// Freeing memory
			delete[] fImpulseResponse;
			fImpulseResponse = NULL;

			// Append channel List to result List
			PyList_Append(listResult, listChannel);

		}
	}

	if (iContentType == DAFF_MAGNITUDE_SPECTRUM)
	{
		DAFFContentMS* pContent = dynamic_cast< DAFFContentMS* >(pReader->getContent());
		int iNumFreqs = pContent->getNumFrequencies();


		// Copy the data
		for (int c = 0; c < iChannels; c++) 
		{
			// Create python list for current channel
			PyObject* listChannel = PyList_New(0);

			// Get the magnitude spectrum
			float* fMagnitudes = NULL;
			fMagnitudes = new float[iNumFreqs];
			pContent->getMagnitudes(iRecordIndex, c, fMagnitudes);

			// Append it to current channel list
			for (int i = 0; i < iNumFreqs; i++)
				PyList_Append(listChannel, PyFloat_FromDouble(fMagnitudes[i]));

			// Freeing memory
			delete[] fMagnitudes;
			fMagnitudes = NULL;

			// Append channel List to result List
			PyList_Append(listResult, listChannel);
		}
	}

	if (iContentType == DAFF_PHASE_SPECTRUM)
	{
		DAFFContentPS* pContent = dynamic_cast< DAFFContentPS* >(pReader->getContent());
		int iNumFreqs = pContent->getNumFrequencies();


		// Copy the data
		for (int c = 0; c < iChannels; c++)
		{
			//Create python list for current channel
			PyObject* listChannel = PyList_New(0);

			// Get the phase coefficients
			float* fPhases = NULL;
			fPhases = new float[iNumFreqs];
			pContent->getPhases(iRecordIndex, c, fPhases);

			// Append it to current channel list
			for (int i = 0; i < iNumFreqs; i++)
				PyList_Append(listChannel, PyFloat_FromDouble(fPhases[i]));

			// Freeing memory
			delete[] fPhases;
			fPhases = NULL;

			// Append channel List to result List
			PyList_Append(listResult, listChannel);
		}
	}

	if (iContentType == DAFF_MAGNITUDE_PHASE_SPECTRUM)
	{
		DAFFContentMPS* pContent = dynamic_cast< DAFFContentMPS* >(pReader->getContent());
		int iNumFreqs = pContent->getNumFrequencies();


		// Copy the data
		for (int c = 0; c < iChannels; c++)
		{
			//Create python list for current channel
			PyObject* listChannel = PyList_New(0);

			// Get the impulse response
			float* fCoeffs = NULL;
			fCoeffs = new float[iNumFreqs];
			pContent->getCoefficientsRI(iRecordIndex, c, fCoeffs);

			// Append it to current channel list. Even indexes are real and odd indexes are imaginary.
			for (int i = 0; i < iNumFreqs; i++) 
			{
				PyList_Append(listChannel, PyComplex_FromDoubles(fCoeffs[2*i], fCoeffs[2*i+1]));
			}

			// Freeing memory
			delete[] fCoeffs;
			fCoeffs = NULL;

			// Append channel List to result List
			PyList_Append(listResult, listChannel);
		}
	}

	if (iContentType == DAFF_DFT_SPECTRUM)
	{
		DAFFContentDFT* pContent = dynamic_cast< DAFFContentDFT* >(pReader->getContent());
		int iNumDFTCoeffs = pContent->getNumDFTCoeffs();


		// Copy the data
		for (int c = 0; c < iChannels; c++)
		{
			//Create python list for current channel
			PyObject* listChannel = PyList_New(0);

			// Get the impulse response
			float* fDFTCoeffs = NULL;
			fDFTCoeffs = new float[iNumDFTCoeffs];
			pContent->getDFTCoeffs(iRecordIndex, c, fDFTCoeffs);

			// Append it to current channel list. Even indexes are real and odd indexes are imaginary.
			for (int i = 0; i < iNumDFTCoeffs; i++)
			{
				PyList_Append(listChannel, PyComplex_FromDoubles(fDFTCoeffs[2 * i], fDFTCoeffs[2 * i + 1]));
			}

			// Freeing memory
			delete[] fDFTCoeffs;
			fDFTCoeffs = NULL;

			// Append channel List to result List
			PyList_Append(listResult, listChannel);
		}
	}
}


static PyObject* daff_content_type( PyObject*, PyObject** ppArgs, Py_ssize_t nArgs, PyObject* pKeywordNames )
{
	static const char * const _keywords[] = { "index", NULL };
	static _PyArg_Parser _parser = { "i:content_type", _keywords, 0 };
	int iHandle = -1;

	if( !_PyArg_ParseStack( ppArgs, nArgs, pKeywordNames, &_parser, &iHandle ) )
		return NULL;

	if( !ValidHandle( iHandle ) )
	{
		PyErr_SetString( PyExc_ConnectionError, std::string( "Invalid DAFF handle" ).c_str() );
		return NULL;
	}
	else
	{
		DAFFReader* pReader = g_mReader[ iHandle ];
		int iContentType = pReader->getContentType(); 
		return PyLong_FromLong( iContentType );
	}
}

static PyObject* daff_content_type_str(PyObject*, PyObject** ppArgs, Py_ssize_t nArgs, PyObject* pKeywordNames)
{
	static const char * const _keywords[] = { "index", NULL };
	static _PyArg_Parser _parser = { "i:content_type_str", _keywords, 0 };
	int iHandle = -1;

	if (!_PyArg_ParseStack(ppArgs, nArgs, pKeywordNames, &_parser, &iHandle))
		return NULL;

	if (!ValidHandle(iHandle))
	{
		PyErr_SetString(PyExc_ConnectionError, std::string("Invalid DAFF handle").c_str());
		return NULL;
	}
	else
	{
		DAFFReader* pReader = g_mReader[iHandle];
		std::string sContentType = DAFFUtils::StrContentType(pReader->getContentType());
		return PyUnicode_FromString(sContentType.c_str());
	}
}

static PyObject* daff_metadata(PyObject*, PyObject** ppArgs, Py_ssize_t nArgs, PyObject* pKeywordNames)
{
	static const char * const _keywords[] = { "index", NULL };
	static _PyArg_Parser _parser = { "i:content_type_str", _keywords, 0 };
	int iHandle = -1;

	if (!_PyArg_ParseStack(ppArgs, nArgs, pKeywordNames, &_parser, &iHandle))
	{
		PyErr_SetString(PyExc_KeyError, std::string("Wrong number of arguments").c_str());
		return NULL;
	}

	if (!ValidHandle(iHandle))
	{
		PyErr_SetString(PyExc_ConnectionError, std::string("Invalid DAFF handle").c_str());
		return NULL;
	}
	else
	{

		DAFFReader* pReader = g_mReader[iHandle];
		int iContentType = pReader->getContentType();

		//Create a new python dictionary
		PyObject* metadata = PyDict_New();

		//Get the metadata
		const DAFFMetadata* pMetadata = pReader->getMetadata();

		// Get all the metadata keys
		std::vector< std::string > vsKeys;
		pMetadata->getKeys(vsKeys);


		for each (std::string sKey in vsKeys)
		{
			//Current key as python object
			PyObject* pyKey = PyUnicode_FromString(sKey.c_str());

			//Check the value type and set the value to the python dictionary at the current key
			switch (pMetadata->getKeyType(sKey))
			{
			case DAFFMetadata::DAFF_BOOL:
				//Set the boolean value
				PyDict_SetItem(metadata, pyKey, PyBool_FromLong(pMetadata->getKeyBool(sKey)));
				break;

			case DAFFMetadata::DAFF_INT:
				//Set the integer value
				PyDict_SetItem(metadata, pyKey, PyLong_FromLong(pMetadata->getKeyInt(sKey)));
				break;

			case DAFFMetadata::DAFF_FLOAT:
				//Set the floating point value
				PyDict_SetItem(metadata, pyKey, PyFloat_FromDouble(pMetadata->getKeyFloat(sKey)));
				break;

			case DAFFMetadata::DAFF_STRING:
				//Set the string value
				PyDict_SetItem(metadata, pyKey, PyUnicode_FromString(pMetadata->getKeyString(sKey).c_str()));
				break;
			}
			
		}
		
		return metadata;
	}

	
	PyErr_SetString(PyExc_ConnectionError, std::string("Something went wrong.").c_str());
	return Py_None;
}

static PyObject* daff_properties(PyObject*, PyObject** ppArgs, Py_ssize_t nArgs, PyObject* pKeywordNames)
{
	static const char * const _keywords[] = { "index", NULL };
	static _PyArg_Parser _parser = { "i:content_type_str", _keywords, 0 };
	int iHandle = -1;

	if (!_PyArg_ParseStack(ppArgs, nArgs, pKeywordNames, &_parser, &iHandle))
	{
		PyErr_SetString(PyExc_KeyError, std::string("Wrong number of arguments").c_str());
		return NULL;
	}

	if (!ValidHandle(iHandle))
	{
		PyErr_SetString(PyExc_ConnectionError, std::string("Invalid DAFF handle").c_str());
		return NULL;
	}
	else
	{

		//Create a new python dictionary
		PyObject* pyProperties = PyDict_New();


		DAFFReader* pReader = g_mReader[iHandle];
		const DAFFProperties* pProps = pReader->getProperties();

		// --= Create a Python dictionary of the property fields =----------

		// Filename
		PyDict_SetItem(pyProperties, PyUnicode_FromString("Filename"), PyUnicode_FromString(pReader->getFilename().c_str()));

		// File format version
		PyDict_SetItem(pyProperties, PyUnicode_FromString("FileFormatVersion"), PyLong_FromLong(pReader->getFileFormatVersion()));

		// Content type
		int iContentType = pReader->getContentType();
		std::string sContentType = DAFFUtils::StrShortContentType(iContentType);
		PyDict_SetItem(pyProperties, PyUnicode_FromString("ContentType"), PyUnicode_FromString(sContentType.c_str()));

		// Quantization
		int iQuantization = pProps->getQuantization();
		std::string sQuantization;
		if (iQuantization == DAFF_INT16)
			sQuantization = "int16";
		if (iQuantization == DAFF_INT24)
			sQuantization = "int24";
		if (iQuantization == DAFF_FLOAT32)
			sQuantization = "float32";
		PyDict_SetItem(pyProperties, PyUnicode_FromString("Quantization"), PyUnicode_FromString(sQuantization.c_str()));

		// Number of channels
		int iChannels = pProps->getNumberOfChannels();
		PyDict_SetItem(pyProperties, PyUnicode_FromString("NumChannels"), PyLong_FromLong(iChannels));

		// Number of records
		PyDict_SetItem(pyProperties, PyUnicode_FromString("NumRecords"), PyLong_FromLong(pProps->getNumberOfRecords()));

		// Channel labels (python list of strings)
		PyObject* pyChannelLabels = PyList_New(0);
		for (int c = 0; c < iChannels; c++)
		{
			if (!pProps->getChannelLabel(c).empty())
				PyList_Append(pyChannelLabels, PyUnicode_FromString(pProps->getChannelLabel(c).c_str()));
		}
		PyDict_SetItem(pyProperties, PyUnicode_FromString("ChannelLabels"), pyChannelLabels);


		// Alpha points 
		PyDict_SetItem(pyProperties, PyUnicode_FromString("AlphaPoints"), PyLong_FromLong(pProps->getAlphaPoints()));

		// Alpha resolution
		PyDict_SetItem(pyProperties, PyUnicode_FromString("AlphaResolution"), PyFloat_FromDouble(pProps->getAlphaResolution()));

		// Alpha range
		PyObject* pyAlphaRange = PyList_New(0);
		float alphaStart = pProps->getAlphaStart();
		float alphaEnd = pProps->getAlphaEnd();
		PyList_Append(pyAlphaRange, PyFloat_FromDouble(alphaStart));
		PyList_Append(pyAlphaRange, PyFloat_FromDouble(alphaEnd));
		PyDict_SetItem(pyProperties, PyUnicode_FromString("AlphaRange"), pyAlphaRange);

		// Beta points 
		PyDict_SetItem(pyProperties, PyUnicode_FromString("BetaPoints"), PyLong_FromLong(pProps->getBetaPoints()));

		// Beta resolution
		PyDict_SetItem(pyProperties, PyUnicode_FromString("BetaResolution"), PyFloat_FromDouble(pProps->getBetaResolution()));

		// Beta range
		PyObject* pyBetaRange = PyList_New(0);
		float betaStart = pProps->getBetaStart();
		float betaEnd = pProps->getBetaEnd();
		PyList_Append(pyBetaRange, PyFloat_FromDouble(betaStart));
		PyList_Append(pyBetaRange, PyFloat_FromDouble(betaEnd));
		PyDict_SetItem(pyProperties, PyUnicode_FromString("BetaRange"), pyBetaRange);

		// Orientation
		DAFFOrientationYPR orient;
		PyObject* pyOrientation = PyDict_New();
		pProps->getOrientation(orient);
		float fYawAngle = orient.fYawAngleDeg;
		float fPitchAngle = orient.fPitchAngleDeg;
		float fRollAngle = orient.fRollAngleDeg;
		PyDict_SetItem(pyOrientation, PyUnicode_FromString("YawAngle"), PyFloat_FromDouble(fYawAngle));
		PyDict_SetItem(pyOrientation, PyUnicode_FromString("PitchAngle"), PyFloat_FromDouble(fPitchAngle));
		PyDict_SetItem(pyOrientation, PyUnicode_FromString("RollAngle"), PyFloat_FromDouble(fRollAngle));
		PyDict_SetItem(pyProperties, PyUnicode_FromString("Orientation"), pyOrientation);

		// Default orientation
		PyObject* pyDefOrientation = PyDict_New();

		pProps->getDefaultOrientation(orient);
		fYawAngle = orient.fYawAngleDeg;
		fPitchAngle = orient.fPitchAngleDeg;
		fRollAngle = orient.fRollAngleDeg;
		PyDict_SetItem(pyDefOrientation, PyUnicode_FromString("YawAngle"), PyFloat_FromDouble(fYawAngle));
		PyDict_SetItem(pyDefOrientation, PyUnicode_FromString("PitchAngle"), PyFloat_FromDouble(fPitchAngle));
		PyDict_SetItem(pyDefOrientation, PyUnicode_FromString("RollAngle"), PyFloat_FromDouble(fRollAngle));
		PyDict_SetItem(pyProperties, PyUnicode_FromString("OrientationDefault"), pyDefOrientation);


		// Full sphere
		PyDict_SetItem(pyProperties, PyUnicode_FromString("FullSphere"), PyBool_FromLong(pProps->coversFullSphere()));
		
		if (iContentType == DAFF_IMPULSE_RESPONSE)
		{
			DAFFContentIR* pContent = dynamic_cast< DAFFContentIR* >(pReader->getContent());

			// Samplerate
			PyDict_SetItem(pyProperties, PyUnicode_FromString("Samplerate"), PyFloat_FromDouble(pContent->getSamplerate()));

			// Filter length
			PyDict_SetItem(pyProperties, PyUnicode_FromString("FilterLength"), PyLong_FromLong(pContent->getFilterLength()));
		}
		else if (iContentType == DAFF_MAGNITUDE_SPECTRUM || iContentType == DAFF_PHASE_SPECTRUM || iContentType == DAFF_MAGNITUDE_PHASE_SPECTRUM)
		{
			DAFFContentMS* pContent = dynamic_cast< DAFFContentMS* >(pReader->getContent());

			// Number of frequencies
			int iNumFreqs = pContent->getNumFrequencies();
			PyDict_SetItem(pyProperties, PyUnicode_FromString("NumFreqs"), PyLong_FromLong(iNumFreqs));

			// Frequencies
			PyObject* pyFrequencies = PyList_New(0);
			const std::vector<float>& vfFreqs = pContent->getFrequencies();
			for (int i = 0; i < iNumFreqs; i++)
				PyList_Append(pyFrequencies, PyFloat_FromDouble(vfFreqs[i]));
			PyDict_SetItem(pyProperties, PyUnicode_FromString("Frequencies"), pyFrequencies);
		}
		else if (iContentType == DAFF_DFT_SPECTRUM) 
		{
			DAFFContentDFT* pContent = dynamic_cast< DAFFContentDFT* >(pReader->getContent());

			PyDict_SetItem(pyProperties, PyUnicode_FromString("TransformSize"), PyLong_FromLong(pContent->getTransformSize()));
			PyDict_SetItem(pyProperties, PyUnicode_FromString("NumDFTCoeffs"), PyLong_FromLong(pContent->getNumDFTCoeffs()));
			PyDict_SetItem(pyProperties, PyUnicode_FromString("IsSymmetric"), PyBool_FromLong(pContent->isSymmetric()));
			PyDict_SetItem(pyProperties, PyUnicode_FromString("Samplerate"), PyFloat_FromDouble(pContent->getSamplerate()));
			PyDict_SetItem(pyProperties, PyUnicode_FromString("FrequencyBandwidth"), PyFloat_FromDouble(pContent->getFrequencyBandwidth()));
		}


		return pyProperties;
	}


	PyErr_SetString(PyExc_ConnectionError, std::string("Something went wrong.").c_str());
	return Py_None;
}


// ------------------ module definitions ------------------ //

static PyMethodDef daff_methods[] =
{
	{ "open", ( PyCFunction ) daff_open, METH_FASTCALL, open_doc },
	{ "close", ( PyCFunction ) daff_close, METH_FASTCALL, no_doc },
	{ "nearest_neighbour_index", ( PyCFunction ) daff_nearest_neighbour_index, METH_FASTCALL, no_doc },
	{ "nearest_neighbour_record", (PyCFunction)daff_nearest_neighbour_record, METH_FASTCALL, no_doc },
	{ "record", ( PyCFunction ) daff_record, METH_FASTCALL, no_doc },
	{ "content_type", ( PyCFunction ) daff_content_type, METH_FASTCALL, no_doc },
	{ "content_type_str", ( PyCFunction ) daff_content_type_str, METH_FASTCALL, no_doc },
	{ "metadata", ( PyCFunction ) daff_metadata, METH_FASTCALL, no_doc },
	{ "properties", ( PyCFunction ) daff_properties, METH_FASTCALL, no_doc },
	{ NULL, NULL, 0, NULL }        /* Sentinel */
};

static struct PyModuleDef daff_module_def =
{
	PyModuleDef_HEAD_INIT,
	"daffCppInterface",
	daff_doc,
	-1,
	daff_methods,
	NULL,
	NULL,
	NULL,
	NULL
};

PyMODINIT_FUNC PyInit_daffCppInterface( void )
{
	PyObject* pModule = PyModule_Create( &daff_module_def );
	g_pDAFFError = PyErr_NewException( "daffCppInterface.error", NULL, NULL );
	Py_INCREF( g_pDAFFError );

	return pModule;
}
