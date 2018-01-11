cmake_minimum_required( VERSION 2.9 )

project( OpenDAFF )

set( CMAKE_AUTOMOC ON )

find_package( VTK REQUIRED )
find_package( FFTW REQUIRED )
find_package( SndFile REQUIRED )
find_package( Qt5 REQUIRED COMPONENTS Core Svg Gui Widgets )
find_package( Qt5 COMPONENTS Svg )

include( ${VTK_USE_FILE} )
set( VTK_BINARY_DIRS "" CACHE STRING "VTK binary dirs (to locate DLLs when running DAFFViewer out of MSVC)" )

link_directories( "${VTK_LIBRARY_DIRS}" )

include_directories( "${FFTW_INCLUDE_DIR}" )
include_directories( "${SNDFILE_INCLUDE_DIR}" )

set( OPENDAFF_DAFFVIEWER_SOURCES
	"src/QDAFFViewer.cpp"
	"src/QDAFFViewerWindow.cpp"
	"src/QDAFFViewerWindow.h"
	"src/QDAFFHeaderGroupBox.h"
	"src/QDAFFContentTable.h"
	"src/QDAFFMetadataTable.h"
	"src/QDAFFPropertiesTable.h"
	"src/QDAFFVTKWidget.h"
	"src/QDAFFVTKWidget.cpp"
    "src/QDAFF2DPlot.h"
    "src/QDAFF2DPlot.cpp"
    "src/QDAFFDialogExport2DPlot.cpp"
    "src/QDAFFDialogExport2DPlot.h"
    "src/QDAFFDialogExport3DPlotImage.cpp"
    "src/QDAFFDialogExport3DPlotImage.h"
    "src/QDAFFDialogExport3DPlotImageSeries.cpp"
    "src/QDAFFDialogExport3DPlotImageSeries.h"
    "ui/QDAFFViewerWindow.ui"
    "ui/QDAFFDialogExport2DPlot.ui"
    "ui/QDAFFDialogExport3DPlotImage.ui"
    "ui/QDAFFDialogExport3DPlotImageSeries.ui"
	)
	
# Qt UI form compiler
qt5_wrap_ui( "ui_QDAFFViewerWindow.h" "ui/QDAFFViewerWindow.ui" ) 
qt5_wrap_ui( "ui_QDAFFDialogExport2DPlot.h" "ui/QDAFFDialogExport2DPlot.ui" )
qt5_wrap_ui( "ui_QDAFFDialogExport3DPlotImage.h" "ui/QDAFFDialogExport3DPlotImage.ui" )
qt5_wrap_ui( "ui_QDAFFDialogExport3DPlotImageSeries.h" "ui/QDAFFDialogExport3DPlotImageSeries.ui" )
include_directories( "${CMAKE_BINARY_DIR}/apps/viewer" ) # Gt UI form compiler generates files into this folder

# Qt resources
qt5_add_resources( OPENDAFF_DAFFVIEWER_RESOURCES "res/QDAFFViewer.qrc" OPTIONS -compress 3 )

add_executable( DAFFViewer ${OPENDAFF_DAFFVIEWER_SOURCES} ${OPENDAFF_DAFFVIEWER_RESOURCES} )
target_link_libraries( DAFFViewer DAFFViz DAFF ${FFTW_LIBRARIES} ${SNDFILE_LIBRARIES} ${VTK_LIBRARIES} Qt5::Widgets Qt5::Svg Qt5::Core )
install( TARGETS DAFFViewer RUNTIME DESTINATION "bin" )
set_property( TARGET DAFFViewer PROPERTY WIN32_EXECUTABLE true )


# Set PATH to run DAFFFViewer from Visual Studio
if( MSVC )
	set( USER_FILE DAFFViewer.vcxproj.user )
	set( OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/${USER_FILE} )
	list( APPEND DAFFUSERTEMPLATE_PATHES "${FFTW_BINARY_DIRS}" )
	#list( APPEND DAFFUSERTEMPLATE_PATHES "${CMAKE_SOURCE_DIR}" )
	list( APPEND DAFFUSERTEMPLATE_PATHES "${CMAKE_BINARY_DIR}/Debug" )
	list( APPEND DAFFUSERTEMPLATE_PATHES "${CMAKE_BINARY_DIR}/Release")
	list( APPEND DAFFUSERTEMPLATE_PATHES "${_qt5Core_install_prefix}/bin/" )
	list( APPEND DAFFUSERTEMPLATE_PATHES "${VTK_CMAKE_DIR}/../../../bin" ) # humble attempt to locate VTK DLLs, leave it to user otherwise
	list( APPEND DAFFUSERTEMPLATE_PATHES "${VTK_BINARY_DIRS}" ) # user defined path
	configure_file( DAFFUserTemplate.vcxproj.user.proto ${USER_FILE} @ONLY )
endif( )

# Qt Creator: Open CMakeLists.txt as project and add directories to external libs, i.e.
#	-DCMAKE_MODULE_PATH=C:\dev\ExternalLibs\OpenDAFF\opendaff-v1.7\opendaff_withcred\cmake -DVTK_DIR=C:\dev\ExternalLibs\VTK\VTK-7.0\win32-x64.vc12\lib\cmake\vtk-7.0 -DFFTW_DIR=C:\dev\ExternalLibs\fftw\fftw-3.3.3\win32-x64 -DSNDFILE_DIR=C:\dev\ExternalLibs\libsndfile\libsndfile-1.0.25\win32-x64
