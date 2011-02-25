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
 *  File:		ReaderTest.cpp
 *  Purpose:	A small tryout test for the reader class
 *  Version:    $Id: ReaderTest.cpp,v 1.8 2010/03/11 13:08:54 fwefers Exp $
 *	Authors:	Frank Wefers (Frank.Wefers@akustik.rwth-aachen.de)
 *
 */

#include <iostream>
#include <math.h>

#include <DAFF.h>
#define EPSILON 0.01

using namespace std;

int main() {
	DAFFReader* r = DAFFReader::create();

	// check impulse response
{
	int ec = r->openFile("test_ir.daff");
	if (ec != 0) {
		cerr << "Error: " << DAFFUtils::StrError(ec) << endl;
		cerr << "Run daff_write_test.m first to create the daff files." << endl;
		exit(ec);
	}

	cout << r->toString() << endl;
	cout << r->getMetadata()->toString() << endl;

	// Access the content
	DAFFContentIR* x = dynamic_cast<DAFFContentIR*>( r->getContent() );
	
	int n = x->getFilterLength();
	if (n==8)
		cout << "Filterlength: " << n << " OK" << endl;
	else {
		cerr << "Incorrect filterlength: " << n << endl;
		exit(1);
	}

	float* h = new float[n];
	int i=0;
	int num = x->getProperties()->getNumberOfRecords();
	for (i=0; i<num; i++) //i=recordindex
	{
		for(int j=0; j<2; j++) 
		{
			// check effective filterlength
			int offset=0, length=0;
			x->getEffectiveFilterBounds(i, 0, offset, length);
			if (length!=4) {
				cerr << "Incorrect effective filterlength: " << length << endl;
				exit(1);
			}

			// check coeffs
			float alpha=0, beta=0;
			x->getFilterCoeffs(i,0, h);
			x->getRecordCoords(i, DAFF_DATA_VIEW, alpha, beta);
			if ( (fabs(h[0]) > EPSILON )
				|| (fabs(h[1]) > EPSILON )
				|| (fabs(h[2]) > EPSILON )
				|| (fabs(h[3]) > EPSILON )
				|| (fabs(h[4]) > EPSILON )
				|| (fabs(h[5]-1) > EPSILON)
				|| (fabs(h[6]*360-alpha) > EPSILON)
				|| (fabs(h[7]*360-beta) > EPSILON) ) {
				cerr << "Coeffs do not match: " << h[0] << " " << h[1] << " " << h[2] << " " << h[3] << " " << h[4] << " " << h[5] << " " << h[6] <<  " " << h[7] << endl;
				cerr << "Should be: 0 0 0 0 0 1 " << alpha/360 << " " << beta/360 << endl;
				exit(1);
			}
		}
	}
	cout << num << " Coeffs match. OK" << endl;

	delete[] h;
	r->closeFile();
}

	// check magnitude spectrum 
{
	int ec = r->openFile("test_ms.daff");
	if (ec != 0) {
		cerr << "Error: " << DAFFUtils::StrError(ec) << endl;
		cerr << "Run daff_write_test.m first to create the daff files." << endl;
		exit(ec);
	}

	cout << r->toString() << endl;
	cout << r->getMetadata()->toString() << endl;

	// Access the content
	DAFFContentMS* x = dynamic_cast<DAFFContentMS*>( r->getContent() );
	
	int n = x->getNumFrequencies();
	if (n==7)
		cout << "Number of frequencies: " << n << " OK" << endl;
	else {
		cerr << "Incorrect number of frequencies: " << n << endl;
		exit(1);
	}
	std::vector<float> f = x->getFrequencies();

	float* h = new float[n];
	int num = x->getProperties()->getNumberOfRecords();
	for (int i=0; i<num; i++) //i=recordindex
	{
		// check coeffs
		float alpha=0, beta=0;
		x->getMagnitudes(i,0, h);
		x->getRecordCoords(i, DAFF_DATA_VIEW, alpha, beta);
		if ( (fabs(h[0]) > EPSILON )
			|| (fabs(h[1]) > EPSILON )
			|| (fabs(h[2]) > EPSILON )
			|| (fabs(h[3]) > EPSILON )
			|| (fabs(h[4]-1) > EPSILON)
			|| (fabs(h[5]*360-alpha) > EPSILON)
			|| (fabs(h[6]*360-beta) > EPSILON) ) {
			cerr << "Coeffs do not match: " << h[0] << " " << h[1] << " " << h[2] << " " << h[3] << " " << h[4] << " " << h[5] << " " << h[6] << endl;
			cerr << "Should be: 0 0 0 0 1 " << alpha/360 << " " << beta/360 << endl;
			exit(1);
		}
	}
	cout << num << " Coeffs match. OK" << endl;

	delete[] h;
}

	r->closeFile();
	
	delete r;

	return 0;
}
