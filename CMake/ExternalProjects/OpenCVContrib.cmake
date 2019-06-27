#/*============================================================================
#
#  SKSURGERYOPENCVCPP: Image-guided surgery functions, in C++, using OpenCV.
#
#  Copyright (c) University College London (UCL). All rights reserved.
#
#  This software is distributed WITHOUT ANY WARRANTY; without even
#  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
#  PURPOSE.
#
#  See LICENSE.txt in the top level directory for details.
#
#============================================================================*/

#-----------------------------------------------------------------------------
# OpenCV contrib gets built whenever OpenCV is selected.
# This cmake file just downloads it, so it wont take too long.
#-----------------------------------------------------------------------------
if(NOT BUILD_OpenCV)
  return()
endif()

# Sanity checks
if(DEFINED OpenCVContrib_DIR AND NOT EXISTS ${OpenCVContrib_DIR})
  message(FATAL_ERROR "OpenCVContrib_DIR variable is defined but corresponds to non-existing directory")
endif()

set(version "${OPENCV_VERSION}")
set(location "https://github.com/opencv/opencv_contrib.git")
mpMacroDefineExternalProjectVariables(OpenCVContrib ${version} ${location})
set(proj_DEPENDENCIES )

if(NOT DEFINED OpenCVContrib_DIR)

  ExternalProject_Add(${proj}
    LIST_SEPARATOR ^^
    PREFIX ${proj_CONFIG}
    SOURCE_DIR ${proj_SOURCE}
    BINARY_DIR ${proj_BUILD}
    INSTALL_DIR ${proj_INSTALL}
    GIT_REPOSITORY ${proj_LOCATION}
    GIT_TAG ${proj_VERSION}
    UPDATE_COMMAND ${GIT_EXECUTABLE} checkout ${proj_VERSION}
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    CMAKE_GENERATOR ${gen}
    CMAKE_ARGS
      ${EP_COMMON_ARGS}
    CMAKE_CACHE_ARGS
      ${EP_COMMON_CACHE_ARGS}
    CMAKE_CACHE_DEFAULT_ARGS
      ${EP_COMMON_CACHE_DEFAULT_ARGS}
    DEPENDS ${proj_DEPENDENCIES}
  )

  set(OpenCVContrib_DIR ${proj_SOURCE})
  mitkFunctionInstallExternalCMakeProject(${proj})

  message("SuperBuild loading OpenCVContrib from ${OpenCVContrib_DIR}")

else()

  mitkMacroEmptyExternalProject(${proj} "${proj_DEPENDENCIES}")

endif()

