cmake_minimum_required( VERSION 2.9 )

project( OpenDAFF )

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
	"src/QDAFFViewer.cpp"
	"src/QDAFFViewerWindow.cpp"
	"src/QDAFFViewerWindow.h"
	"src/QDAFFHeaderGroupBox.h"
	"src/QDAFFMetadataTable.h"
	"src/QDAFFPropertiesTable.h"
	"src/QDAFFVTKWidget.h"
	"src/QDAFFVTKWidget.cpp"
    "src/QDAFF2DPlot.h"
    "src/QDAFF2DPlot.cpp"
    "src/QDAFFDialogExport2DPlot.cpp"
    "src/QDAFFDialogExport2DPlot.h"
    "ui/QDAFFViewerWindow.ui"
    "ui/QDAFFDialogExport2DPlot.ui"
	)
	
# Qt UI form compiler
qt5_wrap_ui( "ui_QDAFFViewerWindow.h" "ui/QDAFFViewerWindow.ui" ) 
qt5_wrap_ui( "ui_QDAFFDialogExport2DPlot.h" "ui/QDAFFDialogExport2DPlot.ui" )
include_directories( "${CMAKE_BINARY_DIR}/apps/viewer" ) # Gt UI form compiler generates files into this folder

# Qt resources
qt5_add_resources( OPENDAFF_DAFFVIEWER_RESOURCES "res/QDAFFViewer.qrc" )

add_executable( DAFFViewer ${OPENDAFF_DAFFVIEWER_SOURCES} ${OPENDAFF_DAFFVIEWER_RESOURCES} )
target_link_libraries( DAFFViewer DAFFViz DAFF ${FFTW_LIBRARIES} ${SNDFILE_LIBRARIES} ${VTK_LIBRARIES} Qt5::Widgets Qt5::Svg Qt5::Core )
install( TARGETS DAFFViewer RUNTIME DESTINATION "bin" )

# Set PATH to run DAFFFViewer from Visual Studio
if( MSVC )
	set( USER_FILE DAFFViewer.vcxproj.user )
	set( OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/${USER_FILE} )
	list( APPEND DAFFUSERTEMPLATE_PATHES "${FFTW_BINARY_DIRS}" )
	#list( APPEND DAFFUSERTEMPLATE_PATHES "${CMAKE_SOURCE_DIR}" )
	list( APPEND DAFFUSERTEMPLATE_PATHES "${CMAKE_BINARY_DIR}/Debug" )
	list( APPEND DAFFUSERTEMPLATE_PATHES "${CMAKE_BINARY_DIR}/Release")
	list( APPEND DAFFUSERTEMPLATE_PATHES "${_qt5Core_install_prefix}/bin/" )
	configure_file( DAFFUserTemplate.vcxproj.user.proto ${USER_FILE} @ONLY )
endif( MSVC )
