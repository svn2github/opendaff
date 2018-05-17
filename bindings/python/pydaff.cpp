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
	// @todo armin
	return PyLong_FromLong( -1 );
}

static PyObject* daff_nearest_neighbour_record( PyObject*, PyObject** ppArgs, Py_ssize_t nArgs, PyObject* pKeywordNames )
{
	// @todo armin, compare VA struct in VAPython
	return Py_None;
}

static PyObject* daff_content_type( PyObject*, PyObject** ppArgs, Py_ssize_t nArgs, PyObject* pKeywordNames )
{
	static const char * const _keywords[] = { "content_type", NULL };
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


static PyObject* daff_dummy( PyObject*, PyObject** ppArgs, Py_ssize_t nArgs, PyObject* pKeywordNames )
{
	// @todo armin, remove when unused
	return Py_None;
}

// ------------------ module definitions ------------------ //

static PyMethodDef daff_methods[] =
{
	{ "open", ( PyCFunction ) daff_open, METH_FASTCALL, open_doc },
	{ "close", ( PyCFunction ) daff_close, METH_FASTCALL, no_doc },
	{ "nearest_neighbour_index", ( PyCFunction ) daff_nearest_neighbour_index, METH_FASTCALL, no_doc },
	{ "nearest_neighbour_record", ( PyCFunction ) daff_nearest_neighbour_record, METH_FASTCALL, no_doc },
	{ "content_type", ( PyCFunction ) daff_content_type, METH_FASTCALL, no_doc },
	{ "content_type_str", ( PyCFunction ) daff_dummy, METH_FASTCALL, no_doc },
	{ "metadata", ( PyCFunction ) daff_dummy, METH_FASTCALL, no_doc },
	{ "properties", ( PyCFunction ) daff_dummy, METH_FASTCALL, no_doc },
	{ "metadata", ( PyCFunction ) daff_dummy, METH_FASTCALL, no_doc },
	{ NULL, NULL, 0, NULL }        /* Sentinel */
};

static struct PyModuleDef daff_module_def =
{
	PyModuleDef_HEAD_INIT,
	"daff",
	daff_doc,
	-1,
	daff_methods,
	NULL,
	NULL,
	NULL,
	NULL
};

PyMODINIT_FUNC PyInit_daff( void )
{
	PyObject* pModule = PyModule_Create( &daff_module_def );
	g_pDAFFError = PyErr_NewException( "daff.error", NULL, NULL );
	Py_INCREF( g_pDAFFError );

	return pModule;
}
