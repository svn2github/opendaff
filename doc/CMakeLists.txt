cmake_minimum_required( VERSION 2.9 )

list( APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" )

find_package( doxygen REQUIRED )

configure_file( doxyfile.in doxyfile @ONLY )
configure_file( doxygen.header.html.in doxygen.header.html @ONLY )
configure_file( doxygen.footer.html.in doxygen.footer.html @ONLY )

add_custom_target( DAFFDoc "${DOXYGEN_EXECUTABLE}" doxyfile 
	WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
	COMMENT "Generating OpenDAFF API documentation with Doxygen" VERBATIM
	)

install( DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/html" DESTINATION "share/doc" )