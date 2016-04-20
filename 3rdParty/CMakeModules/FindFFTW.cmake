# OpenDAFF
#
# FFTW_LIBRARIES library names
# FFTW_INCLUDE_DIR, where to find the headers
# FFTW_LIBRARY_DIRS folder where libraries can be found
# FFTW_FOUND TRUE if found
#

if( NOT DEFINED FFTW_ROOT_DIR )
	set( FFTW_ROOT_DIR "${OPENDAFF_THIRD_PARTY_DIR}/fftw3" CACHE PATH "Path to FFTW folder with bin, include, lib folder structure" )
endif( NOT DEFINED FFTW_ROOT_DIR )

find_path( FFTW_INCLUDE_DIR "fftw3.h"
    "$ENV{FFTW_DIR}/include"
    "/usr/local/include"
    "/opt/local/include"
    "${OPENDAFF_THIRD_PARTY_DIR}/fftw3/include"
	"${FFTW_ROOT_DIR}/include"
)

# Platform sub directories for Visual Studio builds (Win32|x64)
if( MSVC )
	if( CMAKE_CL_64 )
		set( FTW_LIBRARY_WIN_PLATFORM "x64" )
	else( CMAKE_CL_64 )
		set( FFTW_LIBRARY_WIN_PLATFORM "Win32" )
	endif (CMAKE_CL_64 )
elseif( MSVC )
	set (FFTW_LIBRARY_WIN_PLATFORM "" )
endif( MSVC )

find_path( FFTW_LIBRARY_DIRS "libfftw3f-3.lib"
    "${OPENDAFF_THIRD_PARTY_DIR}/fftw3/lib/${FFTW_LIBRARY_WIN_PLATFORM}"
    "${OPENDAFF_THIRD_PARTY_DIR}/fftw3/lib"
	"${FFTW_ROOT_DIR}/lib"
    "${OPENDAFF_THIRD_PARTY_DIR}/fftw3/bin/${FFTW_LIBRARY_WIN_PLATFORM}"
    "${OPENDAFF_THIRD_PARTY_DIR}/fftw3/bin"
	"${FFTW_ROOT_DIR}/bin"
)

set( FFTW_FOUND "" )
if( FFTW_LIBRARY_DIRS AND FFTW_INCLUDE_DIR )
    set( FFTW_FOUND "YES" )
	set( FFTW_LIBRARIES "libfftw3f-3" )
    message( STATUS "Found FFTW includes: ${FFTW_INCLUDE_DIR}" )
    message( STATUS "Found FFTW libraries: ${FFTW_LIBRARIES}" )
    message( STATUS "Found FFTW library directories: ${FFTW_LIBRARY_DIRS}" )
endif( FFTW_LIBRARY_DIRS AND FFTW_INCLUDE_DIR )
