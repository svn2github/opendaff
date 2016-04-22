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
*/

#include <iostream>
#include "../../src/Utils.h"

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
