/*
 * -------------------------------------------------------------------------------------
 *
 *  OpenDAFF - A free, open source software package for directional audio data
 *  Copyright 2016-2018 Institute of Technical Acoustics (ITA), RWTH Aachen University
 *  OpenDAFF is distributed under the Apache License Version 2.0.
 *
 *  ------------------------------------------------------------------------------------
 *
 */

#ifndef IW_DAFF_CS_WRAPPER
#define IW_DAFF_CS_WRAPPER

#if defined WIN32
 #ifdef DAFFCS_EXPORTS
  #define DAFFCS_API __declspec( dllexport )
 #else
  #define DAFFCS_API __declspec( dllimport )
 #endif
#else
 #define DAFFCS_API
#endif

#include <string>
#include <vector>

class CUnmanagedDAFFHandle;
class DAFFContentIR;
class DAFFContentMS;

extern "C"
{
	DAFFCS_API CUnmanagedDAFFHandle* NativeDAFFCreate();
	DAFFCS_API void NativeDAFFDispose( CUnmanagedDAFFHandle* );
	DAFFCS_API bool NativeDAFFLoad( CUnmanagedDAFFHandle*, const char* );
	DAFFCS_API int NativeDAFFGetContentType( CUnmanagedDAFFHandle* );

	DAFFCS_API DAFFContentIR* NativeDAFFGetContentIR( CUnmanagedDAFFHandle* );
	DAFFCS_API int NativeDAFFContentIRGetNearestNeighbourRecordIndex( DAFFContentIR*, double, double );
	DAFFCS_API void NativeDAFFContentIRGetRecordCoords( DAFFContentIR*, const int, double*, double* );
	DAFFCS_API bool NativeDAFFContentIRGetRecordData( DAFFContentIR*, const int, const int, float* );
	DAFFCS_API int NativeDAFFContentIRGetLength( DAFFContentIR* pDAFFContent );

	DAFFCS_API DAFFContentMS* NativeDAFFGetContentMS( CUnmanagedDAFFHandle* );
	DAFFCS_API int NativeDAFFContentMSGetNearestNeighbourRecordIndex( DAFFContentMS*, double, double );
	DAFFCS_API void NativeDAFFContentMSGetRecordCoords( DAFFContentMS*, const int, double*, double* );
	DAFFCS_API bool NativeDAFFContentMSGetRecordData( DAFFContentMS*, const int, const int, float* );
	DAFFCS_API int NativeDAFFContentMSGetLength( DAFFContentMS* pDAFFContent );
}

#endif // IW_DAFF_CS_WRAPPER
