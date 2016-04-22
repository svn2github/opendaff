cmake_minimum_required( VERSION 2.9 )

set( OPENDAFF_THIRD_PARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty" CACHE STRING "Directory where dependent 3rd party projects are located" )
list( APPEND CMAKE_MODULE_PATH "${OPENDAFF_THIRD_PARTY_DIR}/CMakeModules" )

project( OpenDAFF )
set( CMAKE_DEBUG_POSTFIX "D" )

find_package( doxygen QUIET )
find_package( FFTW )
find_package( SndFile )
find_package( VTK QUIET )

if( NOT DEFINED OPENDAFF_BUILD_DAFFLIBS_SHARED )
	set( OPENDAFF_BUILD_DAFFLIBS_SHARED OFF CACHE BOOL "Build OpenDAFF as a shared library" )
endif(  NOT DEFINED OPENDAFF_BUILD_DAFFLIBS_SHARED )

if( NOT DEFINED OPENDAFF_WITH_DAFFVIZ )
	set( OPENDAFF_WITH_DAFFVIZ OFF CACHE BOOL "Build OpenDAFF visualization library (requires third party libraries)" )
endif(  NOT DEFINED OPENDAFF_WITH_DAFFVIZ )

if( NOT DEFINED OPENDAFF_BUILD_DAFF_TOOL )
	set( OPENDAFF_BUILD_DAFF_TOOL OFF CACHE BOOL "Build OpenDAFF with DAFFTool application (requires third party libraries)" )
endif(  NOT DEFINED OPENDAFF_BUILD_DAFF_TOOL )

if( NOT DEFINED OPENDAFF_BUILD_DAFF_VIEWER )
	set( OPENDAFF_BUILD_DAFF_VIEWER OFF CACHE BOOL "Build OpenDAFF with DAFFViewer application (requires third party libraries)" )
endif( NOT DEFINED OPENDAFF_BUILD_DAFF_VIEWER )

if( NOT DEFINED OPENDAFF_BUILD_DAFF_TESTS )
	set( OPENDAFF_BUILD_DAFF_TESTS OFF CACHE BOOL "Build OpenDAFF with tests" )
endif( NOT DEFINED OPENDAFF_BUILD_DAFF_TESTS )

if( NOT DEFINED OPENDAFF_BUILD_DAFF_DOCUMENTATION )
	set( OPENDAFF_BUILD_DAFF_DOCUMENTATION ${DOXYGEN_FOUND} CACHE BOOL "Build OpenDAFF with Doxygen documentation (requires third party libraries)" )
endif( NOT DEFINED OPENDAFF_BUILD_DAFF_DOCUMENTATION )

if( FFTW_FOUND )
	include_directories( "${FFTW_INCLUDE_DIR}" )
endif( FFTW_FOUND )

if( FOX_FOUND )
	include_directories( "${FOX_INCLUDE_DIR}" )
endif( FOX_FOUND )

if( SNDFILE_FOUND )
	include_directories( "${SNDFILE_INCLUDE_DIR}" )
endif( SNDFILE_FOUND )

if( VTK_FOUND )
	include( "${VTK_USE_FILE}" )
else( VTK_FOUND )
	message( STATUS "VTK not found, which is required for DAFFViz library" )
endif( VTK_FOUND )

include_directories( "include" )

set( OPENDAFF_DAFFLIB_HEADER_FILES
	"include/DAFF.h"
	"include/DAFFContent.h"
	"include/DAFFContentDFT.h"
	"include/DAFFContentIR.h"
	"include/DAFFContentMPS.h"
	"include/DAFFContentMS.h"
	"include/DAFFContentPS.h"
	"include/DAFFDefs.h"	
	"include/DAFFMetadata.h"
	"include/DAFFProperties.h"
	"include/DAFFReader.h"
	"include/DAFFUtils.h"
)

set( OPENDAFF_DAFFLIB_SOURCE_FILES
	"src/DAFFHeader.h"
	"src/DAFFMetadataImpl.h"
	"src/DAFFMetadataImpl.cpp"
	"src/DAFFPropertiesImpl.h"
	"src/DAFFReader.cpp"
	"src/DAFFReaderImpl.h"
	"src/DAFFReaderImpl.cpp"
	"src/DAFFSCTransform.h"
	"src/DAFFSCTransform.cpp"
	"src/DAFFUtils.cpp"
	"src/Utils.h"
	"src/Utils.cpp"
)

if( FFTW_FOUND )
	list( APPEND OPENDAFF_DAFFLIB_HEADER_FILES "include/DAFFTransformerIR2DFT.h" )
	list( APPEND OPENDAFF_DAFFLIB_SOURCE_FILES "src/DAFFTransformerIR2DFT.cpp" )
	add_definitions( -DOPENDAFF_WITH_FFTW )
endif( FFTW_FOUND )

set( OPENDAFF_DAFFLIB_FILES ${OPENDAFF_DAFFLIB_HEADER_FILES} ${OPENDAFF_DAFFLIB_SOURCE_FILES} )

if( OPENDAFF_BUILD_DAFFLIBS_SHARED )
	if( WIN32 )
		list( APPEND OPENDAFF_DAFFLIB_FILES "src/platforms/windows/DAFFDLLMain" )
	endif( WIN32 )

	if( MSVC )
		# Deactivate warning C4251 on inconsistent DLL bindings of string and vector
		set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4251" )
	endif( MSVC )

	add_definitions( -DDAFF_DLL -DDAFF_DLL_EXPORTS )
	add_library( DAFF SHARED "${OPENDAFF_DAFFLIB_FILES}" )
	set_property( TARGET DAFF PROPERTY FOLDER "DAFFLibs" )
	message( "Building shared OpenDAFF library" )

	if( FFTW_FOUND )
		target_link_libraries( DAFF ${FFTW_LIBRARIES} )
		link_directories( "${FFTW_LIBRARY_DIRS}" )
		message( "link lib folder ${FFTW_LIBRARY_DIRS}" )
	endif( FFTW_FOUND )

else( OPENDAFF_BUILD_DAFFLIBS_SHARED )
	add_library( DAFF STATIC "${OPENDAFF_DAFFLIB_FILES}" )
	message( "Building static OpenDAFF library" )
	set_property( TARGET DAFF PROPERTY FOLDER "DAFFLibs" )
endif( OPENDAFF_BUILD_DAFFLIBS_SHARED )


install( TARGETS DAFF RUNTIME DESTINATION "bin" LIBRARY DESTINATION "lib" ARCHIVE DESTINATION "lib" )
install( FILES ${OPENDAFF_DAFFLIB_HEADER_FILES} DESTINATION "include" )


set( OPENDAFF_DAFFVIZ_HEADER_FILES
	"include/daffviz/DAFFViz.h"
	"include/daffviz/DAFFVizArrow.h"
	"include/daffviz/DAFFVizBalloonPlot.h"
	"include/daffviz/DAFFVizCarpetPlot.h"
	"include/daffviz/DAFFVizCartesianCoordinateAssistant.h"
	"include/daffviz/DAFFVizGlobalLock.h"
	"include/daffviz/DAFFVizGrid.h"
	"include/daffviz/DAFFVizLabel.h"
	"include/daffviz/DAFFVizLine.h"
	"include/daffviz/DAFFVizPlane.h"
	"include/daffviz/DAFFVizSGNode.h"
	"include/daffviz/DAFFVizSphere.h"
	"include/daffviz/DAFFVizSphericalCoordinateAssistant.h"
)

set( OPENDAFF_DAFFVIZ_SOURCE_FILES
	"src/daffviz/DAFFVizArrow.cpp"
	"src/daffviz/DAFFVizBalloonPlot.cpp"
	"src/daffviz/DAFFVizCarpetPlot.cpp"
	"src/daffviz/DAFFVizCarpetPlot.cpp"
	"src/daffviz/DAFFVizCartesianCoordinateAssistant.cpp"
	"src/daffviz/DAFFVizGlobalLock.cpp"
	"src/daffviz/DAFFVizGrid.cpp"
	"src/daffviz/DAFFVizLabel.cpp"
	"src/daffviz/DAFFVizLine.cpp"
	"src/daffviz/DAFFVizPlane.cpp"
	"src/daffviz/DAFFVizSGNode.cpp"
	"src/daffviz/DAFFVizSphere.cpp"
	"src/daffviz/DAFFVizSphericalCoordinateAssistant.cpp"
)

set( DAFFVIZLIB_FILES ${OPENDAFF_DAFFVIZ_HEADER_FILES} ${OPENDAFF_DAFFVIZ_SOURCE_FILES} )
	
if( OPENDAFF_WITH_DAFFVIZ )

	# DAFFViz uses C++11 mutex
	if( CMAKE_COMPILER_IS_GNUCXX )
		add_definitions(-std=gnu++0x)
	endif()
	
	if( OPENDAFF_BUILD_DAFFLIBS_SHARED )
		add_definitions( -DDAFF_DLL -DDAFF_DLL_EXPORTS )
		link_directories( "${VTK_LIBRARY_DIRS}")
		add_library( DAFFViz SHARED "${DAFFVIZLIB_FILES}" )
		target_link_libraries( DAFFViz ${VTK_LIBRARIES} )
		set_property( TARGET DAFFViz PROPERTY FOLDER "DAFFLibs" )
	else( OPENDAFF_BUILD_DAFFLIBS_SHARED )
		add_library( DAFFViz STATIC "${DAFFVIZLIB_FILES}" )
		set_property( TARGET DAFFViz PROPERTY FOLDER "DAFFLibs" )
	endif( OPENDAFF_BUILD_DAFFLIBS_SHARED )

	install( FILES ${OPENDAFF_DAFFVIZ_HEADER_FILES} DESTINATION "include/daffviz" )
	install( TARGETS DAFFViz
		ARCHIVE DESTINATION "lib"
		RUNTIME DESTINATION "bin"
		LIBRARY DESTINATION "lib"
	)
	
	
endif( OPENDAFF_WITH_DAFFVIZ )

if( OPENDAFF_BUILD_DAFF_TOOL )
	add_subdirectory( "utils/DAFFTool" )
endif( OPENDAFF_BUILD_DAFF_TOOL )

if( OPENDAFF_BUILD_DAFF_VIEWER AND OPENDAFF_WITH_DAFFVIZ )
	add_subdirectory( "utils/DAFFViewer" )
endif( OPENDAFF_BUILD_DAFF_VIEWER AND OPENDAFF_WITH_DAFFVIZ )

if( OPENDAFF_BUILD_DAFF_TESTS )
	add_subdirectory( "tests" )
endif( OPENDAFF_BUILD_DAFF_TESTS )
