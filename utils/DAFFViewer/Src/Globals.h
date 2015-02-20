// $Id: Globals.h,v 1.3 2010/02/08 18:55:49 fwefers Exp $

#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#define APPNAME		"DAFF Viewer"

#define VERSION		"1.23"
#define DATE		"$Date: $"

#ifdef WIN32
	#ifdef _M_X64
		#define PLATFORM	"Windows 64-Bit"
	#else
		#define PLATFORM	"Windows 32-Bit"
	#endif
#endif

#ifndef PLATFORM
#define PLATFORM	L""
#endif

//#define INI_FILE "Preferences.ini"
//
//#define AUDIOFILE_PATTERNS "Alle Dateien (*)\nAlle Audiodateien (*.dat,*.wav)\nMonkey Forest Audiodateien (*.dat)\nMicrosoft Wave Audiodateien (*.wav)\n"
//#define DEFAULT_AUDIOFILE_PATTERN 1
//
//#define HRTFDATABASE_PATTERNS "Alle Dateien (*)\nITA HRTF-Datenbanken (*.ddb)\n"
//#define DEFAULT_HRTFDATABASE_PATTERN 1

#endif // __GLOBALS_H__
