/*=============================================================================

  SKSURGERYOPENCVCPP: Image-guided surgery functions, in C++, using OpenCV.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.

=============================================================================*/

#ifndef sks_triangulate_h
#define sks_triangulate_h

#include <opencv2/core.hpp>

namespace sks
{

/**
 * \brief Triangulates undistorted 2D point pairs into 3D using the midpoint
 * of the shortest distance between two rays.
 *
 * \param inputUndistortedPoints [Nx4] matrix (left_x, left_y, right_x, right_y)
 * \param leftCameraMatrix [3x3] left camera intrinsics
 * \param rightCameraMatrix [3x3] right camera intrinsics
 * \param leftToRightRotationMatrix [3x3] rotation between cameras
 * \param leftToRightTranslationVector [3x1] translation between cameras
 * \return [Nx3] matrix of triangulated 3D points
 */
cv::Mat TriangulatePointsUsingMidpointOfShortestDistance(
  const cv::Mat& inputUndistortedPoints,
  const cv::Mat& leftCameraMatrix,
  const cv::Mat& rightCameraMatrix,
  const cv::Mat& leftToRightRotationMatrix,
  const cv::Mat& leftToRightTranslationVector
  );

/**
 * \brief Triangulates undistorted 2D point pairs into 3D using
 * Hartley & Sturm's iterative SVD method.
 *
 * \param inputUndistortedPoints [Nx4] matrix (left_x, left_y, right_x, right_y)
 * \param leftCameraMatrix [3x3] left camera intrinsics
 * \param rightCameraMatrix [3x3] right camera intrinsics
 * \param leftToRightRotationMatrix [3x3] rotation between cameras
 * \param leftToRightTranslationVector [3x1] translation between cameras
 * \return [Nx3] matrix of triangulated 3D points
 */
cv::Mat TriangulatePointsUsingHartley(
  const cv::Mat& inputUndistortedPoints,
  const cv::Mat& leftCameraMatrix,
  const cv::Mat& rightCameraMatrix,
  const cv::Mat& leftToRightRotationMatrix,
  const cv::Mat& leftToRightTranslationVector
  );

} // end namespace

#endif
