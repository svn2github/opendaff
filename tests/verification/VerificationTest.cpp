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
 *  File:		VerificationTest.cpp
 *  Purpose:	Verification test for the DAFF coordinate transforms
 *  Version:    $Id: $
 *	Authors:	Frank Wefers (Frank.Wefers@akustik.rwth-aachen.de)
 *
 */

#include <cassert>
#include <iostream>
#include <math.h>

#include <DAFF.h>
#define EPSILON 0.01

using namespace std;

int main() {
	DAFFReader* r = DAFFReader::create();

	int ec = r->openFile("verify_1x1_orient_0-0-0.daff");
	if (ec != DAFF_NO_ERROR) {
		cerr << "Error: " << DAFFUtils::StrError(ec) << endl;
		cerr << "Run daff_write_test.m first to create the daff files." << endl;
		exit(ec);
	}

	cout << r->toString() << endl;
	cout << r->getMetadata()->toString() << endl;

	// Access the content
	DAFFContentIR* x = dynamic_cast<DAFFContentIR*>( r->getContent() );
	
	float data[4];

	for (int beta=0; beta<=180; beta++)
	{
		for (int alpha=0; alpha<360; alpha++)
		{
			int index;
			bool outside;

			x->getNearestNeighbour(DAFF_DATA_VIEW, (float) alpha, (float) beta, index, outside);
			assert( !outside );

			x->getFilterCoeffs(index, 0, data);			

			// Ensure ordinals
			assert( (data[0] - floor(data[0])) == 0 );
			int value = (int) data[0];

			float alpha2 = float(value % 1000);
			float beta2 = (value / 1000.0f);

			cout << "in (" << alpha << "," << beta << ") => out (" << alpha2 << "," << beta2 << ")" << endl;

			if ((fabs(alpha - alpha2) > EPSILON) || (fabs(beta - beta2) > EPSILON)) {
				// Ignore single records at the poles
				if ((beta == 0) || (beta == 180)) continue;

				cout << "Error!" << endl;
				assert( false );
			}
		}
	}
	
	r->closeFile();
}
