/*=============================================================================

  SKSURGERYOPENCVCPP: Image-guided surgery functions, in C++, using OpenCV.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.

=============================================================================*/

/**
 * \file bindings.cpp
 * \brief pybind11 module definition for sksurgeryopencvpython.
 *
 * This file defines the Python module interface, exposing C++ functions
 * from the sksurgeryopencvcpp library to Python. NumPy arrays are
 * automatically converted to/from cv::Mat via the NDArrayConverter
 * type caster registered in ndarray_converter.h.
 *
 * All functions accept and return numpy ndarrays. Exceptions thrown
 * by the C++ library (sks::Exception) are translated to Python
 * RuntimeError with file/line information preserved.
 */

#include <pybind11/pybind11.h>
#include "ndarray_converter.h"

#include "triangulate.h"
#include "masking.h"
#include "dots.h"
#include "exception.h"

#include <sstream>

namespace py = pybind11;

/**
 * \brief Translates sks::Exception to a Python RuntimeError.
 *
 * Formats the error message to include the C++ source file and line
 * number where the exception originated.
 */
static void translate_exception(const sks::Exception& e)
{
  std::ostringstream ss;
  ss << e.GetDescription()
     << " [" << e.GetFileName() << ":" << e.GetLineNumber() << "]";
  PyErr_SetString(PyExc_RuntimeError, ss.str().c_str());
}

PYBIND11_MODULE(sksurgeryopencvpython, m)
{
  sks::NDArrayConverter::init_numpy();

  py::register_exception_translator([](std::exception_ptr p) {
    try {
      if (p) std::rethrow_exception(p);
    } catch (const sks::Exception& e) {
      translate_exception(e);
    }
  });

  m.doc() = "scikit-surgeryopencvcpp: Image-guided surgery functions using OpenCV.\n\n"
             "All functions accept numpy ndarrays and return numpy ndarrays.\n"
             "Matrix dimensions follow OpenCV conventions (row-major, CV_64FC1).";

  // --- Triangulation ---

  m.def("triangulate_points_using_hartley",
        &sks::TriangulatePointsUsingHartley,
        "Triangulate undistorted 2D stereo point pairs into 3D using "
        "Hartley & Sturm's iterative SVD method.\n\n"
        "Args:\n"
        "    points: ndarray [Nx4] float64 - (left_x, left_y, right_x, right_y)\n"
        "    left_intrinsics: ndarray [3x3] float64 - left camera matrix\n"
        "    right_intrinsics: ndarray [3x3] float64 - right camera matrix\n"
        "    rotation_matrix: ndarray [3x3] float64 - left-to-right rotation\n"
        "    translation_vector: ndarray [3x1] float64 - left-to-right translation\n\n"
        "Returns:\n"
        "    ndarray [Nx3] float64 - triangulated 3D points in left camera frame",
        py::arg("points"), py::arg("left_intrinsics"), py::arg("right_intrinsics"),
        py::arg("rotation_matrix"), py::arg("translation_vector"));

  m.def("triangulate_points_using_midpoint",
        &sks::TriangulatePointsUsingMidpointOfShortestDistance,
        "Triangulate undistorted 2D stereo point pairs into 3D using the "
        "midpoint of the shortest distance between back-projected rays.\n\n"
        "Args:\n"
        "    points: ndarray [Nx4] float64 - (left_x, left_y, right_x, right_y)\n"
        "    left_intrinsics: ndarray [3x3] float64 - left camera matrix\n"
        "    right_intrinsics: ndarray [3x3] float64 - right camera matrix\n"
        "    rotation_matrix: ndarray [3x3] float64 - left-to-right rotation\n"
        "    translation_vector: ndarray [3x1] float64 - left-to-right translation\n\n"
        "Returns:\n"
        "    ndarray [Nx3] float64 - triangulated 3D points in left camera frame",
        py::arg("points"), py::arg("left_intrinsics"), py::arg("right_intrinsics"),
        py::arg("rotation_matrix"), py::arg("translation_vector"));

  // --- Masking ---

  m.def("mask_points",
        &sks::MaskPoints,
        "Filter 2D points by a binary mask image, keeping only points where "
        "the mask pixel is non-zero.\n\n"
        "Args:\n"
        "    points: ndarray [Nx2] float64 - 2D points (x, y) in pixels\n"
        "    mask: ndarray [HxW] uint8 - binary mask image\n\n"
        "Returns:\n"
        "    ndarray [Mx2] float64 - filtered points (M <= N)",
        py::arg("points"), py::arg("mask"));

  m.def("mask_stereo_points",
        &sks::MaskStereoPoints,
        "Filter stereo point pairs by left and right binary mask images, "
        "keeping only pairs where both left and right points fall on "
        "non-zero mask pixels.\n\n"
        "Args:\n"
        "    points: ndarray [Nx4] float64 - (left_x, left_y, right_x, right_y)\n"
        "    left_mask: ndarray [HxW] uint8 - binary mask for left image\n"
        "    right_mask: ndarray [HxW] uint8 - binary mask for right image\n\n"
        "Returns:\n"
        "    ndarray [Mx4] float64 - filtered stereo pairs (M <= N)",
        py::arg("points"), py::arg("left_mask"), py::arg("right_mask"));

  // --- Dot detection ---

  m.def("extract_dots",
        &sks::ExtractDots,
        "Extract calibration dot locations from a camera image.\n\n"
        "Detects a grid of circular dots, identifies four large fiducial dots, "
        "warps to a canonical view, assigns grid IDs, and returns matched "
        "dot locations with duplicate removal.\n\n"
        "Args:\n"
        "    image: ndarray [HxW] uint8 - greyscale image\n"
        "    intrinsic_matrix: ndarray [3x3] float64 - camera intrinsics\n"
        "    distortion_coefficients: ndarray [1x5] float64 - (k1, k2, p1, p2, k3)\n"
        "    grid_points: ndarray [Nx6] float64 - reference grid "
        "(id, x_pix, y_pix, x_mm, y_mm, z_mm)\n"
        "    reference_point_indexes: ndarray [4x1] int32 - indexes of four "
        "fiducial dots in grid_points\n"
        "    reference_image_width: int - width of canonical warped image\n"
        "    reference_image_height: int - height of canonical warped image\n"
        "    is_distorted: bool - True if input has lens distortion (default True)\n\n"
        "Returns:\n"
        "    ndarray [Mx6] float64 - detected dots "
        "(id, x_pix, y_pix, x_mm, y_mm, z_mm)",
        py::arg("image"), py::arg("intrinsic_matrix"),
        py::arg("distortion_coefficients"), py::arg("grid_points"),
        py::arg("reference_point_indexes"),
        py::arg("reference_image_width"), py::arg("reference_image_height"),
        py::arg("is_distorted") = true);
}
