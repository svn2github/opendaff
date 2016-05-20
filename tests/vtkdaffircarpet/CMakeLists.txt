cmake_minimum_required( VERSION 2.9 )

find_package( VTK REQUIRED )

include( "${VTK_USE_FILE}" )

add_executable( DAFFVizVTKDAFFIRCarpet main.cpp )
target_link_libraries( DAFFVizVTKDAFFIRCarpet DAFF ${VTK_LIBRARIES} )

install( TARGETS DAFFVizVTKDAFFIRCarpet RUNTIME DESTINATION "bin" )

set_property( TARGET DAFFVizVTKDAFFIRCarpet PROPERTY FOLDER "DAFFTests" )

# Set PATH to run from VS2012
if( MSVC )
	set( USER_FILE DAFFVizVTKDAFFIRCarpet.vcxproj.user )
	set( OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/${USER_FILE} )
	list( APPEND DAFFUSERTEMPLATE_PATHES "${VTK_RUNTIME_LIBRARY_DIRS}" )
	list( APPEND DAFFUSERTEMPLATE_PATHES "${_qt5Core_install_prefix}/bin/" )
	configure_file( DAFFUserTemplate.vcxproj.user.proto ${USER_FILE} @ONLY )
endif( MSVC )
