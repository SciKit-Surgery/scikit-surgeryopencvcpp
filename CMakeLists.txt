cmake_minimum_required(VERSION 3.15)
project(scikit-surgeryopencvcpp VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

option(BUILD_TESTING "Build C++ unit tests" OFF)

# ===========================================================================
# Dependencies via FetchContent
# ===========================================================================
include(FetchContent)

# --- OpenCV (static, minimal modules) ---
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(BUILD_LIST "core,imgproc,calib3d,features2d,imgcodecs,stereo" CACHE STRING "" FORCE)
set(BUILD_opencv_apps OFF CACHE BOOL "" FORCE)
set(BUILD_opencv_python3 OFF CACHE BOOL "" FORCE)
set(BUILD_opencv_python2 OFF CACHE BOOL "" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(BUILD_PERF_TESTS OFF CACHE BOOL "" FORCE)
set(BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(BUILD_opencv_highgui OFF CACHE BOOL "" FORCE)
set(BUILD_opencv_videoio OFF CACHE BOOL "" FORCE)
set(BUILD_opencv_video OFF CACHE BOOL "" FORCE)
set(BUILD_opencv_ml OFF CACHE BOOL "" FORCE)
set(BUILD_opencv_dnn OFF CACHE BOOL "" FORCE)
set(BUILD_opencv_flann OFF CACHE BOOL "" FORCE)
set(BUILD_opencv_photo OFF CACHE BOOL "" FORCE)
set(BUILD_opencv_stitching OFF CACHE BOOL "" FORCE)
set(BUILD_opencv_objdetect OFF CACHE BOOL "" FORCE)
set(BUILD_opencv_gapi OFF CACHE BOOL "" FORCE)
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
set(WITH_IPP OFF CACHE BOOL "" FORCE)
set(WITH_LAPACK OFF CACHE BOOL "" FORCE)
set(OPENCV_GENERATE_PKGCONFIG OFF CACHE BOOL "" FORCE)
set(OPENCV_ENABLE_NONFREE OFF CACHE BOOL "" FORCE)
set(CV_DISABLE_OPTIMIZATION OFF CACHE BOOL "" FORCE)
set(ENABLE_PRECOMPILED_HEADERS OFF CACHE BOOL "" FORCE)

# opencv_contrib (for stereo module)
FetchContent_Declare(
  opencv_contrib
  GIT_REPOSITORY https://github.com/opencv/opencv_contrib.git
  GIT_TAG        4.10.0
  GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(opencv_contrib)
set(OPENCV_EXTRA_MODULES_PATH "${opencv_contrib_SOURCE_DIR}/modules" CACHE PATH "" FORCE)

FetchContent_Declare(
  opencv
  GIT_REPOSITORY https://github.com/opencv/opencv.git
  GIT_TAG        4.10.0
  GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(opencv)

# --- pybind11 ---
FetchContent_Declare(
  pybind11
  GIT_REPOSITORY https://github.com/pybind/pybind11.git
  GIT_TAG        v2.13.6
  GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(pybind11)

# ===========================================================================
# Library: sksurgeryopencvcpp (static)
# ===========================================================================
add_subdirectory(src)

# ===========================================================================
# Python module
# ===========================================================================
add_subdirectory(src/python)

# ===========================================================================
# Tests
# ===========================================================================
if(BUILD_TESTING)
  enable_testing()
  add_subdirectory(tests)
endif()
