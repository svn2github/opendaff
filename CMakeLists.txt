# Author: Jonas Stienen

project(DAFF)

cmake_minimum_required(VERSION 2.8)
include(CMakeDependentOption)

set_property (GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_DEBUG_POSTFIX "d")

# 3rdParty folder [ITA specific]
set (THIRD_PARTY_DIR ${CMAKE_SOURCE_DIR}/../3rdParty)
#set (THIRD_PARTY_DIR D:/ITA/Arbeitsbereich/3rdParty)

# If cmake does not find FOX or VTK place the Find***.cmake files here
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${THIRD_PARTY_DIR}/CMakeModules)

# --= Packages =--

SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/3rdParty/CMakeScripts)

find_package(FOX)
find_package(SndFile)

message("${FOX_FOUND}")

#[ITA specific]
# find_package(VTK)
# if (!VTK_FOUND)
	# set (VTK_DIR "${THIRD_PARTY_DIR}/vtk-5.6")
	# find_package(VTK)
# endif (!VTK_FOUND)

# if (VTK_FOUND)
	# message(STATUS "Found VTK in directory: ${VTK_DIR}")
# endif (VTK_FOUND)
# --= Options =--

option(BUILD_DAFFTOOL "Build the DAFF tool" ON)
CMAKE_DEPENDENT_OPTION(BUILD_DAFFVIEWER "Build the DAFF viewer" ON "FFTW_FOUND;FOX_FOUND;VTK_FOUND;" OFF)
CMAKE_DEPENDENT_OPTION(BUILD_DAFFMEX "Build the DAFF Matlab Extension" ON "MATLAB_FOUND;" OFF)
CMAKE_DEPENDENT_OPTION(BUILD_DAFFMEX "Build the DAFF API documentation" ON "DOXYGEN_FOUND;" OFF)
#option (BUILD_DAFFVIEWER "Build the DAFF viewer (requires FFTW,FOX,VTK)" ON) 
#option (BUILD_DAFFDOC "Build the DAFF API documentation (requires doxygen)" CACHE BOOL ON) 

set (BUILD_DAFFVIEWER OFF FORCE)

# --- Libraries

# Includes
include_directories (${CMAKE_SOURCE_DIR}/include)
include_directories (${CMAKE_SOURCE_DIR}/3rdparty/FXVTK2/include)
include_directories (${CMAKE_SOURCE_DIR}/3rdparty/IFX/include)
#include_directories (${FOX_INCLUDE_DIR})
#include_directories (${VTK_INCLUDE_DIRS})
#include_directories (${FFTW_INCLUDE_DIR})

# --= DAFF library =--

# Include files of the library
set (DAFF_INCLUDE_FILES
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
	${CMAKE_SOURCE_DIR}/include/DAFFUtils.h
)

# Source files of the library
set (DAFF_SOURCE_FILES
	${CMAKE_SOURCE_DIR}/src/DAFFHeader.h
	${CMAKE_SOURCE_DIR}/src/DAFFMetadataImpl.h
	${CMAKE_SOURCE_DIR}/src/DAFFMetadataImpl.cpp
	${CMAKE_SOURCE_DIR}/src/DAFFPropertiesImpl.h
	${CMAKE_SOURCE_DIR}/src/DAFFReader.cpp
	${CMAKE_SOURCE_DIR}/src/DAFFReaderImpl.h
	${CMAKE_SOURCE_DIR}/src/DAFFReaderImpl.cpp
	${CMAKE_SOURCE_DIR}/src/DAFFSCTransform.h
	${CMAKE_SOURCE_DIR}/src/DAFFSCTransform.cpp
	#${CMAKE_SOURCE_DIR}/src/DAFFTransformerIR2DFT.h
	#${CMAKE_SOURCE_DIR}/src/DAFFTransformerIR2DFT.cpp
	${CMAKE_SOURCE_DIR}/src/DAFFUtils.cpp
	${CMAKE_SOURCE_DIR}/src/Utils.h
	${CMAKE_SOURCE_DIR}/src/Utils.cpp
)

set (DAFF_FILES ${DAFF_INCLUDE_FILES} ${DAFF_SOURCE_FILES})
source_group( "" FILES ${DAFF_FILES})

# Es wird festgelegt, dass es sich um eine gemeinsam genutzt Bibliothek
# handelt. Um eine statische Bibliothek zu bekommen, wird
# SHARED durch STATIC ersetzt.
add_library (daff SHARED ${DAFF_FILES})
set_property(TARGET daff PROPERTY FOLDER "Libraries")

if (WIN32)
	# Shared library export conventions
	target_compile_definitions(daff PUBLIC $<$<CONFIG:Debug>:DAFF_DLL>)
	target_compile_definitions(daff PUBLIC $<$<CONFIG:Debug>:DAFF_DLL>)
	target_compile_definitions(daff PUBLIC $<$<CONFIG:Debug>:DAFF_DLL_EXPORTS>)
	target_compile_definitions(daff PUBLIC $<$<CONFIG:Debug>:DAFF_DLL_EXPORTS>)
	
	# Disable CRT warnings for unsafe functions under Win32
	target_compile_definitions(daff PRIVATE $<$<CONFIG:Debug>:_CRT_SECURE_NO_WARNINGS>)
	target_compile_definitions(daff PRIVATE $<$<CONFIG:Release>:_CRT_SECURE_NO_WARNINGS>)
endif (WIN32)

install (FILES ${DAFF_INCLUDE_FILES} DESTINATION include)
install (TARGETS daff RUNTIME DESTINATION bin LIBRARY DESTINATION lib)

# --= DAFF tool =--

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

add_executable (dafftool ${DAFFTOOL_FILES})
set_property(TARGET dafftool PROPERTY FOLDER "Utils")
target_include_directories (dafftool PUBLIC ${SNDFILE_INCLUDE_DIR} ${GETOPT_INCLUDE_DIR})
target_link_libraries (dafftool daff ${SNDFILE_LIBRARY})

# --= DAFFVIZ library =--
	
# set (DAFFVIZSrc
	# ${CMAKE_SOURCE_DIR}/src/FXVTK2BalloonPlot.cpp
	# ${CMAKE_SOURCE_DIR}/src/FXVTK2CarpetPlot.cpp
	# ${CMAKE_SOURCE_DIR}/src/DAFFTransformerIR2DFT.cpp
	# ${CMAKE_SOURCE_DIR}/src/IFXDAFFDialogBox.cpp
	# ${CMAKE_SOURCE_DIR}/src/IFXDAFFFrame.cpp
	# )

# #if (${FOX_FOUND} STREQUAL "YES" AND ${VTK_FOUND} AND ${FFTW_FOUND} STREQUAL "TRUE")
# #  add_library (daffviz ${DAFFVIZSrc})
# #  set (libdaffviz 1)
# #endif (${FOX_FOUND} STREQUAL "YES" AND ${VTK_FOUND} AND ${FFTW_FOUND} STREQUAL "TRUE")

  # install (TARGETS daffviz DESTINATION lib)

  # set (DAFFVIZInc
	  # ${CMAKE_SOURCE_DIR}/include/daffviz/FXVTK2BalloonPlot.h
	  # ${CMAKE_SOURCE_DIR}/include/daffviz/FXVTK2CarpetPlot.h
	  # ${CMAKE_SOURCE_DIR}/include/daffviz/IFXDAFFDialogBox.h
	  # ${CMAKE_SOURCE_DIR}/include/daffviz/IFXDAFFFrame.h
	  # )

  # install (FILES ${DAFFVIZInc} DESTINATION include/daffviz)

# endif (${libdaffviz})

# Es sollen auch einige Dateien installiert werden, z.B. Bilder oder Übersetzungen
# install(FILES img/icon.png DESTINATION img)

#add_subdirectory (${CMAKE_SOURCE_DIR}/utils/DAFFTool)
#add_subdirectory (${CMAKE_SOURCE_DIR}/utils/DAFFViewer)
