cmake_minimum_required( VERSION 2.9 )

set( CMAKE_AUTOMOC ON )

find_package( VTK )
find_package( Qt5Widgets )

link_directories( "${VTK_LIBRARY_DIRS}" )

add_executable( DAFFVizQtTest main.cpp DAFFVizQtTestWindow.cpp )
target_link_libraries( DAFFVizQtTest DAFFViz DAFF ${VTK_LIBRARIES} Qt5::Widgets )
install( TARGETS DAFFVizQtTest RUNTIME DESTINATION "bin" )
