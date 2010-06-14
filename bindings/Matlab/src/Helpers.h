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
 *  File:		Helpers.h
 *  Purpose:	DAFFMEX helper functions headerfile
 *  Version:    $Id: Helpers.h,v 1.4 2010/03/11 13:08:54 fwefers Exp $
 *	Authors:	Frank Wefers (Frank.Wefers@akustik.rwth-aachen.de)
 *
 */

#ifndef __HELPERS_H__
#define __HELPERS_H__

#include <mex.h>
#include <string>
#include <vector>

bool isScalar(const mxArray *A);
bool isVector(const mxArray *A);
bool isRowVector(const mxArray *A);
bool isColumnVector(const mxArray *A);
bool getDoubleScalar(const mxArray *A, double& dest);
bool getIntegerScalar(const mxArray *A, int& dest);
bool getIntegerVector(const mxArray *A, std::vector<int>& dest);
bool getString(const mxArray *A, std::string& dest);

bool getRealScalar(const mxArray *A, double& dest);

#endif // __HELPERS_H__