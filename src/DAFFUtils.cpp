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
 *  File:		DAFFUtils.cpp
 *  Purpose:	Collection of utility functions
 *  Version:    $Id: DAFFUtils.cpp,v 1.18 2010/03/11 12:49:27 fwefers Exp $
 *	Authors:	Frank Wefers (Frank.Wefers@akustik.rwth-aachen.de)
 *
 */

#include <DAFFUtils.h>

#include <DAFFDefs.h>
#include <DAFFMetadata.h>

#include <cmath>
#include <cassert>
#include <cstring>
#include <iomanip>

// Define necessary roundf for Microsoft compilers
#ifdef _MSC_VER 
#define roundf(x) (x<0?ceil((x)-0.5f):floor((x)+0.5f));
#endif // _MSC_VER


void DAFFUtils::getLibraryVersion(DAFFVersion& version) {
	// Version 0.1
	version.iVersionMajor = 0;
	version.iVersionMinor = 105;
	version.sVersion = "0.105";
}

// Helper function: Convert float => std::string (fixed format, but without trailing zeros)
std::string DAFFUtils::Float2StrNice(float f, int precision, bool showpos, int leadingzeros) {
	// First convert to fixed format (with enough digits)
	std::stringstream ss;
	if (showpos) ss << (f < 0.0f ? "-" : "+");
	ss << std::fixed << std::setprecision(precision);
	if (leadingzeros > 0) ss << std::setw(leadingzeros+precision+1) << std::setfill('0');
	if (showpos) ss << fabs(f);
	else ss << f;
	std::string s = ss.str();

	// Then remove trailing zeros...
	size_t n=s.length();
	while ((n>0) && (s[n-1] == '0')) n--;

	// Special case: We reached the . => Remove this as well...
	if ((n>1) && (s[n-1] == '.')) n--;

	return s.substr(0, n);
}

// Helper function: Convert double => std::string (fixed format, but without trailing zeros)
std::string DAFFUtils::Double2StrNice(double d, int precision, bool showpos, int leadingzeros) {
	// First convert to fixed format (with enough digits)
	std::stringstream ss;
	if (showpos) ss << (d < 0.0f ? "-" : "+");
	ss << std::fixed << std::setprecision(precision);
	if (leadingzeros > 0) ss << std::setw(leadingzeros+precision+1) << std::setfill('0');
	if (showpos) ss << fabs(d);
	else ss << d;
	std::string s = ss.str();

	// Then remove trailing zeros...
	size_t n=s.length();
	while ((n>0) && (s[n-1] == '0')) n--;

	// Special case: We reached the . => Remove this as well...
	if ((n>1) && (s[n-1] == '.')) n--;

	return s.substr(0, n);
}

std::string DAFFUtils::StrDirection(int iView, double dAngle1, double dAngle2, int precision, int leadingzeros) {
	std::stringstream ss;

	switch (iView) {
	case DAFF_DATA_VIEW:
		ss << std::fixed
		   << "( A" << Double2StrNice(dAngle1, precision, false, leadingzeros)
		   << "\xF8, B" << Double2StrNice(dAngle2, precision, false, leadingzeros) << "\xF8 )";
		return ss.str();
		break;

	case DAFF_OBJECT_VIEW:
		ss << std::fixed << std::showpos
		   << "( P" << Double2StrNice(dAngle1, precision, true, leadingzeros)
		   << "\xF8, T" << Double2StrNice(dAngle2, precision, true, leadingzeros) << "\xF8 )";
		return ss.str();
		break;

	default:
		// Invalid view
		assert( false );
		return "";
	}
	// we will never reach this point	
	return "";
}

std::string DAFFUtils::StrDirectionCompact(int iView, double dAngle1, double dAngle2, int precision, int leadingzeros) {
	std::stringstream ss;

	switch (iView) {
	case DAFF_DATA_VIEW:
		ss << std::fixed
		   << "A" << Double2StrNice(dAngle1, precision, false, leadingzeros)
		   << "\xF8 B" << Double2StrNice(dAngle2, precision, false, leadingzeros) << "\xF8";
		return ss.str();
		break;

	case DAFF_OBJECT_VIEW:
		ss << std::fixed << std::showpos
		   << "P" << Double2StrNice(dAngle1, precision, true, leadingzeros)
		   << "\xF8 T" << Double2StrNice(dAngle2, precision, true, leadingzeros) << "\xF8";
		return ss.str();
		break;

	default:
		// Invalid view
		assert( false );
		return "";
	}
	// we will never reach this point	
	return "";
}

std::string DAFFUtils::StrError(int iErrorcode) {
	switch (iErrorcode) {

	case DAFF_MODAL_ERROR: return "Modal error";
	case DAFF_FILE_CORRUPTED: return "File corrupted";
	case DAFF_FILE_FORMAT_VERSION_UNSUPPORTED: return "File format version unsupported";
	case DAFF_FILE_NOT_FOUND: return "File not found";
	case DAFF_INVALID_INDEX: return "Invalid index";

	// For all other errors use the C-Function
	default: return strerror(iErrorcode);
	}
}

std::string DAFFUtils::StrContentType(int iContentType) {
	switch (iContentType) {

	case DAFF_IMPULSE_RESPONSE:				return "Impulse response";
	case DAFF_MAGNITUDE_SPECTRUM:			return "Magnitude spectrum";
	case DAFF_PHASE_SPECTRUM:				return "Phase spectrum";
	case DAFF_MAGNITUDE_PHASE_SPECTRUM:		return "Magnitude-phase spectrum";
	case DAFF_DFT_SPECTRUM:					return "Discrete Fourier-spectrum";
	default: return "Invalid";
	}
}

std::string DAFFUtils::StrShortContentType(int iContentType) {
	switch (iContentType) {

	case DAFF_IMPULSE_RESPONSE:				return "ir";
	case DAFF_MAGNITUDE_SPECTRUM:			return "ms";
	case DAFF_PHASE_SPECTRUM:				return "ps";
	case DAFF_MAGNITUDE_PHASE_SPECTRUM:		return "mps";
	case DAFF_DFT_SPECTRUM:					return "dft";
	default: return "Invalid";
	}
}

std::string DAFFUtils::StrMetadataKeyType(int iKeyType) {
	switch (iKeyType) {

	case DAFFMetadata::DAFF_BOOL:   return "Boolean";
	case DAFFMetadata::DAFF_INT:    return "Integer";
	case DAFFMetadata::DAFF_FLOAT:  return "Float";
	case DAFFMetadata::DAFF_STRING: return "String";

	default: return "Invalid";
	}
}

std::string DAFFUtils::StrQuantizationType(int iQuantizationType) {
	switch (iQuantizationType) {

	case DAFF_INT16:   return "16-bit signed integer";
	case DAFF_INT24:   return "24-bit signed integer";
	case DAFF_FLOAT32: return "32-bit float point";

	default: return "Invalid";
	}
}

void DAFFUtils::NormalizeDirection(int iView,
                                   const float fAngle1In, const float fAngle2In,
								   float& fAngle1Out, float& fAngle2Out) {

	const float EPSILON = 0.00001F;	// 10^-5 °

	if (iView == DAFF_DATA_VIEW) {

		/*
		 *  - Alpha always within [0°, +360°)
		 *  - Beta always within [0°, +180°]
		 *  - At poles (beta=0°|beta=180°) default: alpha = 0
		 */

		float fAlpha = fAngle1In;
		float fBeta = fmodf(fAngle2In, 360.0f);

		if (fBeta > 180.0f) {
			fAlpha += 180.0f;
			fBeta -= 180.0f;
		}

		fAlpha = fmodf(fAlpha, 360.0f);
		if (fAlpha < 0.0f)
			fAlpha += 360.0f;

		// Note: We use thresholds here to work on wrongly rounded angles also
		if ((std::abs( fBeta ) <= EPSILON) ||		// Beta == 0°
			(std::abs( fBeta-180.0f ) <= EPSILON))	// Beta == 180°
			fAlpha = 0.0f;

		fAngle1Out = roundf(fAlpha*1000.0f);
		fAngle1Out /= 1000.0f;
		fAngle2Out = roundf(fBeta*1000.0f);
		fAngle2Out /= 1000.0f;

		return;
	}

	if (iView == DAFF_OBJECT_VIEW) {
		/*
		 *  - Azimuth always within (-180°, +180°]
		 *  - Elevation always within [-90°, +90°]
		 *  - At poles (elevation=+/-90°) default: azimuth = 0
		 */

		float fAzimuth = fAngle1In;
		float fElevation = fmodf(fAngle2In, 360.0F);

		if ((fElevation > 90.0F) && (fElevation < 270.0F)) {
			fAzimuth += 180.0F;
			fElevation = 180.0F - fElevation;
		}

		// Project azimuth angle into (-180°, +180°]
		fAzimuth = fmodf(fAzimuth + 180.0F, 360.0F) - 180.0F;
		if (fAzimuth == -180.0F) fAzimuth = +180.0F;

		// Note: We use thresholds here to work on wrongly rounded angles also
		if (std::abs(fElevation-90.0f) <= EPSILON)	// Elevation == 90°
			fElevation = 90.0f;
		if (std::abs(fElevation+90.0f) <= EPSILON)	// Elevation == -90°
			fElevation = -90.0f;

		fAngle1Out = roundf(fAzimuth*1000.0f);
		fAngle1Out /= 1000.0f;
		fAngle2Out = roundf(fElevation*1000.0f);
		fAngle2Out /= 1000.0f;

		return;
	}

	// Invalid view
	assert( false );
}
