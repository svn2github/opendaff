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
 *  File:		DAFFDefs.h
 *  Purpose:	Global definitions
 *  Version:    $Id: DAFFDefs.h,v 1.16 2010/03/11 12:49:27 fwefers Exp $
 *	Authors:	Frank Wefers (Frank.Wefers@akustik.rwth-aachen.de)
 *              Jonas Stienen (jst@akustik.rwth-aachen.de)
 *
 */

#ifndef __DAFFDEFS_H__
#define __DAFFDEFS_H__

// Import/export directives
#ifdef WIN32
	// Windows DLL
	#ifdef DAFF_DLL
		#ifdef DAFF_DLL_EXPORTS
			#define DAFF_API __declspec(dllexport)
		#else
			#define DAFF_API __declspec(dllimport)
		#endif
	#endif
#endif

// Default directive: None
#ifndef DAFF_API
	#define DAFF_API
#endif

#include <string>
#include <sstream>

/* +-----------------------------------+ *
 * |                                   | *
 * |   Global scope type definitions   | *
 * |                                   | *
 * +-----------------------------------+ */

// Define necessary typedef from stdint.h for Microsoft compilers
#ifdef _MSC_VER 

typedef __int8 int8_t;
typedef __int16 int16_t;
typedef __int32 int32_t;
typedef __int64 int64_t;

typedef unsigned __int8 uint8_t;
typedef unsigned __int16 uint16_t;
typedef unsigned __int32 uint32_t;
typedef unsigned __int64 uint64_t;

#else

// On GNU-platforms include stdint.h automatically
#include <stdint.h>

#endif // _MSC_VER

// Define NULL if it is not present
#ifndef NULL
#define NULL 0
#endif // NULL


//! Content types
enum {
	DAFF_IMPULSE_RESPONSE=0,		//!< Impulse response (IR) in the time-domain
	DAFF_MAGNITUDE_SPECTRUM			//!< Magnitude spectrum defined at discrete frequencies
};


//! Quantization modes
enum {
	DAFF_INT16=0,	//! 16-Bit signed integer
	DAFF_INT24,		//! 24-Bit signed integer
	DAFF_FLOAT32,	//! 32-Bit floating point
};


//! Views
enum {
	DAFF_DATA_VIEW=0,	//! Data-related view referring to data spherical coordinates (DSC)
	DAFF_OBJECT_VIEW,	//! Object-related view referring to object spherical coordinates (OSC)
};


//! Errorcodes
enum {
	DAFF_NO_ERROR=0,

	DAFF_MODAL_ERROR=10001,					//!< Modal error (e.g. close a file that is not opened)
	DAFF_FILE_CORRUPTED,					//!< File is broken
	DAFF_FILE_FORMAT_VERSION_UNSUPPORTED,	//!< File format version is not supported by this library version
	DAFF_FILE_NOT_FOUND,					//!< File not found
	DAFF_INVALID_INDEX,						//!< Invalid index (e.g. record index)
};


//! Pure data class that covers version information
class DAFF_API DAFFVersion {
public:
	int iVersionMajor;		// Major version (example: 1 for version 1.24)
	int iVersionMinor;		// Minor version (example: 24 for version 1.24)
	std::string sVersion;	// String of version (example: "1.24" for version 1.24)
};


//! Pure data class that describes the points of a quad by their indices and coordinates
class DAFF_API DAFFQuad {
public:
	// Point indices
	int iIndex1, iIndex2, iIndex3, iIndex4;
};


//! Data class for orientations in yaw-pitch-roll (YPR) angles
class DAFF_API DAFFOrientationYPR {
public:
	float fYawAngle;		// Yaw angle (degrees)
	float fPitchAngle;		// Pitch angle (degrees)
	float fRollAngle;		// Roll angle (degrees)

	//! Default constructor
	DAFFOrientationYPR()
		: fYawAngle(0), fPitchAngle(0), fRollAngle(0) {}

	//! Initializing constructor
	DAFFOrientationYPR(float fInitYawAngle, float fInitPitchAngle, float fInitRollAngle)
		: fYawAngle(fInitYawAngle), fPitchAngle(fInitPitchAngle), fRollAngle(fInitRollAngle) {}

	//! Return a string of the orientation
	std::string toString() const {
		std::stringstream ss;
		// Note: \xF8 => Degree sign
		ss << "( Y" << fYawAngle << "\xF8, P" << fPitchAngle << "\xF8, R" << fRollAngle << "\xF8)";
		return ss.str();
	}
};

#endif // __DAFFDEFS_H__
