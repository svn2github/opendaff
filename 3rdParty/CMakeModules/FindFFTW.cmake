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
    ${THIRD_PARTY_DIR}/fftw3/include # ITA
)

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
	${THIRD_PARTY_DIR}/fftw3/Lib/Win32 # ITA
	${THIRD_PARTY_DIR}/fftw3/Lib/x64 # ITA
    )

ENDMACRO(FIND_FFTW_LIBRARY LIBRARY LIBRARYNAME)

# Note: We are only interested in the single-precision version of FFTW3
FIND_FFTW_LIBRARY(FFTW_LIBRARY fftw3f)

SET(FFTW_FOUND "")
IF(FFTW_LIBRARY AND FFTW_INCLUDE_DIR)
    SET(FFTW_FOUND "YES")
    message(STATUS "Found FFTW includes: ${FFTW_INCLUDE_DIR}")
    message(STATUS "Found FFTW library: ${FFTW_LIBRARY}")
ENDIF(FFTW_LIBRARY AND FFTW_INCLUDE_DIR)
