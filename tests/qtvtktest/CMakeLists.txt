cmake_minimum_required( VERSION 2.9 )

find_package( VTK REQUIRED )
include( ${VTK_USE_FILE} )

set( CMAKE_AUTOMOC ON )
find_package( Qt5Widgets )

link_directories( "${VTK_LIBRARY_DIRS}" )

add_executable( DAFFVizQtVTKTest main.cpp DAFFVizQtVTKRenderWindow.cpp DAFFVizQtVTKRenderWindow.h DAFFVizQtVTKRenderWindow.ui ui_DAFFVizQtVTKRenderWindow.h )
target_link_libraries( DAFFVizQtVTKTest DAFFViz DAFF ${VTK_LIBRARIES} Qt5::Widgets )
install( TARGETS DAFFVizQtVTKTest RUNTIME DESTINATION "bin" )
