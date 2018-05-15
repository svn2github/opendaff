# OpenDAFF
#
# PYTHON_LIBRARIES library names
# PYTHON_INCLUDE_DIR, where to find the headers
# PYTHON_FOUND TRUE if found
#

if( NOT DEFINED PYTHON_DIR )#
	set( PYTHON_DIR "NOT-FOUND" CACHE PATH "Path to Python folder with bin, include, lib folder structure" )
endif( NOT DEFINED PYTHON_DIR )

find_path( PYTHON_INCLUDE_DIR "Python.h"
    "$ENV{PYTHON_DIR}/include"
    "/usr/local/include"
    "/opt/local/include"
	"${PYTHON_DIR}/include"
"/usr/include"
)

find_path( PYTHON_BINARY_DIRS "Python.dll"
    "$ENV{PYTHON_DIR}/bin"
    "/usr/local/bin"
    "/opt/local/bin"
	"${PYTHON_DIR}/lib"
	"${PYTHON_DIR}/bin"
)

find_library( PYTHON_LIBRARIES "Python"
	PATHS
    "$ENV{PYTHON_DIR}/lib"
    "/usr/local/lib"
    "/opt/local/lib"
	"${PYTHON_DIR}/lib"
	"${PYTHON_DIR}/bin"
"/usr/lib"
"/usr/lib/arm-linux-gnueabihf"
)

set( PYTHON_FOUND "" )
if( PYTHON_LIBRARIES AND PYTHON_INCLUDE_DIR )
    set( PYTHON_FOUND "YES" )
    message( STATUS "Found Python includes: ${PYTHON_INCLUDE_DIR}" )
    message( STATUS "Found Python libraries: ${PYTHON_LIBRARIES}" )
    message( STATUS "Found Python bin directory: ${PYTHON_BINARY_DIRS}" )
endif( PYTHON_LIBRARIES AND PYTHON_INCLUDE_DIR )
