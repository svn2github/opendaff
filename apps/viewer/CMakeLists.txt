cmake_minimum_required( VERSION 2.9 )

find_package( FFTW REQUIRED )
find_package( SndFile REQUIRED )

include_directories( "${FFTW_INCLUDE_DIR}" )
include_directories( "${SNDFILE_INCLUDE_DIR}" )
set( OPENDAFF_DAFFVIEWER_SOURCES
	"res/REsources.h"
	"src/AboutDialog.cpp"
	"src/AboutDialog.h"
	"src/Globals.h"
	"src/InfoDialog.h"
	"src/AboutDialog.cpp"
	"src/AboutDialog.h"
	)
add_executable( DAFFViewer ${OPENDAFF_DAFFVIEWER_SOURCES} )
target_link_libraries( DAFFViewer DAFF ${FFTW_LIBRARIES} ${SNDFILE_LIBRARIES} )
install( TARGETS DAFFViewer RUNTIME DESTINATION "bin" )

# Set PATH to run from VS2012
if( MSVC )
	set( SYSTEM_NAME $ENV{USERDOMAIN} CACHE STRING SystemName )
	set( USER_NAME $ENV{USERNAME} CACHE STRING UserName )
	set( USER_FILE DAFFViewer.vcxproj.user )
	set( OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/${USER_FILE} )
	list( APPEND DAFFViewer_PATHES "${FFTW_BINARY_DIRS}" )
	list( APPEND DAFFViewer_PATHES "${SNDFILE_BINARY_DIRS}" )
	configure_file( DAFFViewer.vcxproj.user.proto ${USER_FILE} @ONLY )
endif( MSVC )
