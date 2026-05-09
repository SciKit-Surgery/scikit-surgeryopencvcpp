/*=============================================================================

  SKSURGERYOPENCVCPP: Image-guided surgery functions, in C++, using OpenCV.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.

=============================================================================*/

#include <pybind11/pybind11.h>
#include "ndarray_converter.h"

#include "triangulate.h"
#include "stoyanov2010.h"
#include "masking.h"
#include "dot_detection.h"
#include "exception.h"

#include <sstream>

namespace py = pybind11;

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

  m.doc() = "scikit-surgeryopencvcpp: Image-guided surgery functions using OpenCV";

  // Triangulation
  m.def("triangulate_points_using_hartley",
        &sks::TriangulatePointsUsingHartley,
        "Triangulate undistorted 2D point pairs using Hartley's iterative SVD method",
        py::arg("points"), py::arg("left_intrinsics"), py::arg("right_intrinsics"),
        py::arg("rotation_matrix"), py::arg("translation_vector"));

  m.def("triangulate_points_using_midpoint",
        &sks::TriangulatePointsUsingMidpointOfShortestDistance,
        "Triangulate undistorted 2D point pairs using midpoint of shortest distance",
        py::arg("points"), py::arg("left_intrinsics"), py::arg("right_intrinsics"),
        py::arg("rotation_matrix"), py::arg("translation_vector"));

  // Surface reconstruction (Stoyanov 2010)
  m.def("compute_disparity_using_stoyanov",
        &sks::ComputeDisparityUsingStoyanov,
        "Compute disparity map using quasi-dense stereo matching",
        py::arg("left_image"), py::arg("right_image"));

  m.def("match_points_using_stoyanov",
        &sks::MatchPointsUsingStoyanov,
        "Get matching points between stereo image pair",
        py::arg("left_image"), py::arg("right_image"));

  m.def("reconstruct_points_using_stoyanov",
        &sks::ReconstructPointsUsingStoyanov,
        "Full stereo reconstruction returning Nx7 point cloud",
        py::arg("left_image"), py::arg("left_intrinsics"),
        py::arg("right_image"), py::arg("right_intrinsics"),
        py::arg("rotation_matrix"), py::arg("translation_vector"),
        py::arg("use_hartley"));

  // Masking
  m.def("mask_points",
        &sks::MaskPoints,
        "Filter 2D points by a binary mask image",
        py::arg("points"), py::arg("mask"));

  m.def("mask_stereo_points",
        &sks::MaskStereoPoints,
        "Filter stereo point pairs by left and right binary mask images",
        py::arg("points"), py::arg("left_mask"), py::arg("right_mask"));

  // Dot detection
  m.def("extract_dots",
        &sks::ExtractDots,
        "Extract calibration dot locations from a distorted image",
        py::arg("image"), py::arg("intrinsic_matrix"),
        py::arg("distortion_coefficients"), py::arg("grid_points"),
        py::arg("reference_point_indexes"));
}
