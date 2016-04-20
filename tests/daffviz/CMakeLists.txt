cmake_minimum_required( VERSION 2.9 )

find_package( VTK )

link_directories( "${VTK_LIBRARY_DIRS}" )

add_executable( DAFFVizTest main.cpp )
target_link_libraries( DAFFVizTest DAFFViz DAFF ${VTK_LIBRARIES} )
install( TARGETS DAFFVizTest RUNTIME DESTINATION "bin" )
