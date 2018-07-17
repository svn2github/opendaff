/*
* -------------------------------------------------------------------------------------
*
*  OpenDAFF - A free, open source software package for directional audio data
*  OpenDAFF is distributed under the terms of the GNU Lesser Public License (LGPL)
*
*  Copyright (c) Institute of Technical Acoustics (ITA), RWTH Aachen University, 2009-2016
*
*  ------------------------------------------------------------------------------------
*
*/

#ifndef IW_DAFF_TOOL_EXPORT
#define IW_DAFF_TOOL_EXPORT

#include <string>

// Forward declarations
class DAFFContentIR;
class DAFFContentMS;
class DAFFContentPS;
class DAFFContentMPS;
class DAFFContentDFT;

// Write an impulse response record into an audiofile
int exportIR(DAFFContentIR* pContentIR, int iRecordIndex, const std::string& sFilename, int iView, bool bQuiet, bool bVerbose);

// Convert a magnitude spectrum record into dat parsable file output
int exportMS(DAFFContentMS* pContentMS, const std::string& sFilename, int iRecordIndex, float fAngle1, float fAngle2, int iView, bool bQuiet, bool bVerbose);

// Convert a phase spectrum record into dat parsable file output
int exportPS(DAFFContentPS* pContentPS, const std::string& sFilename, int iRecordIndex, float fAngle1, float fAngle2, int iView, bool bQuiet, bool bVerbose);

// Convert a magnitude phase spectrum record into dat parsable file output
int exportMPS(DAFFContentMPS* pContentMPS, const std::string& sFilename, int iRecordIndex, float fAngle1, float fAngle2, int iView, bool bQuiet, bool bVerbose);

// Convert a magnitude phase spectrum record into dat parsable file output
int exportDFT(DAFFContentDFT* pContentDFT, const std::string& sFilename, int iRecordIndex, float fAngle1, float fAngle2, int iView, bool bQuiet, bool bVerbose);

// Convert a magnitude spectrum record into dat parsable file output
// (iRecordIndex == -1 => Dump all records)
std::string convertMS2dat(DAFFContentMS* pContentMS, int iRecordIndex, float fAngle1, float fAngle2, int iView, bool bQuiet, bool bVerbose);

// Convert a phase spectrum record into dat parsable file output
// (iRecordIndex == -1 => Dump all records)
std::string convertPS2dat(DAFFContentPS* pContentPS, int iRecordIndex, float fAngle1, float fAngle2, int iView, bool bQuiet, bool bVerbose);

// Convert a magnitude phase spectrum record into dat parsable file output
// (iRecordIndex == -1 => Dump all records)
std::string convertMPS2dat(DAFFContentMPS* pContentMPS, int iRecordIndex, float fAngle1, float fAngle2, int iView, bool bQuiet, bool bVerbose);

// Convert a DFT spectrum record into dat parsable file output
// (iRecordIndex == -1 => Dump all records)
std::string convertDFT2dat(DAFFContentDFT* pContentDFT, int iRecordIndex, float fAngle1, float fAngle2, int iView, bool bQuiet, bool bVerbose);

#endif // IW_DAFF_TOOL_EXPORT