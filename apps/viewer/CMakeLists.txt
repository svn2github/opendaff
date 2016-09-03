cmake_minimum_required( VERSION 2.9 )

set( CMAKE_AUTOMOC ON )

find_package( VTK REQUIRED )
find_package( FFTW REQUIRED )
find_package( SndFile REQUIRED )
find_package( Qt5 REQUIRED COMPONENTS Core Svg Gui Widgets )
find_package( Qt5 COMPONENTS Svg )

include( ${VTK_USE_FILE} )

link_directories( "${VTK_LIBRARY_DIRS}" )

include_directories( "${FFTW_INCLUDE_DIR}" )
include_directories( "${SNDFILE_INCLUDE_DIR}" )

set( OPENDAFF_DAFFVIEWER_SOURCES
	"res/DAFFViewerRes.h"
	"src/DAFFViewer.cpp"
	"src/QDAFFViewerWindow.cpp"
	"src/QDAFFViewerWindow.h"
	"src/QDAFFHeaderGroupBox.h"
	"src/QDAFFMetadataTable.h"
	"src/QDAFFPropertiesTable.h"
	"src/QDAFFVTKWidget.h"
	"src/QDAFFVTKWidget.cpp"
	"src/QDAFF2DPlot.h"
	"src/QDAFF2DPlot.cpp"
	)

# Add ui include for VS2012
if( MSVC )
	list( APPEND OPENDAFF_DAFFVIEWER_SOURCES "ui/ui_QDAFFViewerWindow.h" )
	include_directories( "ui" )
endif( MSVC )

add_executable( DAFFViewer ${OPENDAFF_DAFFVIEWER_SOURCES} )
target_link_libraries( DAFFViewer DAFFViz DAFF ${FFTW_LIBRARIES} ${SNDFILE_LIBRARIES} ${VTK_LIBRARIES} Qt5::Widgets Qt5::Svg Qt5::Core )
install( TARGETS DAFFViewer RUNTIME DESTINATION "bin" )

# Set PATH to run from VS2012
if( MSVC )
	set( USER_FILE DAFFViewer.vcxproj.user )
	set( OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/${USER_FILE} )
	list( APPEND DAFFUSERTEMPLATE_PATHES "${FFTW_BINARY_DIRS}" )
	list( APPEND DAFFUSERTEMPLATE_PATHES "${SNDFILE_BINARY_DIRS}" )
	list( APPEND DAFFUSERTEMPLATE_PATHES "${_qt5Core_install_prefix}/bin/" )
	configure_file( DAFFUserTemplate.vcxproj.user.proto ${USER_FILE} @ONLY )
endif( MSVC )
