# Author: Jonas Stienen

project(DAFF)

cmake_minimum_required(VERSION 2.6)

# Define 3rdparty path
set (THIRD_PARTY_DIR ${CMAKE_SOURCE_DIR}/3rdparty)

# If cmake does not find FOX or VTK place the Find***.cmake files here
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${THIRD_PARTY_DIR}/CMakeModules)

# Packages
find_package (FOX)
find_package (VTK)
find_package (FFTW)

# --- Libraries

# Includes
include_directories (${CMAKE_SOURCE_DIR}/include)
include_directories (${CMAKE_SOURCE_DIR}/3rdparty/FXVTK2/include)
include_directories (${CMAKE_SOURCE_DIR}/3rdparty/IFX/include)
include_directories (${FOX_INCLUDE_DIR})
include_directories (${VTK_INCLUDE_DIRS})
include_directories (${FFTW_INCLUDE_DIR})

set (DAFFSrc
	${CMAKE_SOURCE_DIR}/src/DAFFMetadataImpl.cpp
	${CMAKE_SOURCE_DIR}/src/DAFFReader.cpp
	${CMAKE_SOURCE_DIR}/src/DAFFReaderImpl.cpp
	${CMAKE_SOURCE_DIR}/src/DAFFSCTransform.cpp
	${CMAKE_SOURCE_DIR}/src/DAFFUtils.cpp
	${CMAKE_SOURCE_DIR}/src/Utils.cpp
	)
	
set (DAFFVIZSrc
	${CMAKE_SOURCE_DIR}/src/FXVTK2BalloonPlot.cpp
	${CMAKE_SOURCE_DIR}/src/FXVTK2CarpetPlot.cpp
	${CMAKE_SOURCE_DIR}/src/DAFFTransformerIR2DFT.cpp
	${CMAKE_SOURCE_DIR}/src/IFXDAFFDialogBox.cpp
	${CMAKE_SOURCE_DIR}/src/IFXDAFFFrame.cpp
	)

# Linker
add_library (daff ${DAFFSrc})

# Disable CRT warnings for unsafe functions under Win32
if (WIN32)
	set_property(TARGET daff PROPERTY COMPILE_DEFINITIONS_DEBUG "_CRT_SECURE_NO_WARNINGS") 
	set_property(TARGET daff PROPERTY COMPILE_DEFINITIONS_RELEASE "_CRT_SECURE_NO_WARNINGS") 
endif (WIN32)

if (${FOX_FOUND} STREQUAL "YES" AND ${VTK_FOUND} AND ${FFTW_FOUND} STREQUAL "TRUE")
  add_library (daffviz ${DAFFVIZSrc})
  set (libdaffviz 1)
endif (${FOX_FOUND} STREQUAL "YES" AND ${VTK_FOUND} AND ${FFTW_FOUND} STREQUAL "TRUE")

# Install
install (TARGETS daff DESTINATION lib)

set (DAFFInc
	${CMAKE_SOURCE_DIR}/include/DAFF.h
	${CMAKE_SOURCE_DIR}/include/DAFFContentDFT.h
	${CMAKE_SOURCE_DIR}/include/DAFFContentMPS.h
	${CMAKE_SOURCE_DIR}/include/DAFFContentPS.h
	${CMAKE_SOURCE_DIR}/include/DAFFMetadata.h
	${CMAKE_SOURCE_DIR}/include/DAFFPropertiesImpl.h
	${CMAKE_SOURCE_DIR}/include/DAFFTransformerIR2DFT.h
	${CMAKE_SOURCE_DIR}/include/DAFFContent.h
	${CMAKE_SOURCE_DIR}/include/DAFFContentIR.h
	${CMAKE_SOURCE_DIR}/include/DAFFContentMS.h
	${CMAKE_SOURCE_DIR}/include/DAFFDefs.h
	${CMAKE_SOURCE_DIR}/include/DAFFProperties.h
	${CMAKE_SOURCE_DIR}/include/DAFFReader.h
	${CMAKE_SOURCE_DIR}/include/DAFFUtils.h
	)

install (FILES ${DAFFInc} DESTINATION include)

if (${libdaffviz})

  install (TARGETS daffviz DESTINATION lib)

  set (DAFFVIZInc
	  ${CMAKE_SOURCE_DIR}/include/daffviz/FXVTK2BalloonPlot.h
	  ${CMAKE_SOURCE_DIR}/include/daffviz/FXVTK2CarpetPlot.h
	  ${CMAKE_SOURCE_DIR}/include/daffviz/IFXDAFFDialogBox.h
	  ${CMAKE_SOURCE_DIR}/include/daffviz/IFXDAFFFrame.h
	  )

  install (FILES ${DAFFVIZInc} DESTINATION include/daffviz)

endif (${libdaffviz})

# Es sollen auch einige Dateien installiert werden, z.B. Bilder oder Übersetzungen
# install(FILES img/icon.png DESTINATION img)

#add_subdirectory (${CMAKE_SOURCE_DIR}/utils/DAFFTool)
#add_subdirectory (${CMAKE_SOURCE_DIR}/utils/DAFFViewer)
