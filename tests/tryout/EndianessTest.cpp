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
 *  File:		EndianessTest.cpp
 *  Purpose:	A small tryout test for the endianess conversion routines
 *  Version:    $Id: EndianessTest.cpp,v 1.4 2010/03/11 13:08:54 fwefers Exp $
 *	Authors:	Frank Wefers (Frank.Wefers@akustik.rwth-aachen.de)
 *
 */

#include <iostream>
#include <Utils.h>

using namespace std;

int main() {
	// The number 1 as 32-bit in in little endian (LSB first)
	char c[4] = {1, 0, 0, 0};
	int& i = *(int*) c;
	cout << "i before conversion = " << i << endl;
	DAFF::le2se_4byte(&i, 1);
	cout << "i after conversion = " << i << endl;

	return 0;
}
