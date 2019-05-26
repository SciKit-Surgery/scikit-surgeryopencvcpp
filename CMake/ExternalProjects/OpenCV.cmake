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
# OpenCV
#-----------------------------------------------------------------------------
if(NOT BUILD_OpenCV)
  return()
endif()

# Sanity checks
if(DEFINED OpenCV_DIR AND NOT EXISTS ${OpenCV_DIR})
  message(FATAL_ERROR "OpenCV_DIR variable is defined but corresponds to non-existing directory")
endif()

set(version "4.1.0")
set(location "https://github.com/opencv/opencv.git")
mpMacroDefineExternalProjectVariables(OpenCV ${version} ${location})
set(proj_DEPENDENCIES )
if (BUILD_VTK)
  list(APPEND proj_DEPENDENCIES VTK)
endif()

if(NOT DEFINED OpenCV_DIR)

  ##############################################################################
  # Module selection logic.
  # When running on travis/appveyor, and building a small library that will
  # have a python interface, you will probably want the smallest build possible.
  # This will promote the idea that a small python extension should be as small
  # as possible, and just provide a few re-usable algorithms.
  ##############################################################################
  set(_additional_options
    -DWITH_QT:BOOL=OFF
    -DWITH_GTK:BOOL=OFF
    -DWITH_EIGEN:BOOL=OFF
    -DWITH_WEBP:BOOL=OFF  # On Mac, with Homebrew, this is read-only which breaks packaging.
    -DWITH_FFMPEG:BOOL=OFF
    -DWITH_OPENMP:BOOL=${OpenMP_FOUND}
    -DWITH_OPENNI:BOOL=OFF
    -DWITH_OPENNI2:BOOL=OFF
    -DWITH_OPENVX:BOOL=OFF
    -DWITH_VTK:BOOL=OFF
    -DENABLE_PRECOMPILED_HEADERS:BOOL=OFF
    -DOPENCV_PYTHON_SKIP_DETECTION:BOOL=ON
  )
  if(WIN32)
    list(APPEND _additional_options
      -DWITH_LAPACK:BOOL=ON
    )
  elseif(APPLE)
    list(APPEND _additional_options
      -DWITH_LAPACK:BOOL=OFF
      -DWITH_IPP:BOOL=OFF
    )
  else()
    list(APPEND _additional_options
      -DWITH_LAPACK:BOOL=ON
      -DWITH_V4L:BOOL=ON
      -DWITH_IPP:BOOL=OFF
    )
    if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -U__STRICT_ANSI__")
    endif()
  endif()

  if(BUILD_Python_Boost OR BUILD_Python_PyBind)

    message("Building minimum OpenCV modules, as you want a Python build.")

    list(APPEND _additional_options
      -DBUILD_opencv_calib3d:BOOL=ON
      -DBUILD_opencv_core:BOOL=ON
      -DBUILD_opencv_imgproc:BOOL=ON
      -DBUILD_opencv_apps:BOOL=OFF
      -DBUILD_opencv_dnn:BOOL=OFF
      -DBUILD_opencv_features2d:BOOL=ON
      -DBUILD_opencv_flann:BOOL=ON
      -DBUILD_opencv_gapi:BOOL=OFF
      -DBUILD_opencv_highgui:BOOL=ON
      -DBUILD_opencv_imgcodecs:BOOL=ON
      -DBUILD_opencv_java_bindings_generator:BOOL=OFF
      -DBUILD_opencv_js:BOOL=OFF
      -DBUILD_opencv_ml:BOOL=OFF
      -DBUILD_opencv_objdetect:BOOL=OFF
      -DBUILD_opencv_photo:BOOL=OFF
      -DBUILD_opencv_python2:BOOL=OFF
      -DBUILD_opencv_python_bindings_generator:BOOL=OFF
      -DBUILD_opencv_stitching:BOOL=OFF
      -DBUILD_opencv_ts:BOOL=OFF
      -DBUILD_opencv_video:BOOL=OFF
      -DBUILD_opencv_videoio:BOOL=ON
      -DBUILD_opencv_world:BOOL=OFF
    )
  else()

    # Otherwise, we will build all default modules,
    # until someone provides more specific config here.

    message("Building mostly default OpenCV modules")

    if(BUILD_VTK)
      list(APPEND _additional_options
        -DWITH_VTK:BOOL=ON
        -DVTK_DIR:PATH=${VTK_DIR}
      )
    endif()
    
  endif()

  set(_cuda_options
    -DWITH_CUDA:BOOL=${SKSURGERYOPENCVCPP_USE_CUDA}
  )
  if(SKSURGERYOPENCVCPP_USE_CUDA)
    list(APPEND _cuda_options
      -DCUDA_TOOLKIT_ROOT_DIR:PATH=${CUDA_TOOLKIT_ROOT_DIR}
      -DCUDA_ARCH_BIN:STRING=${SKSURGERYOPENCVCPP_CUDA_ARCH_BIN}
      -DCUDA_PROPAGATE_HOST_FLAGS:BOOL=OFF
    )
    if(CMAKE_COMPILER_IS_GNUCXX)
      list(APPEND _cuda_options -DCUDA_NVCC_FLAGS:STRING=${SKSURGERYOPENCVCPP_CXX11_FLAG}^^--expt-relaxed-constexpr)
    else()
      list(APPEND _cuda_options -DCUDA_NVCC_FLAGS:STRING=${SKSURGERYOPENCVCPP_CXX11_FLAG})
    endif()
  endif()

  if(CTEST_USE_LAUNCHERS)
    list(APPEND additional_cmake_args
      "-DCMAKE_PROJECT_${proj}_INCLUDE:FILEPATH=${CMAKE_ROOT}/Modules/CTestUseLaunchers.cmake"
    )
  endif()

  ExternalProject_Add(${proj}
    LIST_SEPARATOR ^^
    PREFIX ${proj_CONFIG}
    SOURCE_DIR ${proj_SOURCE}
    BINARY_DIR ${proj_BUILD}
    INSTALL_DIR ${proj_INSTALL}
    GIT_REPOSITORY ${proj_LOCATION}
    GIT_TAG ${proj_VERSION}
    UPDATE_COMMAND ${GIT_EXECUTABLE} checkout ${proj_VERSION}
    CMAKE_GENERATOR ${gen}
    CMAKE_ARGS
      ${EP_COMMON_ARGS}
      -DCMAKE_PREFIX_PATH:PATH=${SKSURGERYOPENCVCPP_PREFIX_PATH}
      -DBUILD_DOCS:BOOL=OFF
      -DBUILD_DOXYGEN_DOCS:BOOL=OFF
      -DBUILD_PERF_TESTS:BOOL=OFF
      ${_additional_options}
      ${_cuda_options}
      "-DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS} ${SKSURGERYOPENCVCPP_CXX11_FLAG} -DVTK_MAJOR_VERSION=6" # Doesn't matter what version, as long as > 5.
      ${additional_cmake_args}
    CMAKE_CACHE_ARGS
      ${EP_COMMON_CACHE_ARGS}
    CMAKE_CACHE_DEFAULT_ARGS
      ${EP_COMMON_CACHE_DEFAULT_ARGS}
    DEPENDS ${proj_DEPENDENCIES}
  )

  set(OpenCV_DIR ${proj_INSTALL})
  set(SKSURGERYOPENCVCPP_PREFIX_PATH ${proj_INSTALL}^^${SKSURGERYOPENCVCPP_PREFIX_PATH})
  mitkFunctionInstallExternalCMakeProject(${proj})

  message("SuperBuild loading OpenCV from ${OpenCV_DIR}")

else()

  mitkMacroEmptyExternalProject(${proj} "${proj_DEPENDENCIES}")

endif()

