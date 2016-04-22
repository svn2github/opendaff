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

#ifndef IW_DAFF_UTILS
#define IW_DAFF_UTILS

#include <DAFFDefs.h>
#include <string>

//! Collection of utility functions
/**
 * This purely static class summerizes utility functions.
 * These include convenient type-to-string conversions and
 * normalization functions.
 */

class DAFF_API DAFFUtils {
public:
	//! Get the library version
	static void getLibraryVersion(DAFFVersion& version);

	//! Format a direction (angular pair) as a string canonically
	/**
	 * Format for data view directions [A => alpha, B => beta]:  (A123.1°, B123.167°)
	 * Format for object view directions [P => phi, T => theta]: (P+123.1°, T-10.167°)
	 */
	static std::string StrDirection(int iView, double dAngle1, double dAngle2, int precision=9, int leadingzeros=3);

	//! Format a direction (angular pair) as a string canonically and in more compact form
	/**
	 * Format for data view directions [A => alpha, B => beta]:  A123.1° B123.167°
	 * Format for object view directions [P => phi, T => theta]: P+123.1° T-10.167°
	 */
	static std::string StrDirectionCompact(int iView, double dAngle1, double dAngle2, int precision=9, int leadingzeros=3);

	//! Return the string corresponding to an errorcode
	static std::string StrError(int iErrorcode);

	//! Returns a string corresponding to a content type
	static std::string StrContentType(int iContentType);
	
	//! Returns a short form string corresponding to a content type e.g. ("ir", "ms", "dft"...)
	static std::string StrShortContentType(int iContentType);

	//! Returns a string corresponding to a metadata key datatype
	static std::string StrMetadataKeyType(int iKeyType);

	//! Returns a string corresponding to a quantization type
	static std::string StrQuantizationType(int iQuantizationType);

	//! Normalize a direction (angular pair)
	/**
	 * The methods normalizes directions regarding a spherical coordinate system (data|object)
	 * and projects the angles into the following ranges:
	 *
	 * [DSC] alpha in [0°,360°), beta in [0°,180°]
	 * [OSC] phi in (-180°,180°], elevation in [-90°,+90°]
	 *
	 * The method defines canonical forms of directions, which
	 * are more easy to interpret (by humans).
	 */
	static void NormalizeDirection(int iView,
		                           const float fAngle1In, const float fAngle2In,
								   float& fAngle1Out, float& fAngle2Out);

	//! Converts a single precision floating point into nice std::string
	/**
	 * Removes trailing zeros and rounds to the 3rd decimal point
	 */
	static std::string Float2StrNice(float f, int precision, bool showpos, int leadingzeros=0);

	//! Converts a double precision floating point into nice std::string
	/**
	 * Removes trailing zeros and rounds to the 3rd decimal point
	 */
	static std::string Double2StrNice(double d, int precision, bool showpos, int leadingzeros=0);

private:
	DAFFUtils() {}
	DAFFUtils(const DAFFUtils& rhs) {}
	~DAFFUtils() {}
};

#endif // IW_DAFF_UTILS
