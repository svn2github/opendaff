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
 *  File:		DAFFMetadataImpl.h
 *  Purpose:	Meta implementation headerfile
 *  Version:    $Id: DAFFMetadataImpl.h,v 1.7 2010/03/11 13:08:54 fwefers Exp $
 *	Authors:	Frank Wefers (Frank.Wefers@akustik.rwth-aachen.de)
 *
 */

#ifndef __DAFFMETADATAIMPL_H__
#define __DAFFMETADATAIMPL_H__

#include <DAFFMetadata.h>
#include <map>

// Forward declarations
class DAFFMetadataKey;

class DAFFMetadataImpl : public DAFFMetadata {
public:
	DAFFMetadataImpl();
	~DAFFMetadataImpl();

	// Load data from a buffer
	int load(void* pData);

	// --= Implementation of the interface "DAFFMetadata" =--

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

	// Insert a key into the keymap
	void insertKey(const std::string& sName, DAFFMetadataKey* pKey);

	// Search for a key and return the pointer to it (NULL => key not found)
	const DAFFMetadataKey* findKey(const std::string& sKey) const;

	// Delete functor for key pairs
	static void deleteMetadataKeyPair(const KeyPair& pKeyPair);
};

#endif // __DAFFMETADATAIMPL_H__
