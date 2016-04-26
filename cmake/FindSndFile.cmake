# OpenDAFF
#
# SNDFILE_LIBRARIES library names
# SNDFILE_INCLUDE_DIR, where to find the headers
# SNDFILE_FOUND TRUE if found
#

if( NOT DEFINED SNDFILE_DIR )#
	set( SNDFILE_DIR "NOT-FOUND" CACHE PATH "Path to libsndfile folder with bin, include, lib folder structure" )
endif( NOT DEFINED SNDFILE_DIR )

find_path( SNDFILE_INCLUDE_DIR "sndfile.h"
    "$ENV{SNDFILE_DIR}/include"
    "/usr/local/include"
    "/opt/local/include"
	"${SNDFILE_DIR}/include"
)

find_path( SNDFILE_BINARY_DIRS "libsndfile-1.dll"
    "$ENV{SNDFILE_DIR}/bin"
    "/usr/local/bin"
    "/opt/local/bin"
	"${SNDFILE_DIR}/lib"
	"${SNDFILE_DIR}/bin"
)

find_library( SNDFILE_LIBRARIES "libsndfile-1"
	PATHS
    "$ENV{SNDFILE_DIR}/lib"
    "/usr/local/lib"
    "/opt/local/lib"
	"${SNDFILE_DIR}/lib"
	"${SNDFILE_DIR}/bin"
)

set( SNDFILE_FOUND "" )
if( SNDFILE_LIBRARIES AND SNDFILE_INCLUDE_DIR )
    set( SNDFILE_FOUND "YES" )
    message( STATUS "Found SndFile includes: ${SNDFILE_INCLUDE_DIR}" )
    message( STATUS "Found SndFile libraries: ${SNDFILE_LIBRARIES}" )
    message( STATUS "Found SndFile bin directory: ${SNDFILE_BINARY_DIRS}" )
endif( SNDFILE_LIBRARIES AND SNDFILE_INCLUDE_DIR )
