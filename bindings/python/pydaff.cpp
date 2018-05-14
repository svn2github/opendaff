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

#include "pydaffdoc.hpp"

static PyObject* g_pDAFFError = nullptr; //!< Static pointer to error instance

static PyMethodDef daff_methods[] = {
	{ "open", ( PyCFunction ) open, METH_FASTCALL, "Open a DAFF file." },
		{ NULL, NULL, 0, NULL }        /* Sentinel */
};

static struct PyModuleDef daffmoduledef =
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

PyMODINIT_FUNC PyInit_va( void )
{
	PyObject* pModule = PyModule_Create( &daffmoduledef );
	g_pDAFFError = PyErr_NewException( "daff.error", NULL, NULL );
	Py_INCREF( g_pDAFFError );

	//PyAdd
	return pModule;
}

static PyObject* open( PyObject*, PyObject** ppArgs, Py_ssize_t nArgs, PyObject* pKeywordNames )
{
	static const char * const _keywords[] = { "open", NULL };
	static _PyArg_Parser _parser = { "s:connect", _keywords, 0 };
	char* pcFilePath = nullptr;

	if( !_PyArg_ParseStack( ppArgs, nArgs, pKeywordNames, &_parser, &pcFilePath ) )
		return NULL;

	std::string sFilePath = pcFilePath ? std::string( pcFilePath ) : "";

	int iID = -1; // DAFF open file
	if( iID > 0 )
		return PyLong_FromLong( iID );

	PyErr_SetString( PyExc_ConnectionError, std::string( "Could not open " + sFilePath ).c_str() );
	return NULL;
}
