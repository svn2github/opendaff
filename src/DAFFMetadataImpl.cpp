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
 *  File:		DAFFMetadataImpl.cpp
 *  Purpose:	Meta implementation
 *  Version:    $Id: DAFFMetadataImpl.cpp,v 1.9 2010/03/11 12:49:27 fwefers Exp $
 *	Authors:	Frank Wefers (Frank.Wefers@akustik.rwth-aachen.de)
 *
 */

#include <DAFFMetadataImpl.h>

#include <Utils.h>

#include <algorithm>
#include <cassert>
#include <iomanip>
#include <sstream>

class DAFFMetadataKey {
public:
	int m_iType;

	DAFFMetadataKey(int iType) : m_iType(iType) {
		
	}

	virtual ~DAFFMetadataKey() {};
};

class DAFFMetadataKeyBool : public DAFFMetadataKey {
public:
	bool m_bValue;

	DAFFMetadataKeyBool(bool bValue)
		: DAFFMetadataKey(DAFFMetadata::DAFF_BOOL), m_bValue(bValue) {};
};

class DAFFMetadataKeyInt : public DAFFMetadataKey {
public:
	int m_iValue;

	DAFFMetadataKeyInt(int iValue)
		: DAFFMetadataKey(DAFFMetadata::DAFF_INT), m_iValue(iValue) {};
};

class DAFFMetadataKeyFloat : public DAFFMetadataKey {
public:
	double m_dValue;

	DAFFMetadataKeyFloat(double dValue)
		: DAFFMetadataKey(DAFFMetadata::DAFF_FLOAT), m_dValue(dValue) {};

};

class DAFFMetadataKeyString : public DAFFMetadataKey {
public:
	std::string m_sValue;

	DAFFMetadataKeyString(const std::string& sValue)
		: DAFFMetadataKey(DAFFMetadata::DAFF_STRING), m_sValue(sValue) {};
};

DAFFMetadataImpl::DAFFMetadataImpl() {}

int DAFFMetadataImpl::load(void* pData) {
	char* p = (char*) pData;

	// Read the number of keys
	int* piNumKeys = (int*) p;
	DAFF::le2se_4byte(piNumKeys, 1);
	p += 4;

	int* piDatatype;
	int* piValue;
	double* pdValue;
	std::string sValue;

	// Read the keys
	for (int i=0; i<*piNumKeys; i++) {
		// Datatype
		piDatatype = (int*) p;
		DAFF::le2se_4byte(piDatatype, 1);
		p += 4;

		// Key name
		std::string sKey(p);
		p += sKey.length()+1;

		// Value
		switch (*piDatatype) {
		case DAFF_BOOL:
			piValue = (int*) p;
			DAFF::le2se_4byte(piValue, 1);
			p += 4;

			insertKey(sKey, new DAFFMetadataKeyBool( *piValue != 0 ));
			break;

		case DAFF_INT:
			piValue = (int*) p;
			DAFF::le2se_4byte(piValue, 1);
			p += 4;

			insertKey(sKey, new DAFFMetadataKeyInt( *piValue ));
			break;

		case DAFF_FLOAT:
			pdValue = (double*) p;
			DAFF::le2se_8byte(pdValue, 1);
			p += 8;

			insertKey(sKey, new DAFFMetadataKeyFloat( *pdValue ));
			break;

		case DAFF_STRING:
			sValue = p;
			p += sValue.length()+1;

			insertKey(sKey, new DAFFMetadataKeyString( sValue ));
			break;

		default:
			return DAFF_FILE_CORRUPTED;
		}
	}

	return 0;
};

DAFFMetadataImpl::~DAFFMetadataImpl() {
	// Delete all keys
	std::for_each(m_mKeys.begin(), m_mKeys.end(), deleteMetadataKeyPair);
	m_mKeys.clear();
};

bool DAFFMetadataImpl::hasKey(const std::string& sKey) const {
	return (findKey(sKey) != NULL);
}

void DAFFMetadataImpl::getKeys(std::vector< std::string >& vsKeyList) const {
	vsKeyList.clear();
	for (KeyMapConstIterator cit=m_mKeys.begin(); cit!=m_mKeys.end(); ++cit)
		vsKeyList.push_back(cit->first);
}

int DAFFMetadataImpl::getKeyType(const std::string& sKey) const {
	const DAFFMetadataKey* pKey = findKey(sKey);
	return (pKey != NULL ? pKey->m_iType : -1);
}

std::string DAFFMetadataImpl::getKeyString(const std::string& sKey) const {
	const DAFFMetadataKey* pKey = findKey(sKey);
	assert( pKey != NULL );

	if (pKey == NULL) return "";

	std::stringstream ss;
	switch (pKey->m_iType) {
	case DAFF_BOOL:
		return (dynamic_cast<const DAFFMetadataKeyBool*>(pKey)->m_bValue ? "yes" : "no");

	case DAFF_INT:
		ss << dynamic_cast<const DAFFMetadataKeyInt*>(pKey)->m_iValue;
		return ss.str();

	case DAFF_FLOAT:
		ss << dynamic_cast<const DAFFMetadataKeyFloat*>(pKey)->m_dValue;
		return ss.str();

	case DAFF_STRING:
		return dynamic_cast<const DAFFMetadataKeyString*>(pKey)->m_sValue;

	default:
		// This will never be reached, but satisfies the compiler warning...
		return "";
	}
}

bool DAFFMetadataImpl::getKeyBool(const std::string& sKey) const {
	const DAFFMetadataKey* pKey = findKey(sKey);
	assert( pKey != NULL );
	assert( pKey->m_iType == DAFF_BOOL );

	if ((pKey == NULL) || (pKey->m_iType != DAFF_BOOL)) return 0;
	return dynamic_cast<const DAFFMetadataKeyBool*>(pKey)->m_bValue;
}

int DAFFMetadataImpl::getKeyInt(const std::string& sKey) const {
	const DAFFMetadataKey* pKey = findKey(sKey);
	assert( pKey != NULL );
	assert( pKey->m_iType == DAFF_INT );

	if ((pKey == NULL) || (pKey->m_iType != DAFF_INT)) return 0;
	return dynamic_cast<const DAFFMetadataKeyInt*>(pKey)->m_iValue;
}

double DAFFMetadataImpl::getKeyFloat(const std::string& sKey) const {
	const DAFFMetadataKey* pKey = findKey(sKey);
	assert( pKey != NULL );
	assert( (pKey->m_iType == DAFF_FLOAT) || (pKey->m_iType == DAFF_INT) );

	if (pKey == NULL) return 0;
	if ((pKey->m_iType != DAFF_FLOAT) && (pKey->m_iType != DAFF_INT)) return 0;

	if (pKey->m_iType == DAFF_FLOAT)
		return dynamic_cast<const DAFFMetadataKeyFloat*>(pKey)->m_dValue;
	else
		return (double) dynamic_cast<const DAFFMetadataKeyInt*>(pKey)->m_iValue;
}

std::string DAFFMetadataImpl::toString() const {
	std::stringstream ss;
	for (KeyMapConstIterator cit=m_mKeys.begin(); cit!=m_mKeys.end(); ++cit) {
		ss << cit->first << " = " << getKeyString(cit->first) << std::endl;
	}

	return ss.str();
}

void DAFFMetadataImpl::insertKey(const std::string& sName, DAFFMetadataKey* pKey) {
	// For later key search: Ensure that the keyname is upper case 
	std::string sKeyUpper(sName);
	std::transform(sKeyUpper.begin(), sKeyUpper.end(), sKeyUpper.begin(), (int(*)(int)) ::toupper);

	m_mKeys.insert( KeyPair(sKeyUpper, pKey) );
};

const DAFFMetadataKey* DAFFMetadataImpl::findKey(const std::string& sKey) const {
	// Empty strings are no valid key names
	if (sKey.empty()) return NULL;

	// Convert the keyname to upper case
	std::string sKeyUpper(sKey);
	std::transform(sKeyUpper.begin(), sKeyUpper.end(), sKeyUpper.begin(), (int(*)(int)) ::toupper);

	// Search in the keymap
	KeyMapConstIterator cit = m_mKeys.find(sKeyUpper);
	return (cit == m_mKeys.end() ? NULL : cit->second);
}

void DAFFMetadataImpl::deleteMetadataKeyPair(const KeyPair& pKeyPair) {
	delete pKeyPair.second;
}
