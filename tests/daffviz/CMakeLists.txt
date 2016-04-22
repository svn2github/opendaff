cmake_minimum_required( VERSION 2.9 )

find_package( VTK REQUIRED )

include( "${VTK_USE_FILE}" )

add_executable( DAFFVizTest main.cpp VTKDAFFVizWindow.h VTKDAFFVizWindow.cpp )
target_link_libraries( DAFFVizTest DAFFViz DAFF )
install( TARGETS DAFFVizTest RUNTIME DESTINATION "bin" )

set_property( TARGET DAFFVizTest PROPERTY FOLDER "DAFFTests" )