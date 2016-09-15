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

#ifndef IW_DAFF_METADATAIMPL
#define IW_DAFF_METADATAIMPL

#include <DAFFMetadata.h>
#include <map>

// Forward declarations
class DAFFMetadataKey;

class DAFFMetadataImpl : public DAFFMetadata
{
public:
	DAFFMetadataImpl();
	~DAFFMetadataImpl();

	// Load data from a buffer
	int load( void* pData, size_t &iBytesRead );

	// --= Implementation of the interface "DAFFMetadata" =--

	bool isEmpty() const;
	bool hasKey(const std::string& sKey) const;
	void getKeys(std::vector< std::string >& vsKeyList) const;
	int getKeyType(const std::string& sKey) const;
	std::string getKeyString(const std::string& sKey) const;
	bool getKeyBool(const std::string& sKey) const;
	int getKeyInt(const std::string& sKey) const;
	double getKeyFloat(const std::string& sKey) const;

	std::string toString() const;

private:
	typedef std::pair<std::string, DAFFMetadataKey*> KeyPair;
	typedef std::map<std::string, DAFFMetadataKey*> KeyMap;
	typedef std::map<std::string, DAFFMetadataKey*>::const_iterator KeyMapConstIterator;

	KeyMap m_mKeys;

	//! Insert a key into the keymap
	void insertKey(const std::string& sName, DAFFMetadataKey* pKey);

	//! Search for a key and return the pointer to it (NULL => key not found)
	const DAFFMetadataKey* findKey(const std::string& sKey) const;

	//! Delete functor for key pairs
	static void deleteMetadataKeyPair(const KeyPair& pKeyPair);
};

#endif // IW_DAFF_METADATAIMPL
