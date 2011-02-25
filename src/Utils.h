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
 *  File:		Library-internal utility functions
 *  Purpose:	Collection of utility functions
 *  Version:    $Id: Utils.h,v 1.7 2010/03/11 12:49:27 fwefers Exp $
 *	Authors:	Frank Wefers (Frank.Wefers@akustik.rwth-aachen.de),
 *              Jonas Stienen (Jonas.Stienen@akustik.rwth-aachen.de)
 *
 */

/*
 *  OpenDAFF - A free, open-source software package for directional audio data,
 *             distributed under the terms of the GNU Lesser Public License (LGPL)
 *
 *  (c) Copyright Institute of Technical Acoustics, RWTH Aachen University
 *
 *  File:		Utils.h
 *  Purpose:	Library-internal utility functions
 *	Authors:	Frank Wefers (Frank.Wefers@akustik.rwth-aachen.de)
 *              Jonas Stienen (Jonas.Stienen@akustik.rwth-aachen.de)
 */

// $Id: Utils.h,v 1.7 2010/03/11 12:49:27 fwefers Exp $

#ifndef __UTILS_H__
#define __UTILS_H__

#include <DAFFDefs.h>

// STL includes
#include <cmath>
#include <string>
#include <cstring> // required for size_t

namespace DAFF {

// Define necessary roundf for Microsoft compilers
#ifdef _MSC_VER 
#define roundf(x) (x<0?ceil((x)-0.5f):floor((x)+0.5f));
#endif // _MSC_VER

// --= Global constants =--

static const float PI_F = acos(-1.0F);
static const float HALF_PI_F = PI_F/2;
static const float TWO_PI_F = 2*PI_F;

// --= Angle conversion and normalization =--

float grad2radf(float phi);
float rad2gradf(float phi);

double grad2rad(double phi);
double rad2grad(double phi);

// --= Complex number conversion =--

float cabs(float Re, float Im);
float carg(float Re, float Im);

// degrees
float anglef_proj_0_360_DEG(float alpha);
float anglef_mindiff_0_360_DEG(float alpha, float beta);
float anglef_mindiff_abs_0_360_DEG(float alpha, float beta);

// radiants
float anglef_proj_0_2PI(float alpha);
float anglef_mindiff_0_2PI(float alpha, float beta);
float anglef_mindiff_abs_0_2PI(float alpha, float beta);

// --= Endianess conversion functions =--
 
/*
 *  All convert from little endian (used within the DAFF files)
 *  into the systems endianess (little endian -> system endianess)
 *
 */

extern void (*le2se_2byte)(void* src, size_t count);
extern void (*le2se_3byte)(void* src, size_t count);
extern void (*le2se_4byte)(void* src, size_t count);
extern void (*le2se_8byte)(void* src, size_t count);

// --= Memory (de)allocation =--

// Allocate/free memory on with a 16-byte boundary
void* malloc_aligned16(size_t bytes);
void free_aligned16(void* ptr);

// --= Sample type conversion =--

//! Convert signed integer 16-Bit -> single precision floating point (32-Bit)
void stc_sint16_to_float(float* dest, const short* src, size_t count, int input_stride=1, int output_stride=1);

//! Convert signed integer 24-Bit -> single precision floating point (32-Bit)
void stc_sint24_to_float(float* dest, const void* src, size_t count, int input_stride=1, int output_stride=1);


// --= File system functions =--

//! Größe einer Datei in Bytes zurückgeben (Gibt im Fehlerfall -1 zurück)
int64_t getFileSize(const std::string& sFilename);


} // End of namespace "DAFF"

#endif // __UTILS_H__
