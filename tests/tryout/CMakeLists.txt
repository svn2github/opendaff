cmake_minimum_required( VERSION 2.9 )

add_executable( EndianessTest EndianessTest.cpp )
target_link_libraries( EndianessTest DAFF )
install( TARGETS EndianessTest RUNTIME DESTINATION "bin" )
set_property( TARGET EndianessTest PROPERTY FOLDER "DAFFTests" )

add_executable( MetadataImplTest MetadataImplTest.cpp )
target_link_libraries( MetadataImplTest DAFF )
install( TARGETS MetadataImplTest RUNTIME DESTINATION "bin" )
set_property( TARGET MetadataImplTest PROPERTY FOLDER "DAFFTests" )

add_executable( ReaderTest ReaderTest.cpp )
target_link_libraries( ReaderTest DAFF )
install( TARGETS ReaderTest RUNTIME DESTINATION "bin" )
set_property( TARGET ReaderTest PROPERTY FOLDER "DAFFTests" )

add_executable( SCTransformTest SCTransformTest.cpp )
target_link_libraries( SCTransformTest DAFF )
install( TARGETS SCTransformTest RUNTIME DESTINATION "bin" )
set_property( TARGET SCTransformTest PROPERTY FOLDER "DAFFTests" )
