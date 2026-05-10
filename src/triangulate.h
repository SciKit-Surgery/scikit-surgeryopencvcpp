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
 * \brief Triangulates undistorted 2D stereo point pairs into 3D using the
 * midpoint of the shortest distance between two back-projected rays.
 *
 * For each point pair, a ray is cast from each camera origin through the
 * normalised image coordinate. The 3D point is taken as the midpoint of
 * the shortest line segment connecting the two rays. This is a simple
 * geometric method that works well when the stereo baseline is reasonable
 * relative to the scene depth.
 *
 * Points must already be undistorted (i.e. corrected for lens distortion)
 * before being passed to this function.
 *
 * The function is parallelised with OpenMP when available.
 *
 * Reference: http://geomalgorithms.com/a07-_distance.html
 *
 * \param inputUndistortedPoints cv::Mat [Nx4] CV_64FC1 matrix where each row
 *        contains (left_x, left_y, right_x, right_y) in pixel coordinates.
 * \param leftCameraMatrix cv::Mat [3x3] left camera intrinsic matrix (CV_64FC1 or CV_32FC1).
 * \param rightCameraMatrix cv::Mat [3x3] right camera intrinsic matrix (CV_64FC1 or CV_32FC1).
 * \param leftToRightRotationMatrix cv::Mat [3x3] rotation from left to right
 *        camera coordinate frame (CV_64FC1 or CV_32FC1).
 * \param leftToRightTranslationVector cv::Mat [3x1] translation from left to
 *        right camera origin (CV_64FC1 or CV_32FC1).
 * \return cv::Mat [Nx3] CV_64FC1 matrix of triangulated 3D points in the left
 *         camera coordinate frame.
 * \throws sks::Exception if input dimensions are invalid or no points are provided.
 */
cv::Mat TriangulatePointsUsingMidpointOfShortestDistance(
  const cv::Mat& inputUndistortedPoints,
  const cv::Mat& leftCameraMatrix,
  const cv::Mat& rightCameraMatrix,
  const cv::Mat& leftToRightRotationMatrix,
  const cv::Mat& leftToRightTranslationVector
  );

/**
 * \brief Triangulates undistorted 2D stereo point pairs into 3D using
 * Hartley & Sturm's iterative linear method.
 *
 * Implements the optimal triangulation approach from:
 *   "Triangulation", Hartley, R.I. and Sturm, P.,
 *   Computer Vision and Image Understanding, 1997.
 *
 * For each point pair, constructs a linear system from the projection
 * matrices and solves via SVD. Iteratively refines the solution by
 * re-weighting (up to 10 iterations) to account for projective depth.
 *
 * Points must already be undistorted (i.e. corrected for lens distortion)
 * before being passed to this function.
 *
 * The function is parallelised with OpenMP when available.
 *
 * \param inputUndistortedPoints cv::Mat [Nx4] CV_64FC1 matrix where each row
 *        contains (left_x, left_y, right_x, right_y) in pixel coordinates.
 * \param leftCameraMatrix cv::Mat [3x3] CV_64FC1 left camera intrinsic matrix.
 * \param rightCameraMatrix cv::Mat [3x3] CV_64FC1 right camera intrinsic matrix.
 * \param leftToRightRotationMatrix cv::Mat [3x3] CV_64FC1 rotation from left
 *        to right camera coordinate frame.
 * \param leftToRightTranslationVector cv::Mat [3x1] CV_64FC1 translation from
 *        left to right camera origin.
 * \return cv::Mat [Nx3] CV_64FC1 matrix of triangulated 3D points in the left
 *         camera coordinate frame.
 * \throws sks::Exception if input dimensions are invalid or no points are provided.
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
