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
 *  File:		DAFFMetadata.h
 *  Purpose:	Metadata interface definition
 *  Version:    $Id: DAFFMetadata.h,v 1.7 2010/03/11 12:49:27 fwefers Exp $
 *	Authors:	Frank Wefers (Frank.Wefers@akustik.rwth-aachen.de),
 *              Jonas Stienen (Jonas.Stienen@akustik.rwth-aachen.de)
 *
 */

#ifndef __DAFFMETADATA_H__
#define __DAFFMETADATA_H__

#include <DAFFDefs.h>
#include <string>
#include <vector>

//! Metadata interface
/**
 * This purely abstract class defines the interface for accessing the
 * metadata of DAFF files. The metadata contains an arbitrary number of
 * (key, value) pairs. Keys are indexed by their name (case-insensitive).
 * Values are types. Valid datatypes are boolean, integer, floating-point
 * and string.
 *
 * Note: The method getKeyString can be used for each key datatype.
 *       Other methods cause errors when they are called on keys of wrong datatype.
 *       (e.g. getKeyBool for a floating-point key)
 */
class DAFF_API DAFFMetadata {
public:
	//! Metadata types
	enum {
		DAFF_BOOL=0,	//<! Boolean (true|false, yes|no)
		DAFF_INT=1,		//<! Integer number
		DAFF_FLOAT=2,  //<! Floating-point number
		DAFF_STRING=3  //<! String
	};

	//! Destructor
	virtual ~DAFFMetadata() {};

	//! Returns if metadata does not contain any keys
	virtual bool isEmpty() const=0;

	//! Checks for existence of a key
	virtual bool hasKey(const std::string& sKey) const=0;

	//! Returns a list of all keys
	virtual void getKeys(std::vector< std::string >& vsKeyList) const=0;

	//! Returns the datatype of a key
	virtual int getKeyType(const std::string& sKey) const=0;

	//! Returns the value of a string key
	/**
	 * Note: This method may be called for every key, no matter what type
	 */
	virtual std::string getKeyString(const std::string& sKey) const=0;

	//! Returns the value (true|false) of a boolean key
	virtual bool getKeyBool(const std::string& sKey) const=0;

	//! Returns the value of a integer number key
	virtual int getKeyInt(const std::string& sKey) const=0;

	//! Returns the value of a floating-point number key
	/**
	 * Note: This method may also be called for INT keys
	 */
	virtual double getKeyFloat(const std::string& sKey) const=0;

	//! Return information on the metadata as string
	virtual std::string toString() const=0;
};

#endif // __DAFFMETADATA_H__
