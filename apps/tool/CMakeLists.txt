cmake_minimum_required( VERSION 2.9 )

find_package( FFTW REQUIRED )
find_package( SndFile REQUIRED )

include_directories( "${FFTW_INCLUDE_DIR}" )
include_directories( "${SNDFILE_INCLUDE_DIR}" )
add_executable( DAFFTool DAFFTool.cpp Export.h Export.cpp getopt/getopt.h getopt/getopt.c )
target_link_libraries( DAFFTool DAFF ${FFTW_LIBRARIES} ${SNDFILE_LIBRARIES} )
install( TARGETS DAFFTool RUNTIME DESTINATION "bin" )

# Set PATH to run from VS2012
if( MSVC )
	set( SYSTEM_NAME $ENV{USERDOMAIN} CACHE STRING SystemName )
	set( USER_NAME $ENV{USERNAME} CACHE STRING UserName )
	set( USER_FILE DAFFTool.vcxproj.user )
	set( OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/${USER_FILE} )
	list( APPEND DAFFTOOL_PATHES "${FFTW_BINARY_DIRS}" )
	list( APPEND DAFFTOOL_PATHES "${SNDFILE_BINARY_DIRS}" )
	configure_file( DAFFTool.vcxproj.user.proto ${USER_FILE} @ONLY )
endif( MSVC )
