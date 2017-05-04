cmake_minimum_required( VERSION 2.9 )

find_package( Matlab REQUIRED )

# DAFFv17 Matlab executable library

include_directories( "../../include" )
include_directories( "${MATLAB_INCLUDE_DIR}" )

set( OPENDAFF_DAFFMEX_FILES
	"DAFFMexMain.cpp"
	"DAFFMexDLL.def"
	"DAFFMexHelpers.cpp"
	"DAFFMexHelpers.h"
)

if( MSVC )
	# Deactivate warning C4251 on inconsistent DLL bindings of string and vector
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4251" )
endif( MSVC )

if( OPENDAFF_BUILD_DAFFLIBS_SHARED )
	message( WARNING "For Matlab executables, static linking is recommended" )
endif( OPENDAFF_BUILD_DAFFLIBS_SHARED )

add_library( DAFFv17 SHARED "${OPENDAFF_DAFFMEX_FILES}" )

SET_TARGET_PROPERTIES( DAFFv17 PROPERTIES LINK_FLAGS "/DEF:\"${CMAKE_CURRENT_SOURCE_DIR}/DAFFMexDLL.def\" /NODEFAULTLIB:\"mfc110d\"")

target_link_libraries( DAFFv17 DAFF ${MATLAB_LIBRARIES})
set_property( TARGET DAFFv17 PROPERTY FOLDER "DAFFLibs" )
set_target_properties( DAFFv17 PROPERTIES SUFFIX ".${MATLAB_LIBRARY_WIN_EXTENSION}")
install( TARGETS DAFFv17 RUNTIME DESTINATION "matlab" )
