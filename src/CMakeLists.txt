add_library(sksurgeryopencvcpp STATIC
  exception.cpp
  maths.cpp
  validate.cpp
  triangulate.cpp
  stoyanov2010.cpp
  masking.cpp
  dot_detection.cpp
)

target_include_directories(sksurgeryopencvcpp
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

target_link_libraries(sksurgeryopencvcpp
  PUBLIC
    opencv_core
    opencv_imgproc
    opencv_calib3d
    opencv_features2d
    opencv_stereo
)

# OpenMP (optional)
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
  target_link_libraries(sksurgeryopencvcpp PRIVATE OpenMP::OpenMP_CXX)
endif()
