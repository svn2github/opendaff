/*
 * -------------------------------------------------------------------------------------
 *
 *  OpenDAFF - A free, open source software package for directional audio data
 *  Copyright 2016 Institute of Technical Acoustics, RWTH Aachen University
 *  OpenDAFF is distributed under the Apache License Version 2.0.
 *
 *  ------------------------------------------------------------------------------------
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
