cmake_minimum_required( VERSION 2.9 )

find_package( VTK REQUIRED )
find_package( FFTW REQUIRED )
find_package( SndFile REQUIRED )
find_package( Qt REQUIRED )

include( ${VTK_USE_FILE} )

link_directories( "${VTK_LIBRARY_DIRS}" )

set( CMAKE_AUTOMOC ON )
find_package( Qt5Widgets )

include_directories( "${FFTW_INCLUDE_DIR}" )
include_directories( "${SNDFILE_INCLUDE_DIR}" )

set( OPENDAFF_DAFFVIEWER_SOURCES
	"res/Resources.h"
	"src/AboutDialog.cpp"
	"src/AboutDialog.h"
	"src/Globals.h"
	"src/InfoDialog.h"
	"src/AboutDialog.cpp"
	"src/AboutDialog.h"
	)
add_executable( DAFFViewer ${OPENDAFF_DAFFVIEWER_SOURCES} )
target_link_libraries( DAFFViewer DAFFViz DAFF ${FFTW_LIBRARIES} ${SNDFILE_LIBRARIES} ${VTK_LIBRARIES} Qt5::Widgets )
install( TARGETS DAFFViewer RUNTIME DESTINATION "bin" )

# Set PATH to run from VS2012
if( MSVC )
	set( USER_FILE DAFFViewer.vcxproj.user )
	set( OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/${USER_FILE} )
	list( APPEND DAFFVIEWER_PATHES "${FFTW_BINARY_DIRS}" )
	list( APPEND DAFFVIEWER_PATHES "${SNDFILE_BINARY_DIRS}" )
	configure_file( DAFFViewer.vcxproj.user.proto ${USER_FILE} @ONLY )
endif( MSVC )
