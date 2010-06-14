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

#include <DAFF.h>

using namespace std;

int main() {
	DAFFReader* r = DAFFReader::create();

	int ec = r->openFile("ir.daff");
	if (ec != 0) {
		cerr << "Error: " << DAFFUtils::StrError(ec) << endl;
		exit(ec);
	}

	cout << r->toString() << endl;
	cout << r->getMetadata()->toString() << endl;

	// Access the content
	if (r->getContentType() == DAFF_IMPULSE_RESPONSE) {
		DAFFContentIR* x = dynamic_cast<DAFFContentIR*>( r->getContent() );
		
		int n = x->getFilterLength();
		float* h = new float[n];

		x->getFilterCoeffs(0,0, h);

		delete[] h;
	}

	if (r->getContentType() == DAFF_MAGNITUDE_SPECTRUM) {
		DAFFContentMS* x = dynamic_cast<DAFFContentMS*>( r->getContent() );
		
		int n = x->getNumFrequencies();
		std::vector<float> f = x->getFrequencies();

		float* m = new float[n];

		x->getMagnitudes(2000, 0, m);

		delete[] m;
	}

	r->closeFile();
	
	delete r;

	return 0;
}
