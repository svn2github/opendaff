cmake_minimum_required( VERSION 2.8 )

add_library( DAFFCSWrapper SHARED "DAFFCSWrapper.h" "DAFFCSWrapper.cpp" )
add_definitions( -DDAFF_DLL )
add_definitions( -DDAFFCS_EXPORTS )
set_property( TARGET DAFFCSWrapper PROPERTY FOLDER "DAFFLibs" )
target_link_libraries( DAFFCSWrapper DAFF )

install( TARGETS DAFFCSWrapper RUNTIME DESTINATION "bin" LIBRARY DESTINATION "lib" ARCHIVE DESTINATION "lib" )
