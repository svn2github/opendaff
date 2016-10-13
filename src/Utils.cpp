#include "Utils.h"

#ifdef _MSC_VER
// Microsoft Visual Studio compilers
#include <windows.h>
#endif

#include <cstdlib>

#ifdef __APPLE__
// Different include path on MacOS
#include <sys/malloc.h>
#else
#include <malloc.h>
#endif

#include <sys/stat.h>
#include <sys/types.h>

namespace DAFF {

static const double PI_D = acos(-1.0);

// --= Angle conversion and normalization =--

float grad2radf(float phi) {
	return phi * PI_F / 180.0F;
}

double grad2rad(double phi) {
	return phi * PI_D / 180.0;
}

float rad2gradf(float phi) {
	return phi * 180.0F / PI_F;
}

double rad2grad(double phi) {
	return phi * 180.0 / PI_D;
}

float anglef_proj_0_2PI(float alpha) {
	alpha = fmodf(alpha, 2*PI_F);
	if (alpha < 0) alpha += 2*PI_F;
	return alpha;
}

float anglef_proj_0_360_DEG(float alpha) {
	return DAFF::rad2gradf(anglef_proj_0_2PI(DAFF::grad2radf(alpha)));
}

float anglef_mindiff_0_2PI(float alpha, float beta) {
	float gamma = anglef_proj_0_2PI(beta) - anglef_proj_0_2PI(alpha);
	if (gamma >= 0)
		return (gamma <= PI_F ? gamma : gamma-2*PI_F);
	else
		return (gamma >= -PI_F ? gamma : gamma+2*PI_F);
}

float anglef_mindiff_abs_0_2PI(float alpha, float beta) {
	return fabs(anglef_mindiff_0_2PI(alpha, beta));
}

float anglef_mindiff_0_360_DEG(float alpha, float beta) {
	return DAFF::rad2gradf(anglef_mindiff_0_2PI(DAFF::grad2radf(alpha), DAFF::grad2radf(beta)));
}

float anglef_mindiff_abs_0_360_DEG(float alpha, float beta) {
	return DAFF::rad2gradf(anglef_mindiff_abs_0_2PI(DAFF::grad2radf(alpha), DAFF::grad2radf(beta)));
}


// --= Complex number conversion =--

float cabs(float Re, float Im){
	return sqrt(Re*Re+Im*Im);
}

float carg(float Re, float Im){
	if (Re > 0)
		return atan(Im/Re);
	else
		return PI_F+atan(Im/Re);
}

// --= Endianess conversion functions =--


// System endianess test (inspiration: http://www.gamedev.net/reference/articles/article2091.asp)
static const char cTest[4] = {1, 0, 0, 0};
static const int iTest = *(int*) cTest;

// Function forward declarations
void byteswap_2byte(void*, size_t);
void byteswap_3byte(void*, size_t);
void byteswap_4byte(void*, size_t);
void byteswap_8byte(void*, size_t);
void noswap(void*,size_t);

// Function pointers
// Note: iTest == 1  =>  Little endian

/* DEBUG:
void (*DAFF::le2se_2byte)(void* src, size_t count) = &noswap;
void (*DAFF::le2se_4byte)(void* src, size_t count) = &noswap;
void (*DAFF::le2se_8byte)(void* src, size_t count) = &noswap;
*/

/* DEBUG: Inverse
void (*DAFF::le2se_2byte)(void* src, size_t count) = &byteswap_2byte;
void (*DAFF::le2se_4byte)(void* src, size_t count) = &byteswap_4byte;
void (*DAFF::le2se_8byte)(void* src, size_t count) = &byteswap_8byte;
*/

void (*le2se_2byte)(void* src, size_t count) = (iTest == 1 ? &noswap : &byteswap_2byte);
void (*le2se_3byte)(void* src, size_t count) = (iTest == 1 ? &noswap : &byteswap_3byte);
void (*le2se_4byte)(void* src, size_t count) = (iTest == 1 ? &noswap : &byteswap_4byte);
void (*le2se_8byte)(void* src, size_t count) = (iTest == 1 ? &noswap : &byteswap_8byte);

/**
 * [fwe 2009-05-02] This implementation is probably not the most efficient.
 *                  Maybe improve this sometime in the future...
 */

// Swap with temporary variable
#define SWAP(i,j) \
	tmp = p[i]; \
	p[i] = p[j]; \
	p[j] = tmp;

void byteswap_2byte(void* src, size_t count) {

	// In:  |  B1  |  B0  |
    // Out: |  B0  |  B1  |

	char* p = (char*) src;
	char tmp;

	for (size_t i=0; i<count; i++) {
		SWAP(0,1);

		p = p + 2;
	}
}

void byteswap_3byte(void* src, size_t count) {
	
	// In:  |  B2  |  B1  |  B0  |
	// Out: |  B0  |  B1  |  B2  |

	char* p = (char*) src;
	char tmp;

	for (size_t i=0; i<count; i++) {
		SWAP(0,2);

		p = p + 3;
	}
}

void byteswap_4byte(void* src, size_t count) {
	
	// In:  |  B3  |  B2  |  B1  |  B0  |
	// Out: |  B0  |  B1  |  B2  |  B3  |

	char* p = (char*) src;
	char tmp;

	for (size_t i=0; i<count; i++) {
		SWAP(0,3);
		SWAP(1,2);

		p = p + 4;
	}
}

void byteswap_8byte(void* src, size_t count) {

	// In:  |  B7  |  B6  |  B5  |  B4  |  B3  |  B2  |  B1  |  B0  |
	// Out: |  B0  |  B1  |  B2  |  B3  |  B4  |  B5  |  B6  |  B7  |

	char* p = (char*) src;
	char tmp;

	for (size_t i=0; i<count; i++) {
		SWAP(0,7);
		SWAP(1,6);
		SWAP(2,5);
		SWAP(3,4);

		p = p + 8;
	}
}

// Do nothing
void noswap(void*,size_t) {}

// --= Memory (de)allocation =--

void* malloc_aligned16(size_t bytes) {
#ifdef WIN32
	return _aligned_malloc(bytes, 16);
#elif __APPLE__
	// [fwe 2011-10-17] Manpage for malloc on MacOS tells that
	// malloc returns data aligned on a memory boundary,
	// so that AltiVec, SSE, etc. work on these…
	// Let's hope they mean 16-Byte alignment.
	// TODO: Recheck
	return malloc(bytes);
#else
	// Default
	return memalign(16, bytes);
#endif
}

void free_aligned16(void* ptr) {
#ifdef WIN32
	_aligned_free(ptr);
#else
	free(ptr);
#endif
}

// --= Sample type conversion =--


void stc_sint16_to_float( float* dest, const short* src, size_t count, int input_stride, int output_stride, float gain )
{
	float c = gain / 32767.0F;
	for( size_t i = 0; i < count; i++ )
		dest[ i*output_stride ] = ( float ) src[ i*input_stride ] * c;
}

void stc_sint16_to_float_add(float* dest, const short* src, size_t count, int input_stride, int output_stride, float gain) {
	float c = gain / 32767.0F;
	for (size_t i=0; i<count; i++)
		dest[i*output_stride] += (float) src[i*input_stride] * c;
}

void stc_sint24_to_float(float* dest, const void* src, size_t count, int input_stride, int output_stride, float gain) {
	
	const unsigned char* p = (const unsigned char*) src;

	union {
		int ivalue;
		unsigned char byte[4];
	};

	float c = gain / 8388607.0F;

	if (iTest != 1) {
		for (size_t i=0; i<count; i++) {
			
			// Big endian
			byte[1] = p[0];
			byte[2] = p[1];
			byte[3] = p[2];

			// Input value negative (MSB = 1)?
			if (p[1] & 0x80)
				byte[0] = 0xFF;
			else
				byte[0] = 0x00;

			dest[i*output_stride] = (float) ivalue * c;

			p = p + input_stride*3;
		}
	} else {
		for (size_t i=0; i<count; i++) {
			
			// Little endian
			byte[0] = p[0];
			byte[1] = p[1];
			byte[2] = p[2];

			// Input value negative (MSB = 1)?
			if (p[2] & 0x80)
				byte[3] = 0xFF;
			else
				byte[3] = 0x00;

			dest[i*output_stride] = (float) ivalue * c;

			p = p + input_stride*3;
		}
	}
}

void stc_sint24_to_float_add(float* dest, const void* src, size_t count, int input_stride, int output_stride, float gain) {
	
	const unsigned char* p = (const unsigned char*) src;

	union {
		int ivalue;
		unsigned char byte[4];
	};

	float c = gain / 8388607.0F;

	if (iTest != 1) {
		for (size_t i=0; i<count; i++) {
			
			// Big endian
			byte[1] = p[0];
			byte[2] = p[1];
			byte[3] = p[2];

			// Input value negative (MSB = 1)?
			if (p[1] & 0x80)
				byte[0] = 0xFF;
			else
				byte[0] = 0x00;

			dest[i*output_stride] += (float) ivalue * c;

			p = p + input_stride*3;
		}
	} else {
		for (size_t i=0; i<count; i++) {
			
			// Little endian
			byte[0] = p[0];
			byte[1] = p[1];
			byte[2] = p[2];

			// Input value negative (MSB = 1)?
			if (p[2] & 0x80)
				byte[3] = 0xFF;
			else
				byte[3] = 0x00;

			dest[i*output_stride] += (float) ivalue * c;

			p = p + input_stride*3;
		}
	}
}


// --= File system functions =--


int64_t getFileSize(const std::string& sFilename) {
	if (sFilename.empty()) 
		return -1;

#ifdef _MSC_VER
	// Microsoft Visual Studio compilers
	struct _stat64 statinfo;
	if (_stat64(sFilename.c_str(), &statinfo) != 0) 
		return -1;
	return (int64_t) statinfo.st_size;
#else
	// TODO: 64-bit support
	struct stat statinfo;
	if (stat(sFilename.c_str(), &statinfo) != 0)
		return -1;
	return (int64_t) statinfo.st_size;
#endif

}

} // End of namespace "DAFF"
