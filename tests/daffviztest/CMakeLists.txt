cmake_minimum_required( VERSION 2.9 )

find_package( VTK REQUIRED )

include( "${VTK_USE_FILE}" )

add_executable( DAFFVizTest main.cpp VTKDAFFVizWindow.h VTKDAFFVizWindow.cpp )
target_link_libraries( DAFFVizTest DAFFViz DAFF ${VTK_LIBRARIES} )

install( TARGETS DAFFVizTest RUNTIME DESTINATION "bin" )

set_property( TARGET DAFFVizTest PROPERTY FOLDER "DAFFTests" )

# Set PATH to run from VS2012
if( MSVC )
	set( USER_FILE DAFFVizTest.vcxproj.user )
	set( OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/${USER_FILE} )
	list( APPEND DAFFUSERTEMPLATE_PATHES "${VTK_RUNTIME_LIBRARY_DIRS}" )
	list( APPEND DAFFUSERTEMPLATE_PATHES "${_qt5Core_install_prefix}/bin/" )
	configure_file( DAFFUserTemplate.vcxproj.user.proto ${USER_FILE} @ONLY )
endif( MSVC )
