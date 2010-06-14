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
 *  File:		Export.h
 *  Purpose:	DAFFTool export routines headerfile
 *  Version:    $Id: Export.h,v 1.3 2010/03/11 13:08:54 fwefers Exp $
 *	Authors:	Frank Wefers (Frank.Wefers@akustik.rwth-aachen.de),
 *              Jonas Stienen (Jonas.Stienen@akustik.rwth-aachen.de) 
 *
 */

#ifndef __EXPORT_H__
#define __EXPORT_H__

#include <string>

// Forward declarations
class DAFFContentIR;
class DAFFContentMS;

// Write an impulse response record into an audiofile
int exportIR(DAFFContentIR* pContentIR, int iRecordIndex, const std::string& sFilename, int iView, bool bQuiet, bool bVerbose);

// Convert a magnitude spectrum record into dat parsable file output
// (iRecordIndex == -1 => Dump all records)
std::string convertMS2dat(DAFFContentMS* pContentMS, int iRecordIndex, int iView, bool bQuiet, bool bVerbose);

// Convert a magnitude spectrum record into dat parsable file output
int exportMS(DAFFContentMS* pContentMS, const std::string& sFilename, int iRecordIndex, int iView, bool bQuiet, bool bVerbose);


#endif // __EXPORT_H__