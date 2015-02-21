# - Find a VTK installation or build tree.
# The following variables are set if VTK is found.  If VTK is not
# found, VTK_FOUND is set to false.
#  VTK_FOUND         - Set to true when VTK is found.
#  VTK_USE_FILE      - CMake file to use VTK.
#  VTK_MAJOR_VERSION - The VTK major version number.
#  VTK_MINOR_VERSION - The VTK minor version number
#                       (odd non-release).
#  VTK_BUILD_VERSION - The VTK patch level
#                       (meaningless for odd minor).
#  VTK_INCLUDE_DIRS  - Include directories for VTK
#  VTK_LIBRARY_DIRS  - Link directories for VTK libraries
#  VTK_KITS          - List of VTK kits, in CAPS
#                      (COMMON,IO,) etc.
#  VTK_LANGUAGES     - List of wrapped languages, in CAPS
#                      (TCL, PYHTON,) etc.
# The following cache entries must be set by the user to locate VTK:
#  VTK_DIR  - The directory containing VTKConfig.cmake.
#             This is either the root of the build tree,
#             or the lib/vtk directory.  This is the
#             only cache entry.
# The following variables are set for backward compatibility and
# should not be used in new code:
#  USE_VTK_FILE - The full path to the UseVTK.cmake file.
#                 This is provided for backward
#                 compatibility.  Use VTK_USE_FILE
#                 instead.
#

#=============================================================================
# Copyright 2001-2009 Kitware, Inc.
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

# Assume not found.
set(VTK_FOUND 0)

# Construct consistent error messages for use below.
set(VTK_DIR_DESCRIPTION "directory containing VTKConfig.cmake.  This is either the root of the build tree, or PREFIX/lib/vtk for an installation.")
set(VTK_DIR_MESSAGE "VTK not found.  Set the VTK_DIR cmake cache entry to the ${VTK_DIR_DESCRIPTION}")

# Use the Config mode of the find_package() command to find VTKConfig.
# If this succeeds (possibly because VTK_DIR is already set), the
# command will have already loaded VTKConfig.cmake and set VTK_FOUND.
if(NOT VTK_FOUND)
  find_package(VTK QUIET NO_MODULE)
endif()

#-----------------------------------------------------------------------------
if(VTK_FOUND)
  # Set USE_VTK_FILE for backward-compatibility.
  set(USE_VTK_FILE ${VTK_USE_FILE})
else()
  # VTK not found, explain to the user how to specify its location.
  if(VTK_FIND_REQUIRED)
    message(FATAL_ERROR ${VTK_DIR_MESSAGE})
  else()
    if(NOT VTK_FIND_QUIETLY)
      message(STATUS ${VTK_DIR_MESSAGE})
    endif()
  endif()
endif()
