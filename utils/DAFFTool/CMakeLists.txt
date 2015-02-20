# Author: Jonas Stienen

project(DAFFTool)

cmake_minimum_required(VERSION 2.6)

# Define 3rdparty path
set (THIRD_PARTY_DIR ${CMAKE_SOURCE_DIR}/../../3rdparty)

# If cmake does not find a required package drop the Find***.cmake here
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${THIRD_PARTY_DIR}/CMakeModules)

# Packages
#find_package (SNDFILE REQUIRED)

# --- Libraries

# Includes
include_directories (${CMAKE_SOURCE_DIR}/../../include)
#include_directories (${SNDFILE_INCLUDE_DIR})

set (DAFFToolSrc
	${CMAKE_SOURCE_DIR}/src/DAFFTool.cpp
	${CMAKE_SOURCE_DIR}/src/Export.cpp
	)

# Linker
link_directories (${CMAKE_SOURCE_DIR}/../..)
add_executable (DAFFTool ${DAFFToolSrc})
target_link_libraries (DAFFTool daff)
target_link_libraries (DAFFTool sndfile)

# Install (might need superuser rights)
install (TARGETS DAFFTool DESTINATION bin)