cmake_minimum_required( VERSION 2.9 )

find_package( VTK REQUIRED )
include( ${VTK_USE_FILE} )

set( CMAKE_AUTOMOC ON )
find_package( Qt5Widgets )

link_directories( "${VTK_LIBRARY_DIRS}" )

add_executable( DAFFVizQtVTKTest main.cpp DAFFVizQtVTKRenderWindow.cpp DAFFVizQtVTKRenderWindow.h DAFFVizQtVTKRenderWindow.ui ui_DAFFVizQtVTKRenderWindow.h )
target_link_libraries( DAFFVizQtVTKTest DAFFViz DAFF ${VTK_LIBRARIES} Qt5::Widgets )
install( TARGETS DAFFVizQtVTKTest RUNTIME DESTINATION "bin" )

# Set PATH to run from VS2012
if( MSVC )
	set( USER_FILE DAFFVizQtVTKTest.vcxproj.user )
	set( OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/${USER_FILE} )
	list( APPEND DAFFVIZQTVTKTEST_PATHES "${VTK_RUNTIME_LIBRARY_DIRS}" )
	list( APPEND DAFFVIZQTVTKTEST_PATHES "${_qt5Core_install_prefix}/bin/" )
	configure_file( DAFFVizQtVTKTest.vcxproj.user.proto ${USER_FILE} @ONLY )
endif( MSVC )
