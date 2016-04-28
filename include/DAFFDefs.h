/*
* -------------------------------------------------------------------------------------
*
*  OpenDAFF - A free, open source software package for directional audio data
*  OpenDAFF is distributed under the terms of the GNU Lesser Public License (LGPL)
*
*  Copyright (c) Institute of Technical Acoustics, RWTH Aachen University, 2009-2016
*
*  ------------------------------------------------------------------------------------
*
*/

#ifndef IW_DAFF_DEFS
#define IW_DAFF_DEFS

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

// Define necessary typedef from stdint.h for Microsoft compilers before Visual C++ 2010
#if _MSC_VER < 1600

#ifdef WIN32
typedef __int8 int8_t;
typedef __int16 int16_t;
typedef __int32 int32_t;
typedef __int64 int64_t;

typedef unsigned __int8 uint8_t;
typedef unsigned __int16 uint16_t;
typedef unsigned __int32 uint32_t;
typedef unsigned __int64 uint64_t;
#endif

#else

// On GNU-platforms include stdint.h automatically
#include <stdint.h>

#endif // _MSC_VER

// Define NULL if it is not present
#ifndef NULL
#define NULL 0
#endif // NULL


//! Content types
enum DAFF_CONTENT_TYPES
{
	DAFF_IMPULSE_RESPONSE=0,			//!< Impulse response (IR) in the time-domain
	DAFF_MAGNITUDE_SPECTRUM=1,			//!< Magnitude spectrum defined at discrete frequencies
	DAFF_PHASE_SPECTRUM=2,				//!< Phase spectrum defined at discrete frequencies
	DAFF_MAGNITUDE_PHASE_SPECTRUM=3,	//!< Magnitude-phase spectrum defined at discrete frequencies
	DAFF_DFT_SPECTRUM=4					//!< Discrete Fourier spectrum in the frequency-domain
};


//! Quantization modes
enum DAFF_QUANTIZATIONS
{
	DAFF_INT16		=0,	//!@ 16-Bit signed integer
	DAFF_INT24		=1,	//!@ 24-Bit signed integer
	DAFF_FLOAT32	=2,	//!@ 32-Bit floating point
};


//! Views on the data (special spherical coordinate system)
enum DAFF_VIEWS
{
	DAFF_DATA_VIEW		=0,	//!@ Data-related view referring to data spherical coordinates (DSC)
	DAFF_OBJECT_VIEW	=1,	//!@ Object-related view referring to object spherical coordinates (OSC)
};


//! Errorcodes
enum DAFF_ERROR 
{
	DAFF_NO_ERROR=0,						//!< No error = 0

	DAFF_MODAL_ERROR=10001,					//!< Modal error (e.g. close a file that is not opened)
	DAFF_FILE_CORRUPTED,					//!< File is broken
	DAFF_FILE_FORMAT_VERSION_UNSUPPORTED,	//!< File format version is not supported by this library version
	DAFF_FILE_NOT_FOUND,					//!< File not found
	DAFF_INVALID_INDEX,						//!< Invalid index (e.g. record index)
};


//! Pure data class that covers version information
struct DAFF_API DAFFVersion
{
	int iVersionMajor;		//!@ Major version (example: 1 for version 1.7)
	int iVersionMinor;		//!@ Minor version (example: 7 for version 1.7)
	std::string sVersion;	//!@ String of version (example: "1.7" for version 1.7)
};


//! Pure data class that describes the points of a quad by their indices and coordinates
struct DAFF_API DAFFQuad
{
	int iIndex1; //!@ First index point
	int iIndex2; //!@ Second index point
	int iIndex3; //!@ Third index point
	int iIndex4; //!@ Fourth index point
};


//! Data class for orientations in yaw-pitch-roll (YPR) angles (right-handed OpenGL coordinate system)
/**
  * Yaw Pitch Roll angles define Euler angles using the OpenGL right-handed Cartesian coordinate system.
  *		1. Yaw arount +Y
  *		2. Pitch around +X
  *		3. Roll around -Z
  */
class DAFF_API DAFFOrientationYPR
{
public:
	float fYawAngleDeg;		//!@ Yaw angle (degrees)
	float fPitchAngleDeg;		//!@ Pitch angle (degrees)
	float fRollAngleDeg;		//!@ Roll angle (degrees)

	//! Default constructor
	inline DAFFOrientationYPR()
		: fYawAngleDeg(0)
		, fPitchAngleDeg(0)
		, fRollAngleDeg(0)
	{};

	//! Initializing constructor
	/**
	  * @param fInitYawAngleDeg	Initial yaw angle in degree
	  * @param fInitPitchAngleDeg	Initial pitch angle in degree
	  * @param fInitRollAngleDeg	Initial roll angle in degre
	  */
	inline DAFFOrientationYPR(float fInitYawAngleDeg, float fInitPitchAngleDeg, float fInitRollAngleDeg)
		: fYawAngleDeg(fInitYawAngleDeg)
		, fPitchAngleDeg(fInitPitchAngleDeg)
		, fRollAngleDeg(fInitRollAngleDeg)
	{};

	//! Return a string of the orientation
	/** 
	  * @return Human readable string of orientation
	  */
	inline std::string toString() const
	{
		std::stringstream ss;
		// Note: \xF8 => Degree sign
		ss << "( Y" << fYawAngleDeg << "\xF8, P" << fPitchAngleDeg << "\xF8, R" << fRollAngleDeg << "\xF8 )";
		return ss.str();
	};
};

#endif // IW_DAFF_DEFS
