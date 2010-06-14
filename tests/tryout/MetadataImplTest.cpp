/*
 *  A small tryout test for the metadata implementation class
 *  Autor: Frank.Wefers@akustik.rwth-aachen.de
 */

#include <algorithm>
#include <iostream>
#include <DAFFMetadataImpl.h>
#include <DAFFMetadataImpl.cpp>

using namespace std;

void PrintKey(const std::string& s) { cout << "Key: " << s << endl; }

int main() {
	DAFFMetadataImpl* md = new DAFFMetadataImpl;

	DAFFMetadataKey* sKey;
	md->insertKey("Robot", new DAFFMetadataKeyString( "I am one." ));

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
