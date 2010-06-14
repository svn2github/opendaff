/*
 *  OpenDAFF - A free, open-source software package for directional audio data,
 *  OpenDAFF is distributed under the terms of the GNU Lesser Public License (LGPL)
 *
 *  Copyright (C) Institute of Technical Acoustics, RWTH Aachen University, 2009-2010
 *
 *  Visit the OpenDAFF homepage: http://www.opendaff.org
 *
 *
 *  --= License & warranty =--
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
 *
 *  --= File information =--
 *
 *  File:		Helpers.cpp
 *  Purpose:	DAFFMEX helper functions
 *  Version:    $Id: Helpers.cpp,v 1.3 2010/03/11 13:08:54 fwefers Exp $
 *	Authors:	Frank Wefers (Frank.Wefers@akustik.rwth-aachen.de)
 *
 */
#include "Helpers.h"

#include <cmath>
#include <string>

bool isScalar(const mxArray *A) {
	// TODO: Dieser Test ist möglicherweise nicht 100%tig sicher.
	//       Leider gibt es keine MEX-Funktion zum Testen auf Skalar.

	mwSize s = mxGetNumberOfDimensions(A);
	if (s != 2) return false;
	const mwSize* d = mxGetDimensions(A);
	return ((d[0] == 1) && (d[1] == 1));
}

bool isVector(const mxArray *A) {
	mwSize s = mxGetNumberOfDimensions(A);
	if (s != 2) return false;
	const mwSize* d = mxGetDimensions(A);
	return ((d[0] == 1) || (d[1] == 1));
}

bool isRowVector(const mxArray *A) {
	mwSize s = mxGetNumberOfDimensions(A);
	if (s != 2) return false;
	const mwSize* d = mxGetDimensions(A);
	return (d[0] == 1);
}

bool isColumnVector(const mxArray *A) {
	mwSize s = mxGetNumberOfDimensions(A);
	if (s != 2) return false;
	const mwSize* d = mxGetDimensions(A);
	return (d[1] == 1);
}

bool getDoubleScalar(const mxArray *A, double& dest) {
	// TODO: Test auf Skalar
	if (!mxIsNumeric(A) || !isScalar(A)) return false;

	dest = *mxGetPr(A);
	return true;
}

bool getIntegerScalar(const mxArray *A, int& dest) {
	double x;
	if (!getDoubleScalar(A, x)) return false;
    
	if (floor(x) != x) return false;

	dest = (int) x;
	return true;
}

bool getIntegerVector(const mxArray *A, std::vector<int>& dest) {
	if (!mxIsNumeric(A)) return false;
	if (!isVector(A)) return false;

	const mwSize* d = mxGetDimensions(A);
	int n = (int) (std::max)(d[0], d[1]);
	dest.resize(n);
	double* data = mxGetPr(A);
	for (int i=0; i<n; i++) {
		double x = data[i];

		// Test auf Ganzzahl
		if (floor(x) != x) return false;

		dest[i] = (int) x;
	}

	return true;
}

bool getString(const mxArray *A, std::string& dest) {
	// TODO: Test auf Zeichen
	if (!mxIsChar(A) || !isRowVector(A)) return false;

	char buf[1024];
	mxGetString(A, buf, 1024);
	dest = buf;
	return true;
}

bool getRealScalar(const mxArray *A, double& dest) {
	if (!mxIsNumeric(A) || !isScalar(A) || mxIsComplex(A)) return false;

	dest = *mxGetPr(A);
	return true;
}
