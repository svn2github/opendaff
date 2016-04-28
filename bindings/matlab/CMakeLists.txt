cmake_minimum_required( VERSION 2.9 )

find_package( Matlab REQUIRED )

# daffmex lib

include_directories( "../../include" )
include_directories( "${MATLAB_INCLUDE_DIR}" )

set( OPENDAFF_DAFFMEX_FILES
	"DAFFMEX.cpp"
	"Helpers.cpp"
	"Helpers.h"
)

if( MSVC )
	# Deactivate warning C4251 on inconsistent DLL bindings of string and vector
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4251" )
endif( MSVC )

if( OPENDAFF_BUILD_DAFFLIBS_SHARED )
	message( WARNING "For Matlab executables, static linking is recommended" )
endif( OPENDAFF_BUILD_DAFFLIBS_SHARED )

add_library( DAFFMEX SHARED "${OPENDAFF_DAFFMEX_FILES}" )
target_link_libraries( DAFFMEX DAFF ${MATLAB_LIBRARIES})
set_property( TARGET DAFFMEX PROPERTY FOLDER "DAFFLibs" )
#set_target_properties( DAFFMEX PROPERTIES PREFIX "" SUFFIX ".${MATLAB_LIBRARY_WIN_EXTENSION}")
install( TARGETS DAFFMEX RUNTIME DESTINATION "matlab" )
