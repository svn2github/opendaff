cmake_minimum_required( VERSION 2.9 )

set( OPENDAFF_THIRD_PARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty" CACHE STRING "Directory where dependent 3rd party projects are located" )
list( APPEND CMAKE_MODULE_PATH "${OPENDAFF_THIRD_PARTY_DIR}/CMakeModules" )

project( OpenDAFF )
set( CMAKE_DEBUG_POSTFIX "D" )

find_package( doxygen )
find_package( FFTW )
find_package( FOX )
find_package( SndFile )
find_package( VTK )

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
	include_directories( "${VTK_INCLUDE_DIR}" )
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
	add_definitions( -DOPENDAFF_WITH_FFWT )
endif( FFTW_FOUND )

set( OPENDAFF_DAFFLIB_FILES ${OPENDAFF_DAFFLIB_HEADER_FILES} ${OPENDAFF_DAFFLIB_SOURCE_FILES} )

if( OPENDAFF_BUILD_DAFFLIBS_SHARED )
	if( WIN32 )
		list( APPEND OPENDAFF_DAFFLIB_FILES "src/platforms/windows/DAFFDLLMain" )
	endif( WIN32 )
	add_definitions( -DDAFF_DLL -DDAFF_DLL_EXPORTS )
	add_library( DAFF SHARED "${OPENDAFF_DAFFLIB_FILES}" )
	message( "Building shared OpenDAFF library" )

	if( FFTW_FOUND )
		target_link_libraries( DAFF ${FFTW_LIBRARIES} )
		link_directories( "${FFTW_LIBRARY_DIRS}" )
		message( "link lib folder ${FFTW_LIBRARY_DIRS}" )
	endif( FFTW_FOUND )

else( OPENDAFF_BUILD_DAFFLIBS_SHARED )
	add_library( DAFF "${OPENDAFF_DAFFLIB_FILES}" )
	message( "Building static OpenDAFF library" )
endif( OPENDAFF_BUILD_DAFFLIBS_SHARED )


install( TARGETS DAFF RUNTIME DESTINATION "bin" LIBRARY DESTINATION "lib" ARCHIVE DESTINATION "lib" )
install( FILES ${OPENDAFF_DAFFLIB_HEADER_FILES} DESTINATION "include" )


set( OPENDAFF_DAFFVIZ_HEADER_FILES
	"include/daffviz/FXVTK2BalloonPlot.h"
	"include/daffviz/FXVTK2CarpetPlot.h"
	"include/daffviz/IFXDAFFDialogBox.h"
	"include/daffviz/IFXDAFFDialogBoxRes.h"
	"include/daffviz/IFXDAFFFrame.h"
)

set( OPENDAFF_DAFFVIZ_SOURCE_FILES
	"src/daffviz/FXVTK2BalloonPlot.cpp"
	"src/daffviz/FXVTK2CarpetPlot.cpp"
	"src/daffviz/IFXDAFFDialogBox.cpp"
	"src/daffviz/IFXDAFFFrame.cpp"
	"src/daffviz/IFXFancyTable.h"
	"src/daffviz/IFXFancyTable.cpp"
)

set( DAFFVIZLIB_FILES ${OPENDAFF_DAFFVIZ_HEADER_FILES} ${OPENDAFF_DAFFVIZ_SOURCE_FILES} )
	
if( OPENDAFF_WITH_DAFFVIZ )

	source_group( "" FILES ${DAFFVIZLIB_FILES})

	if( OPENDAFF_BUILD_DAFFLIBS_SHARED )
		if( WIN32 )
			list( APPEND OPENDAFF_DAFFLIB_FILES "src/platforms/windows/DAFFDLLMain" )
		endif( WIN32 )
		add_definitions( -DDAFF_DLL -DDAFF_DLL_EXPORTS )
		add_library( DAFFViz SHARED "${OPENDAFF_DAFFVIZ_SOURCE_FILES}" )
	else( OPENDAFF_BUILD_DAFFLIBS_SHARED )
		add_library( DAFFViz "${OPENDAFF_DAFFVIZ_SOURCE_FILES}" )
	endif( OPENDAFF_BUILD_DAFFLIBS_SHARED )

	install( FILES ${OPENDAFF_DAFFVIZ_HEADER_FILES} DESTINATION "include" )
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
