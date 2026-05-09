add_library(sksurgeryopencvcpp STATIC
  exception.cpp
  maths.cpp
  validate.cpp
  triangulate.cpp
  masking.cpp
  dots.cpp
)

target_include_directories(sksurgeryopencvcpp
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${opencv_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${opencv_SOURCE_DIR}/modules/core/include>
    $<BUILD_INTERFACE:${opencv_SOURCE_DIR}/modules/imgproc/include>
    $<BUILD_INTERFACE:${opencv_SOURCE_DIR}/modules/calib3d/include>
    $<BUILD_INTERFACE:${opencv_SOURCE_DIR}/modules/features2d/include>
    $<BUILD_INTERFACE:${opencv_SOURCE_DIR}/modules/imgcodecs/include>
    $<BUILD_INTERFACE:${opencv_SOURCE_DIR}/modules/flann/include>
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
)

target_link_libraries(sksurgeryopencvcpp
  PUBLIC
    opencv_core
    opencv_imgproc
    opencv_calib3d
    opencv_features2d
)

# OpenMP (optional)
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
  target_link_libraries(sksurgeryopencvcpp PRIVATE OpenMP::OpenMP_CXX)
endif()
