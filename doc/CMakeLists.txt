cmake_minimum_required( VERSION 3.10 )

list( APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" )

#cmake_policy( SET CMP0057 NEW )
find_package( doxygen REQUIRED )

configure_file( doxyfile.in doxyfile @ONLY )
configure_file( doxygen.footer.html.in doxygen.footer.html @ONLY )

add_custom_target( DAFFDoc "${DOXYGEN_EXECUTABLE}" doxyfile 
	WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
	COMMENT "Generating OpenDAFF API documentation with Doxygen" VERBATIM
	)

install( DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/html" DESTINATION "share/doc" )
