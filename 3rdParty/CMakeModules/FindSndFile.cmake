# - Find SndFile library
# Find the native SndFile includes and library
# This module defines
#  SNDFILE_INCLUDE_DIRS, where to find sndfile.h, Set when
#                        SNDFILE_INCLUDE_DIR is found.
#  SNDFILE_LIBRARIES, libraries to link against to use SndFile.
#  SNDFILE_ROOT_DIR, The base directory to search for SndFile.
#                    This can also be an environment variable.
#  SNDFILE_FOUND, If false, do not try to use SndFile.
#
# also defined, but not for general use are
#  SNDFILE_LIBRARY, where to find the SndFile library.

#=============================================================================
# Copyright 2002-2009 Kitware, Inc.
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)

# Extended by Jonas Stienen and Frank Wefers

# If SNDFILE_ROOT_DIR was defined in the environment, use it.
IF(NOT SNDFILE_ROOT_DIR AND NOT $ENV{SNDFILE_ROOT_DIR} STREQUAL "")
  SET(SNDFILE_ROOT_DIR $ENV{SNDFILE_ROOT_DIR})
ENDIF()

file(TO_CMAKE_PATH "$ENV{ProgramFiles}/Mega-Nerd/libsndfile" SNDFILE_WIN_PATH1) # Windows 32/64
file(TO_CMAKE_PATH "$ENV{ProgramW6432}/Mega-Nerd/libsndfile" SNDFILE_WIN_PATH2) # Windows 64

SET(_sndfile_SEARCH_DIRS
  ${SNDFILE_ROOT_DIR}
  /usr/local
  /sw # Fink
  /opt/local # DarwinPorts
  /opt/csw # Blastwave
  ${SNDFILE_WIN_PATH1}
  ${SNDFILE_WIN_PATH2}
  ${THIRD_PARTY_DIR}/libsndfile
)

FIND_PATH(SNDFILE_INCLUDE_DIR sndfile.h
  HINTS
    ${_sndfile_SEARCH_DIRS}
  PATH_SUFFIXES
    include
)

# Platform sub directories for Visual Studio builds (Win32|x64)
if (MSVC)
	if (CMAKE_CL_64)
		set (SNDFILE_LIBRARY_WIN_PLATFORM x64)
	else (CMAKE_CL_64)
		set (SNDFILE_LIBRARY_WIN_PLATFORM Win32)
	endif (CMAKE_CL_64)
elseif (MSVC)
	set (SNDFILE_LIBRARY_WIN_PLATFORM "")
endif (MSVC)

FIND_LIBRARY(SNDFILE_LIBRARY
	NAMES "sndfile" "libsndfile" "libsndfile-1"
	HINTS ${_sndfile_SEARCH_DIRS}
	PATH_SUFFIXES lib64 lib lib/${SNDFILE_LIBRARY_WIN_PLATFORM}
)

# handle the QUIETLY and REQUIRED arguments and set SNDFILE_FOUND to TRUE if 
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(SndFile DEFAULT_MSG SNDFILE_LIBRARY SNDFILE_INCLUDE_DIR)

IF(SNDFILE_FOUND)
  SET(SNDFILE_LIBRARIES ${SNDFILE_LIBRARY})
  SET(SNDFILE_INCLUDE_DIRS ${SNDFILE_INCLUDE_DIR})

  #message(STATUS "Found SndFile includes: ${SNDFILE_INCLUDE_DIR}")
  message(STATUS "Found SndFile library: ${SNDFILE_LIBRARY}")
ENDIF(SNDFILE_FOUND)

MARK_AS_ADVANCED(
  SNDFILE_INCLUDE_DIR
  SNDFILE_LIBRARY
)
