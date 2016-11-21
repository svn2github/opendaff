# OpenDAFF
#
# FFTW_LIBRARIES library names
# FFTW_INCLUDE_DIR, where to find the headers
# FFTW_FOUND TRUE if found
#

if( NOT DEFINED FFTW_DIR )
	set( FFTW_DIR "NOT-FOUND" CACHE PATH "Path to FFTW folder with bin, include, lib folder structure" )
endif( NOT DEFINED FFTW_DIR )

find_path( FFTW_INCLUDE_DIR "fftw3.h"
    "$ENV{FFTW_DIR}/include"
    "/usr/include"
    "/usr/local/include"
    "/opt/local/include"
	"${FFTW_DIR}/include"
)

find_path( FFTW_BINARY_DIRS "libfftw3f-3"
    "$ENV{FFTW_DIR}/bin"
"/usr/lib"
    "/usr/local/bin"
    "/opt/local/bin"
	"${FFTW_DIR}/bin"
)

# Platform sub directories for Visual Studio builds (Win32|x64)
if( MSVC )
	if( CMAKE_CL_64 )
		set( FTWW_LIBRARY_WIN_PLATFORM "x64" )
	else( CMAKE_CL_64 )
		set( FFTW_LIBRARY_WIN_PLATFORM "Win32" )
	endif (CMAKE_CL_64 )
elseif( MSVC )
	set (FFTW_LIBRARY_WIN_PLATFORM "" )
endif( MSVC )

find_library( FFTW_LIBRARIES "fftw3"
	PATHS
    "${FFTW_DIR}/lib/${FFTW_LIBRARY_WIN_PLATFORM}"
	"${FFTW_DIR}/lib"
    "${FFTW_DIR}/bin/${FFTW_LIBRARY_WIN_PLATFORM}"
	"${FFTW_DIR}/bin"
"/usr/lib"
)

set( FFTW_FOUND "" )
if( FFTW_LIBRARIES AND FFTW_INCLUDE_DIR )
    set( FFTW_FOUND "YES" )
    message( STATUS "Found FFTW includes: ${FFTW_INCLUDE_DIR}" )
    message( STATUS "Found FFTW libraries: ${FFTW_LIBRARIES}" )
    message( STATUS "Found FFTW bin directory: ${FFTW_BINARY_DIRS}" )
endif( FFTW_LIBRARIES AND FFTW_INCLUDE_DIR )
