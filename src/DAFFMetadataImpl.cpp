#include "DAFFMetadataImpl.h"

#include "Utils.h"

#include <algorithm>
#include <cassert>
#include <iomanip>
#include <sstream>

class DAFFMetadataKey
{
public:
	int m_iType;

	inline DAFFMetadataKey(int iType) : m_iType(iType)
	{};

	inline virtual ~DAFFMetadataKey() {};
};

class DAFFMetadataKeyBool : public DAFFMetadataKey
{
public:
	bool m_bValue;

	inline DAFFMetadataKeyBool(bool bValue)
		: DAFFMetadataKey(DAFFMetadata::DAFF_BOOL), m_bValue(bValue) {};
};

class DAFFMetadataKeyInt : public DAFFMetadataKey
{
public:
	int m_iValue;

	inline DAFFMetadataKeyInt(int iValue)
		: DAFFMetadataKey(DAFFMetadata::DAFF_INT), m_iValue(iValue) {};
};

class DAFFMetadataKeyFloat : public DAFFMetadataKey
{
public:
	double m_dValue;

	inline DAFFMetadataKeyFloat(double dValue)
		: DAFFMetadataKey(DAFFMetadata::DAFF_FLOAT), m_dValue(dValue) {};

};

class DAFFMetadataKeyString : public DAFFMetadataKey
{
public:
	std::string m_sValue;

	inline DAFFMetadataKeyString(const std::string& sValue)
		: DAFFMetadataKey(DAFFMetadata::DAFF_STRING), m_sValue(sValue) {};
};

DAFFMetadataImpl::DAFFMetadataImpl() {}

int DAFFMetadataImpl::load(void* pData, size_t &iBytesRead)
{ 
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
	for (int i=0; i<*piNumKeys; i++)
	{
		// Datatype
		piDatatype = (int*) p;
		DAFF::le2se_4byte(piDatatype, 1);
		p += 4;

		// Key name
		std::string sKey(p);
		p += sKey.length()+1;

		// Value
		switch (*piDatatype)
		{
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

	iBytesRead = (size_t) (p - (char*)pData);
	return 0;
}

DAFFMetadataImpl::~DAFFMetadataImpl()
{
	// Delete all keys
	std::for_each(m_mKeys.begin(), m_mKeys.end(), deleteMetadataKeyPair);
	m_mKeys.clear();
}

bool DAFFMetadataImpl::isEmpty() const
{
	return m_mKeys.empty();
}

bool DAFFMetadataImpl::hasKey(const std::string& sKey) const {
	return (findKey(sKey) != NULL);
}

void DAFFMetadataImpl::getKeys(std::vector< std::string >& vsKeyList) const {
	vsKeyList.clear();
	for (KeyMapConstIterator cit=m_mKeys.begin(); cit!=m_mKeys.end(); ++cit)
		vsKeyList.push_back(cit->first);
}

int DAFFMetadataImpl::getKeyType( const std::string& sKey ) const {
	const DAFFMetadataKey* pKey = findKey( sKey );
	return ( pKey != NULL ? pKey->m_iType : -1 );
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

	m_mKeys[sKeyUpper] = pKey;
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
