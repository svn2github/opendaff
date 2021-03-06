cmake_minimum_required( VERSION 2.9 )

# inspired by https://cmake.org/Wiki/VTK/Examples/Cxx/Qt/RenderWindowUISingleInheritance

set( CMAKE_AUTOMOC ON )

find_package( VTK )
find_package( Qt5Widgets )

link_directories( "${VTK_LIBRARY_DIRS}" )

add_executable( DAFFVizQtTest main.cpp DAFFVizQtTestWindow.cpp )
target_link_libraries( DAFFVizQtTest DAFFViz DAFF ${VTK_LIBRARIES} Qt5::Widgets )
install( TARGETS DAFFVizQtTest RUNTIME DESTINATION "bin" )

# Set PATH to run from VS2012
if( MSVC )
	set( USER_FILE DAFFVizQtTest.vcxproj.user )
	set( OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/${USER_FILE} )
	list( APPEND DAFFUSERTEMPLATE_PATHES "${VTK_RUNTIME_LIBRARY_DIRS}" )
	list( APPEND DAFFUSERTEMPLATE_PATHES "${_qt5Core_install_prefix}/bin/" )
	configure_file( DAFFUserTemplate.vcxproj.user.proto ${USER_FILE} @ONLY )
endif( MSVC )
