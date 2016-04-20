# Locate gdal
# This module defines
# FOX_LIBRARY
# FOX_FOUND, if false, do not try to link to gdal 
# FOX_INCLUDE_DIR, where to find the headers
#
# $FOX_DIR is an environment variable that would
# correspond to the ./configure --prefix=$FOX_DIR
#
# Created by Robert Osfield. 
# Extended by Jonas Stienen.

FIND_PATH(FOX_INCLUDE_DIR fx.h
    $ENV{FOX_DIR}/include/fox-1.6
    $ENV{FOX_DIR}/fox-1.6
    ~/Library/Frameworks/fox-1.6
    /Library/Frameworks/fox-1.6
    /usr/local/include/fox-1.6
    /usr/include/fox-1.6
    /sw/include/fox-1.6 # Fink
    /opt/local/include/fox-1.6 # DarwinPorts
    /opt/csw/include/fox-1.6 # Blastwave
    /opt/include/fox-1.6
    /usr/freeware/include/fox-1.6
    $ENV{FOX_DIR}/include
    $ENV{FOX_DIR}
    ~/Library/Frameworks
    /Library/Frameworks
    /usr/local/include
    /usr/include
    /sw/include # Fink
    /opt/local/include # DarwinPorts
    /opt/csw/include # Blastwave
    /opt/include
    /usr/freeware/include
	${THIRD_PARTY_DIR}/fox-1.6/include # ITA
)

# Platform sub directories for Visual Studio builds (Win32|x64)
if (MSVC)
	if (CMAKE_CL_64)
		set (FOX_LIBRARY_WIN_PLATFORM x64)
	else (CMAKE_CL_64)
		set (FOX_LIBRARY_WIN_PLATFORM Win32)
	endif (CMAKE_CL_64)
elseif (MSVC)
	set (FOX_LIBRARY_WIN_PLATFORM "")
endif (MSVC)

MACRO(FIND_FOX_LIBRARY MYLIBRARY MYLIBRARYNAME)

    FIND_LIBRARY(${MYLIBRARY}
        NAMES ${MYLIBRARYNAME}
        PATHS
        $ENV{FOX_DIR}/lib
        $ENV{FOX_DIR}
        ~/Library/Frameworks
        /Library/Frameworks
        /usr/local/lib
        /usr/lib
        /sw/lib
        /opt/local/lib
        /opt/csw/lib
        /opt/lib
        /usr/freeware/lib64
		# Order strictly necessary for ITA build environment
		${THIRD_PARTY_DIR}/fox-1.6/lib/${FOX_LIBRARY_WIN_PLATFORM}
		${THIRD_PARTY_DIR}/fox-1.6/lib
    )

ENDMACRO(FIND_FOX_LIBRARY LIBRARY LIBRARYNAME)

# Use shared linking under Windows
# TODO: Define somewhere else. Maybe a switch?
if (WIN32)
	FIND_FOX_LIBRARY(FOX_LIBRARY FOXDLL-1.6)
else (WIN32)
	FIND_FOX_LIBRARY(FOX_LIBRARY FOX-1.6)
endif (WIN32)

SET(FOX_FOUND "")
IF(FOX_LIBRARY AND FOX_INCLUDE_DIR)
    SET(FOX_FOUND "YES")
    #message(STATUS "Found FOX toolkit includes: ${FOX_INCLUDE_DIR}")
    message(STATUS "Found FOX toolkit library: ${FOX_LIBRARY}")
ENDIF(FOX_LIBRARY AND FOX_INCLUDE_DIR)