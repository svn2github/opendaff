# Authors: Jonas Stienen, Frank Wefers

cmake_minimum_required(VERSION 2.8)

# Source for package modules (do not change position of this line!)
set (THIRD_PARTY_DIR ${CMAKE_SOURCE_DIR}/3rdParty CACHE STRING "Directory where dependent 3rd party projects are located")
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${THIRD_PARTY_DIR}/CMakeModules)

include(CMakeDependentOption)

project(DAFF)
set_property (GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_DEBUG_POSTFIX "D")

# --= Packages =--

# TODO: FFTW currently required due to transformer
find_package(doxygen)
find_package(FFTW REQUIRED)
find_package(FOX)
find_package(SndFile)
#set (VTK_DIR "/" THIRD_PARTY_DIR)
find_package(VTK)

if (FFTW_FOUND)
	include_directories(${FFTW_INCLUDE_DIR})
endif (FFTW_FOUND)

if (FOX_FOUND)
	include_directories(${FOX_INCLUDE_DIR})
endif (FOX_FOUND)

if (SNDFILE_LIBRARY)
	include_directories(${SNDFILE_INCLUDE_DIR})
endif (SNDFILE_LIBRARY)

if (VTK_FOUND)
	include(${VTK_USE_FILE})
endif (VTK_FOUND)

# --= Options =--

option(BUILD_DAFFTOOL "Build the DAFF tool" ON)
CMAKE_DEPENDENT_OPTION(BUILD_DAFFVIEWER "Build the DAFF viewer" ON "FFTW_FOUND;FOX_FOUND;VTK_FOUND;" OFF)
CMAKE_DEPENDENT_OPTION(BUILD_DAFFMEX "Build the DAFF Matlab Extension" ON "MATLAB_FOUND;" OFF)
CMAKE_DEPENDENT_OPTION(BUILD_DAFFDOC "Build the DAFF API documentation" ON "DOXYGEN_FOUND;" OFF)
#option (BUILD_DAFFVIEWER "Build the DAFF viewer (requires FFTW,FOX,VTK)" ON) 
#option (BUILD_DAFFDOC "Build the DAFF API documentation (requires doxygen)" CACHE BOOL ON) 

set (BUILD_DAFFVIEWER OFF FORCE)

# --- Libraries

# Includes
include_directories (${CMAKE_SOURCE_DIR}/include)
include_directories (${CMAKE_SOURCE_DIR}/3rdParty/FXVTK2/include)
include_directories (${CMAKE_SOURCE_DIR}/3rdParty/IFX/include)
#include_directories (${FOX_INCLUDE_DIR})
#include_directories (${VTK_INCLUDE_DIRS})
#include_directories (${FFTW_INCLUDE_DIR})

# +--------------------------------------+
# |                                      |
# |    DAFF library                      |
# |                                      |
# +--------------------------------------+

# Include files of the library
set (DAFFLIB_INCLUDE_FILES
	${CMAKE_SOURCE_DIR}/include/DAFF.h
	${CMAKE_SOURCE_DIR}/include/DAFFContent.h
	${CMAKE_SOURCE_DIR}/include/DAFFContentDFT.h
	${CMAKE_SOURCE_DIR}/include/DAFFContentIR.h
	${CMAKE_SOURCE_DIR}/include/DAFFContentMPS.h
	${CMAKE_SOURCE_DIR}/include/DAFFContentMS.h
	${CMAKE_SOURCE_DIR}/include/DAFFContentPS.h
	${CMAKE_SOURCE_DIR}/include/DAFFDefs.h	
	${CMAKE_SOURCE_DIR}/include/DAFFMetadata.h
	${CMAKE_SOURCE_DIR}/include/DAFFProperties.h
	${CMAKE_SOURCE_DIR}/include/DAFFReader.h
	${CMAKE_SOURCE_DIR}/include/DAFFTransformerIR2DFT.h
	${CMAKE_SOURCE_DIR}/include/DAFFUtils.h
)

# Source files of the library
set (DAFFLIB_SOURCE_FILES
	${CMAKE_SOURCE_DIR}/src/DAFFHeader.h
	${CMAKE_SOURCE_DIR}/src/DAFFMetadataImpl.h
	${CMAKE_SOURCE_DIR}/src/DAFFMetadataImpl.cpp
	${CMAKE_SOURCE_DIR}/src/DAFFPropertiesImpl.h
	${CMAKE_SOURCE_DIR}/src/DAFFReader.cpp
	${CMAKE_SOURCE_DIR}/src/DAFFReaderImpl.h
	${CMAKE_SOURCE_DIR}/src/DAFFReaderImpl.cpp
	${CMAKE_SOURCE_DIR}/src/DAFFSCTransform.h
	${CMAKE_SOURCE_DIR}/src/DAFFSCTransform.cpp
	${CMAKE_SOURCE_DIR}/src/DAFFTransformerIR2DFT.cpp
	${CMAKE_SOURCE_DIR}/src/DAFFUtils.cpp
	${CMAKE_SOURCE_DIR}/src/Utils.h
	${CMAKE_SOURCE_DIR}/src/Utils.cpp
)

if (WIN32)
	# On Windows, add the DLL main function
	set (DAFFLIB_SOURCE_FILES ${DAFFLIB_SOURCE_FILES} ${CMAKE_SOURCE_DIR}/src/platforms/windows/DAFFDLLMain.cpp)
endif (WIN32)

set (DAFFLIB_FILES ${DAFFLIB_INCLUDE_FILES} ${DAFFLIB_SOURCE_FILES})
source_group( "" FILES ${DAFFLIB_FILES})

# Es wird festgelegt, dass es sich um eine gemeinsam genutzt Bibliothek
# handelt. Um eine statische Bibliothek zu bekommen, wird
# SHARED durch STATIC ersetzt.
add_library (DAFF SHARED ${DAFFLIB_FILES})
set_property(TARGET DAFF PROPERTY FOLDER "Libraries")

target_include_directories(DAFF PUBLIC
	${CMAKE_SOURCE_DIR}/include
	${CMAKE_SOURCE_DIR}/src
)

target_link_libraries (DAFF ${FFTW_LIBRARY})


if (WIN32)
	# Call the shared library DAFFDLL not DAFF
	set_target_properties(DAFF PROPERTIES OUTPUT_NAME DAFFDLL)

	# Shared library export conventions
	target_compile_definitions(DAFF PUBLIC $<$<CONFIG:Debug>:DAFF_DLL>)
	target_compile_definitions(DAFF PUBLIC $<$<CONFIG:Release>:DAFF_DLL>)
	target_compile_definitions(DAFF PUBLIC $<$<CONFIG:Debug>:DAFF_DLL_EXPORTS>)
	target_compile_definitions(DAFF PUBLIC $<$<CONFIG:Release>:DAFF_DLL_EXPORTS>)
	
	# Disable CRT warnings for unsafe functions under Win32
	target_compile_definitions(DAFF PRIVATE $<$<CONFIG:Debug>:_CRT_SECURE_NO_WARNINGS>)
	target_compile_definitions(DAFF PRIVATE $<$<CONFIG:Release>:_CRT_SECURE_NO_WARNINGS>)
else (WIN32)
	set_property(TARGET DAFF PROPERTY OUTPUT_NAME daff)
endif (WIN32)

install (FILES ${DAFFLIB_INCLUDE_FILES} DESTINATION include)
install (TARGETS DAFF
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
)


# +--------------------------------------+
# |                                      |
# |    DAFFViz library                   |
# |                                      |
# +--------------------------------------+

# Include files of the visulization library
set (DAFFVIZLIB_INCLUDE_FILES
	${CMAKE_SOURCE_DIR}/include/daffviz/FXVTK2BalloonPlot.h
	${CMAKE_SOURCE_DIR}/include/daffviz/FXVTK2CarpetPlot.h
	${CMAKE_SOURCE_DIR}/include/daffviz/IFXDAFFDialogBox.h
	${CMAKE_SOURCE_DIR}/include/daffviz/IFXDAFFDialogBoxRes.h
	${CMAKE_SOURCE_DIR}/include/daffviz/IFXDAFFFrame.h
)

# Source files of the visulization library
set (DAFFVIZLIB_SOURCE_FILES
	${CMAKE_SOURCE_DIR}/src/daffviz/FXVTK2BalloonPlot.cpp
	${CMAKE_SOURCE_DIR}/src/daffviz/FXVTK2CarpetPlot.cpp
	${CMAKE_SOURCE_DIR}/src/daffviz/IFXDAFFDialogBox.cpp
	${CMAKE_SOURCE_DIR}/src/daffviz/IFXDAFFFrame.cpp
	${CMAKE_SOURCE_DIR}/src/daffviz/IFXFancyTable.h
	${CMAKE_SOURCE_DIR}/src/daffviz/IFXFancyTable.cpp
)

set (DAFFVIZLIB_FILES ${DAFFVIZLIB_INCLUDE_FILES} ${DAFFVIZLIB_SOURCE_FILES})
source_group( "" FILES ${DAFFVIZLIB_FILES})

add_library (DAFFViz STATIC ${DAFFVIZLIB_FILES})
set_property(TARGET DAFFViz PROPERTY FOLDER "Libraries")

# TODO: Überarbeiten
target_include_directories(DAFFViz PUBLIC 
	${CMAKE_SOURCE_DIR}/src
	${CMAKE_SOURCE_DIR}/src/daffviz
	${THIRD_PARTY_DIR}/vtk-5.6/include/x64
	${THIRD_PARTY_DIR}/fox-1.6/include
	${THIRD_PARTY_DIR}/fftw3/include
)
# Deps: FXVTK2D.lib;IFXD.lib;FOXD-1.6.lib;libjpegd.lib;libpngd.lib;zlibd.lib;libfftw3f-3.lib;vtkCommon.lib;vtkexpat.lib;vtkFiltering.lib;vtkfreetype.lib;vtkftgl.lib;vtkGraphics.lib;vtkHybrid.lib;vtkIO.lib;vtkjpeg.lib;vtkpng.lib;vtkRendering.lib;vtksys.lib;vtktiff.lib;vtkzlib.lib

if (WIN32)
	# FOX import declarations
	target_compile_definitions(DAFFViz PUBLIC $<$<CONFIG:Debug>:FOXDLL>)
	target_compile_definitions(DAFFViz PUBLIC $<$<CONFIG:Release>:FOXDLL>)
else (WIN32)
	set_property(TARGET DAFFViz PROPERTY OUTPUT_NAME daffviz)
endif (WIN32)

install (FILES ${DAFFVIZ_INCLUDE_FILES} DESTINATION include)
install (TARGETS DAFFViz
	ARCHIVE DESTINATION lib
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
)

# +--------------------------------------+
# |                                      |
# |    FXVTK2 library                    |
# |                                      |
# +--------------------------------------+

# Include files of the FXVTK2 library
set (FXVTK2_INCLUDE_FILES
	${CMAKE_SOURCE_DIR}/3rdParty/FXVTK2/include/FXVTK2.h
	${CMAKE_SOURCE_DIR}/3rdParty/FXVTK2/include/FXVTK2Arrow.h
	${CMAKE_SOURCE_DIR}/3rdParty/FXVTK2/include/FXVTK2CartesianCoordinateAssistant.h
	${CMAKE_SOURCE_DIR}/3rdParty/FXVTK2/include/FXVTK2Frame.h
	${CMAKE_SOURCE_DIR}/3rdParty/FXVTK2/include/FXVTK2GlobalLock.h
	${CMAKE_SOURCE_DIR}/3rdParty/FXVTK2/include/FXVTK2Grid.h
	${CMAKE_SOURCE_DIR}/3rdParty/FXVTK2/include/FXVTK2Label.h
	${CMAKE_SOURCE_DIR}/3rdParty/FXVTK2/include/FXVTK2Line.h
	${CMAKE_SOURCE_DIR}/3rdParty/FXVTK2/include/FXVTK2Plane.h
	${CMAKE_SOURCE_DIR}/3rdParty/FXVTK2/include/FXVTK2SGNode.h
	${CMAKE_SOURCE_DIR}/3rdParty/FXVTK2/include/FXVTK2Sphere.h
	${CMAKE_SOURCE_DIR}/3rdParty/FXVTK2/include/FXVTK2SphericalCoordinateAssistant.h
)

# Source files of the FXVTK2 library
set (FXVTK2_SOURCE_FILES
	${CMAKE_SOURCE_DIR}/3rdParty/FXVTK2/src/FXVTK2Arrow.cpp
	${CMAKE_SOURCE_DIR}/3rdParty/FXVTK2/src/FXVTK2CartesianCoordinateAssistant.cpp
	${CMAKE_SOURCE_DIR}/3rdParty/FXVTK2/src/FXVTK2Frame.cpp
	${CMAKE_SOURCE_DIR}/3rdParty/FXVTK2/src/FXVTK2GlobalLock.cpp
	${CMAKE_SOURCE_DIR}/3rdParty/FXVTK2/src/FXVTK2Grid.cpp
	${CMAKE_SOURCE_DIR}/3rdParty/FXVTK2/src/FXVTK2Label.cpp
	${CMAKE_SOURCE_DIR}/3rdParty/FXVTK2/src/FXVTK2Line.cpp
	${CMAKE_SOURCE_DIR}/3rdParty/FXVTK2/src/FXVTK2Plane.cpp
	${CMAKE_SOURCE_DIR}/3rdParty/FXVTK2/src/FXVTK2SGNode.cpp
	${CMAKE_SOURCE_DIR}/3rdParty/FXVTK2/src/FXVTK2Sphere.cpp
	${CMAKE_SOURCE_DIR}/3rdParty/FXVTK2/src/FXVTK2SphericalCoordinateAssistant.cpp
)

set (FXVTK2_FILES ${FXVTK2_INCLUDE_FILES} ${FXVTK2_SOURCE_FILES})
source_group( "" FILES ${FXVTK2_FILES})

add_library (FXVTK2 STATIC ${FXVTK2_FILES})
set_property(TARGET FXVTK2 PROPERTY FOLDER "Libraries")

# TODO: Überarbeiten
target_include_directories(FXVTK2 PUBLIC 
	${THIRD_PARTY_DIR}/fox-1.6/include
	${THIRD_PARTY_DIR}/vtk-5.6/include/x64
)

if (WIN32)
	# FOX import declarations
	target_compile_definitions(FXVTK2 PUBLIC $<$<CONFIG:Debug>:FOXDLL>)
	target_compile_definitions(FXVTK2 PUBLIC $<$<CONFIG:Release>:FOXDLL>)
else (WIN32)
	set_property(TARGET FXVTK2 PROPERTY OUTPUT_NAME fxvtk2)
endif (WIN32)

install (FILES ${FXVTK2_INCLUDE_FILES} DESTINATION include)
install (TARGETS FXVTK2
	ARCHIVE DESTINATION lib
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
)


# +--------------------------------------+
# |                                      |
# |    DAFFTool binary                   |
# |                                      |
# +--------------------------------------+

set (GETOPT_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/3rdParty/getopt")
set (GETOPT_SOURCE_DIR "${CMAKE_SOURCE_DIR}/3rdParty/getopt")

# Source files of the tool
set (DAFFTOOL_SOURCE_FILES
	${CMAKE_SOURCE_DIR}/utils/DAFFTool/src/DAFFTool.cpp
	${CMAKE_SOURCE_DIR}/utils/DAFFTool/src/Export.h
	${CMAKE_SOURCE_DIR}/utils/DAFFTool/src/Export.cpp
	
	# Getopt (TODO: Remove on POSIX platforms)
	${GETOPT_INCLUDE_DIR}/getopt.h
	${GETOPT_SOURCE_DIR}/getopt.c
)

set(DAFFTOOL_FILES ${DAFFTOOL_SOURCE_FILES})
if (MSVC)
	# Visual Studio resource file
	set(DAFFTOOL_FILES ${DAFFTOOL_FILES} ${CMAKE_SOURCE_DIR}/utils/DAFFTool/res/DAFFTool.rc)
endif (MSVC)
source_group( "" FILES ${DAFF_FILES})

add_executable (DAFFTool ${DAFFTOOL_FILES})
set_property(TARGET DAFFTool PROPERTY FOLDER "Utils")
target_include_directories (DAFFTool PUBLIC ${SNDFILE_INCLUDE_DIR} ${GETOPT_INCLUDE_DIR})
target_link_libraries (DAFFTool DAFF ${SNDFILE_LIBRARY})

# TODO: Unless under Windows, use lower case names for binaries and libs
#set(myname get_property(TARGET DAFFTool PROPERTY OUTPUT_NAME ))
#message(${myname})
if(NOT WIN32)
	set_property(TARGET DAFFTool PROPERTY OUTPUT_NAME dafftool)
endif(NOT WIN32)

install (TARGETS DAFFTool
	RUNTIME DESTINATION bin
	COMPONENT ${FFTW_BINARY}
)

# +--------------------------------------+
# |                                      |
# |    DAFFViewer binary                 |
# |                                      |
# +--------------------------------------+

# TODO: Src => src
# Source files of the tool
set (DAFFVIEWER_SOURCE_FILES
	${CMAKE_SOURCE_DIR}/utils/DAFFViewer/Src/AboutDialog.h
	${CMAKE_SOURCE_DIR}/utils/DAFFViewer/Src/AboutDialog.cpp
	${CMAKE_SOURCE_DIR}/utils/DAFFViewer/Src/Globals.h
	${CMAKE_SOURCE_DIR}/utils/DAFFViewer/Src/InfoDialog.h
	${CMAKE_SOURCE_DIR}/utils/DAFFViewer/Src/InfoDialog.cpp
	${CMAKE_SOURCE_DIR}/utils/DAFFViewer/Src/Main.cpp
	${CMAKE_SOURCE_DIR}/utils/DAFFViewer/Src/MainWindow.h
	${CMAKE_SOURCE_DIR}/utils/DAFFViewer/Src/MainWindow.cpp
)

set(DAFFVIEWER_FILES ${DAFFVIEWER_SOURCE_FILES})
if (MSVC)
	# Visual Studio resource file
	set(DAFFVIEWER_FILES ${DAFFVIEWER_FILES} ${CMAKE_SOURCE_DIR}/utils/DAFFViewer/Res/DAFFViewer.rc)
endif (MSVC)
source_group( "" FILES ${DAFFVIEWER_FILES})

add_executable (DAFFViewer ${DAFFVIEWER_FILES})
set_property(TARGET DAFFViewer PROPERTY FOLDER "Utils")
target_include_directories (DAFFViewer PUBLIC
	${CMAKE_SOURCE_DIR}/src
	${CMAKE_SOURCE_DIR}/src/DAFFViz
)
target_link_libraries (DAFFViewer DAFF DAFFViz FXVTK2 ${FFTW_LIBRARY} ${FOX_LIBRARY} ${SNDFILE_LIBRARY} ${VTK_LIBRARIES})

# Es sollen auch einige Dateien installiert werden, z.B. Bilder oder Übersetzungen
# install(FILES img/icon.png DESTINATION img)

#add_subdirectory (${CMAKE_SOURCE_DIR}/utils/DAFFTool)
#add_subdirectory (${CMAKE_SOURCE_DIR}/utils/DAFFViewer)

if (WIN32)
	# FOX import declarations
	target_compile_definitions(DAFFViewer PUBLIC $<$<CONFIG:Debug>:FOXDLL>)
	target_compile_definitions(DAFFViewer PUBLIC $<$<CONFIG:Release>:FOXDLL>)
else (WIN32)
	set_property(TARGET DAFFViewer PROPERTY OUTPUT_NAME daffviewer)
endif(WIN32)

install (TARGETS DAFFViewer
	RUNTIME DESTINATION bin
)