cmake_minimum_required( VERSION 2.9 )

add_executable( DAFFFileBufferTest DAFFFileBufferTest.cpp )
target_link_libraries( DAFFFileBufferTest DAFF )
install( TARGETS DAFFFileBufferTest RUNTIME DESTINATION "bin" )
set_property( TARGET DAFFFileBufferTest PROPERTY FOLDER "DAFFTests" )

