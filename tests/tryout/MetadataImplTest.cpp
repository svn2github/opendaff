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

#include <algorithm>
#include <iostream>
#include "../../src/DAFFMetadataImpl.h"
#include "../../src/DAFFMetadataImpl.cpp"
#include "../../src/Utils.cpp"

using namespace std;

void PrintKey(const std::string& s) { cout << "Key: " << s << endl; }

int main() {
	DAFFMetadataImpl* md = new DAFFMetadataImpl;

	//DAFFMetadataKey* sKey;
	//md->insertKey("Robot", new DAFFMetadataKeyString( "I am one." ));

	cout << md->toString() << endl;

	vector<string> keys;
	md->getKeys(keys);
	std::for_each(keys.begin(), keys.end(), PrintKey);

	cout << "hasKey(Robot) = " << md->hasKey("Robot") << endl;
	cout << "getStringKey(Robot) = " << md->getKeyString("Robot") << endl;
	
	// Force error:
	cout << "getIntKey(Robot) = " << md->getKeyInt("Robot") << endl;
	
	delete md;
	return 0;
}
