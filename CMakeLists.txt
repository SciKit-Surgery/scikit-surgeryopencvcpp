cmake_minimum_required(VERSION 3.15)
project(scikit-surgeryopencvcpp VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Use static MSVC runtime on Windows to match static OpenCV
if(MSVC)
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>" CACHE STRING "" FORCE)
endif()

option(BUILD_TESTING "Build C++ unit tests" OFF)
option(BUILD_PYTHON_BINDINGS "Build Python pybind11 module" ON)

# ===========================================================================
# Dependencies via FetchContent
# ===========================================================================
include(FetchContent)

# --- OpenCV (static, minimal modules) ---
# BUILD_LIST is the authoritative way to restrict which modules are built.
# Only modules in this list (and their dependencies) will be configured.
set(BUILD_LIST "core,imgproc,calib3d,features2d,imgcodecs" CACHE STRING "" FORCE)

# Static build, no apps/tests/docs
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(BUILD_opencv_apps OFF CACHE BOOL "" FORCE)
set(BUILD_opencv_python3 OFF CACHE BOOL "" FORCE)
set(BUILD_opencv_python2 OFF CACHE BOOL "" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(BUILD_PERF_TESTS OFF CACHE BOOL "" FORCE)
set(BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(BUILD_JAVA OFF CACHE BOOL "" FORCE)
set(BUILD_opencv_java_bindings_generator OFF CACHE BOOL "" FORCE)
set(BUILD_opencv_python_bindings_generator OFF CACHE BOOL "" FORCE)
set(BUILD_opencv_python_tests OFF CACHE BOOL "" FORCE)
set(BUILD_opencv_ts OFF CACHE BOOL "" FORCE)
set(BUILD_opencv_world OFF CACHE BOOL "" FORCE)
set(OPENCV_PYTHON_SKIP_DETECTION ON CACHE BOOL "" FORCE)

# Disable all optional I/O and hardware backends
set(WITH_FFMPEG OFF CACHE BOOL "" FORCE)
set(WITH_GSTREAMER OFF CACHE BOOL "" FORCE)
set(WITH_V4L OFF CACHE BOOL "" FORCE)
set(WITH_DSHOW OFF CACHE BOOL "" FORCE)
set(WITH_MSMF OFF CACHE BOOL "" FORCE)
set(WITH_AVFOUNDATION OFF CACHE BOOL "" FORCE)
set(WITH_1394 OFF CACHE BOOL "" FORCE)
set(WITH_OPENEXR OFF CACHE BOOL "" FORCE)
set(WITH_JASPER OFF CACHE BOOL "" FORCE)
set(WITH_OPENJPEG OFF CACHE BOOL "" FORCE)
set(WITH_TIFF OFF CACHE BOOL "" FORCE)
set(WITH_WEBP OFF CACHE BOOL "" FORCE)
set(WITH_PROTOBUF OFF CACHE BOOL "" FORCE)
set(WITH_QUIRC OFF CACHE BOOL "" FORCE)
set(WITH_ADE OFF CACHE BOOL "" FORCE)
set(WITH_ITT OFF CACHE BOOL "" FORCE)
set(WITH_EIGEN OFF CACHE BOOL "" FORCE)
set(WITH_OPENCL OFF CACHE BOOL "" FORCE)
set(WITH_OPENCL_SVM OFF CACHE BOOL "" FORCE)
set(WITH_OPENCLAMDFFT OFF CACHE BOOL "" FORCE)
set(WITH_OPENCLAMDBLAS OFF CACHE BOOL "" FORCE)
set(OPENCV_DNN_OPENCL OFF CACHE BOOL "" FORCE)
set(WITH_IPP OFF CACHE BOOL "" FORCE)
set(WITH_LAPACK OFF CACHE BOOL "" FORCE)
set(WITH_OBSENSOR OFF CACHE BOOL "" FORCE)
set(WITH_PNG ON CACHE BOOL "" FORCE)
set(BUILD_PNG ON CACHE BOOL "" FORCE)
set(BUILD_ZLIB ON CACHE BOOL "" FORCE)
set(OPENCV_GENERATE_PKGCONFIG OFF CACHE BOOL "" FORCE)
set(OPENCV_ENABLE_NONFREE OFF CACHE BOOL "" FORCE)
set(CV_DISABLE_OPTIMIZATION OFF CACHE BOOL "" FORCE)
set(CPU_DISPATCH "" CACHE STRING "" FORCE)
set(ENABLE_PRECOMPILED_HEADERS OFF CACHE BOOL "" FORCE)

# Disable OpenCV's install rules to prevent polluting our wheel
set(INSTALL_CREATE_DISTRIB OFF CACHE BOOL "" FORCE)
set(OPENCV_BIN_INSTALL_PATH "opencv_unused" CACHE STRING "" FORCE)
set(OPENCV_LIB_INSTALL_PATH "opencv_unused" CACHE STRING "" FORCE)
set(OPENCV_3P_LIB_INSTALL_PATH "opencv_unused" CACHE STRING "" FORCE)
set(OPENCV_INCLUDE_INSTALL_PATH "opencv_unused" CACHE STRING "" FORCE)
set(OPENCV_OTHER_INSTALL_PATH "opencv_unused" CACHE STRING "" FORCE)
set(OPENCV_LICENSES_INSTALL_PATH "opencv_unused" CACHE STRING "" FORCE)
set(OPENCV_SAMPLES_SRC_INSTALL_PATH "opencv_unused" CACHE STRING "" FORCE)
set(OPENCV_CONFIG_INSTALL_PATH "opencv_unused" CACHE STRING "" FORCE)

FetchContent_Declare(
  opencv
  GIT_REPOSITORY https://github.com/opencv/opencv.git
  GIT_TAG        4.11.0
  GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(opencv)

# --- pybind11 ---
if(BUILD_PYTHON_BINDINGS)
  # Find Python first so pybind11 uses the correct interpreter
  find_package(Python REQUIRED COMPONENTS Interpreter Development.Module NumPy)

  FetchContent_Declare(
    pybind11
    GIT_REPOSITORY https://github.com/pybind/pybind11.git
    GIT_TAG        v2.13.6
    GIT_SHALLOW    TRUE
  )
  FetchContent_MakeAvailable(pybind11)
endif()

# ===========================================================================
# Library: sksurgeryopencvcpp (static)
# ===========================================================================
add_subdirectory(src)

# ===========================================================================
# Python module
# ===========================================================================
if(BUILD_PYTHON_BINDINGS)
  add_subdirectory(src/python)
endif()

# ===========================================================================
# Tests
# ===========================================================================
if(BUILD_TESTING)
  enable_testing()
  add_subdirectory(tests)
endif()
