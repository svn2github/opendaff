# This module defines
# FFTW_LIBRARY
# FFTW_FOUND, if false, do not try to link to gdal 
# FFTW_INCLUDE_DIR, where to find the headers
#
# $FFTW_DIR is an environment variable that would
# correspond to the ./configure --prefix=$FFTW_DIR
#
# Created by Frank Wefers based on a script by Robert Osfield

FIND_PATH(FFTW_INCLUDE_DIR fftw3.h
    $ENV{FFTW_DIR}/include
    $ENV{FFTW_DIR}
    /usr/local/include
    /usr/include
    /sw/include # Fink
    /opt/local/include # DarwinPorts
    /opt/csw/include # Blastwave
    /opt/include
    /usr/freeware/include
    ${THIRD_PARTY_DIR}/fftw3/include
)

# Platform sub directories for Visual Studio builds (Win32|x64)
if (MSVC)
	if (CMAKE_CL_64)
		set (FFTW_LIBRARY_WIN_PLATFORM x64)
	else (CMAKE_CL_64)
		set (FFTW_LIBRARY_WIN_PLATFORM Win32)
	endif (CMAKE_CL_64)
elseif (MSVC)
	set (FFTW_LIBRARY_WIN_PLATFORM "")
endif (MSVC)

MACRO(FIND_FFTW_LIBRARY MYLIBRARY MYLIBRARYNAME)

    FIND_LIBRARY(${MYLIBRARY}
        NAMES ${MYLIBRARYNAME}
        PATHS
        $ENV{FFTW_DIR}/lib
        $ENV{FFTW_DIR}
        /usr/local/lib
        /usr/lib
        /sw/lib
        /opt/local/lib
        /opt/csw/lib
        /opt/lib
        /usr/freeware/lib64
		# Order strictly necessary for ITA build environment
		${THIRD_PARTY_DIR}/fftw3/lib/${FFTW_LIBRARY_WIN_PLATFORM}
		${THIRD_PARTY_DIR}/fftw3/Lib
    )

ENDMACRO(FIND_FFTW_LIBRARY LIBRARY LIBRARYNAME)

# Note: We are only interested in the single-precision version of FFTW3
# TODO: This works under windows. Also under linux?
if (WIN32)
	FIND_FFTW_LIBRARY(FFTW_LIBRARY libfftw3f-3)
else (WIN32)
	FIND_FFTW_LIBRARY(FFTW_LIBRARY fftw3f)
endif (WIN32)

message(STATUS ${FFTW_LIBRARY})

SET(FFTW_FOUND "")
IF(FFTW_LIBRARY AND FFTW_INCLUDE_DIR)
    SET(FFTW_FOUND "YES")
    #message(STATUS "Found FFTW includes: ${FFTW_INCLUDE_DIR}")
    message(STATUS "Found FFTW library: ${FFTW_LIBRARY}")
ENDIF(FFTW_LIBRARY AND FFTW_INCLUDE_DIR)
