# Author: Jonas Stienen

project(DAFFViewer)

cmake_minimum_required(VERSION 2.6)

# Define 3rdparty path
set (THIRD_PARTY_DIR ${CMAKE_SOURCE_DIR}/../../3rdparty)

# If cmake does not find a required package drop the Find***.cmake here
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${THIRD_PARTY_DIR}/CMakeModules)

# Packages
find_package (FOX REQUIRED)
find_package (VTK REQUIRED)

# --- Libraries

# Includes
include_directories (${CMAKE_SOURCE_DIR}/../../include)
include_directories (${THIRD_PARTY_DIR}/FXVTK2/include)
include_directories (${THIRD_PARTY_DIR}/IFX/include)
include_directories (${FOX_INCLUDE_DIR})
include_directories (${VTK_INCLUDE_DIRS})

set (DAFFViewerSrc
	${CMAKE_SOURCE_DIR}/Src/AboutDialog.cpp
	${CMAKE_SOURCE_DIR}/Src/InfoDialog.cpp
	${CMAKE_SOURCE_DIR}/Src/Main.cpp
	${CMAKE_SOURCE_DIR}/Src/MainWindow.cpp
	${THIRD_PARTY_DIR}/FXVTK2/src/FXVTK2Frame.cpp
	${THIRD_PARTY_DIR}/FXVTK2/src/FXVTK2Arrow.cpp
	${THIRD_PARTY_DIR}/FXVTK2/src/FXVTK2Frame.cpp
	${THIRD_PARTY_DIR}/FXVTK2/src/FXVTK2Grid.cpp
	${THIRD_PARTY_DIR}/FXVTK2/src/FXVTK2Line.cpp
	${THIRD_PARTY_DIR}/FXVTK2/src/FXVTK2SGNode.cpp
	${THIRD_PARTY_DIR}/FXVTK2/src/FXVTK2SphericalCoordinateAssistant.cpp
	${THIRD_PARTY_DIR}/FXVTK2/src/FXVTK2CartesianCoordinateAssistant.cpp
	${THIRD_PARTY_DIR}/FXVTK2/src/FXVTK2GlobalLock.cpp
	${THIRD_PARTY_DIR}/FXVTK2/src/FXVTK2Label.cpp
	${THIRD_PARTY_DIR}/FXVTK2/src/FXVTK2Plane.cpp
	${THIRD_PARTY_DIR}/FXVTK2/src/FXVTK2Sphere.cpp
	${THIRD_PARTY_DIR}/IFX/src/IFXFancyTable.cpp
	)

# Linker
link_directories (${CMAKE_SOURCE_DIR}/../..)
add_executable (DAFFViewer ${DAFFViewerSrc})
target_link_libraries (DAFFViewer daff daffviz)
target_link_libraries (DAFFViewer FOX-1.6)
target_link_libraries (DAFFViewer fftw3f)
target_link_libraries (DAFFViewer vtkCommon vtkFiltering vtkftgl vtkGraphics vtkHybrid vtkIO vtkRendering)

# Install (might need superuser rights)
install (TARGETS DAFFViewer DESTINATION bin)