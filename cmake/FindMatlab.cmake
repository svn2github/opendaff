# OpenDAFF
#
# MATLAB_LIBRARIES library names
# MATLAB_INCLUDE_DIR, where to find the headers
# MATLAB_FOUND TRUE if found
#

if( NOT DEFINED MATLAB_DIR )
	set( MATLAB_DIR "NOT-FOUND" CACHE PATH "Path to Matlab folder with bin, include, lib folder structure" )
endif( NOT DEFINED MATLAB_DIR )

find_path( MATLAB_INCLUDE_DIR "mex.h"
    "$ENV{MATLAB_DIR}/extern/include"
    "/usr/local/include"
    "/opt/local/include"
	"${MATLAB_DIR}/extern/include"
)

# Platform sub directories for Visual Studio builds (Win32|x64)
if( MSVC )
	if( CMAKE_CL_64 )
		set( MATLAB_LIBRARY_WIN_PLATFORM "win64" )
		set( MATLAB_LIBRARY_WIN_EXTENSION "mexw64" )
	else( CMAKE_CL_64 )
		set( MATLAB_LIBRARY_WIN_PLATFORM "win32" )
		set( MATLAB_LIBRARY_WIN_EXTENSION "mexw64" )
	endif (CMAKE_CL_64 )
elseif( MSVC )
	set (MATLAB_LIBRARY_WIN_PLATFORM "" )
endif( MSVC )

find_library( MATLAB_LIBRARY1 libmex
	PATHS
    "${MATLAB_DIR}/extern/lib/${MATLAB_LIBRARY_WIN_PLATFORM}/microsoft"
	"${MATLAB_DIR}/extern/lib"
    "${MATLAB_DIR}/extern/bin/${MATLAB_LIBRARY_WIN_PLATFORM}/microsoft"
	"${MATLAB_DIR}/extern/bin"
)

find_library( MATLAB_LIBRARY2 libmx
	PATHS
    "${MATLAB_DIR}/extern/lib/${MATLAB_LIBRARY_WIN_PLATFORM}/microsoft"
	"${MATLAB_DIR}/extern/lib"
    "${MATLAB_DIR}/extern/bin/${MATLAB_LIBRARY_WIN_PLATFORM}/microsoft"
	"${MATLAB_DIR}/extern/bin"
)

set( MATLAB_LIBRARIES ${MATLAB_LIBRARY1} ${MATLAB_LIBRARY2} )

set( MATLAB_FOUND "" )
if( MATLAB_LIBRARIES AND MATLAB_INCLUDE_DIR )
    set( MATLAB_FOUND "YES" )
    message( STATUS "Found Matlab includes: ${MATLAB_INCLUDE_DIR}" )
    message( STATUS "Found Matlab libraries: ${MATLAB_LIBRARIES}" )
endif( MATLAB_LIBRARIES AND MATLAB_INCLUDE_DIR )
