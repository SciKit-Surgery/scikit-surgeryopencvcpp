/*=============================================================================

  SKSURGERYOPENCVCPP: Image-guided surgery functions, in C++, using OpenCV.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.

=============================================================================*/

#ifndef sks_stoyanov2010_h
#define sks_stoyanov2010_h

#include <opencv2/core.hpp>

namespace sks
{

/**
 * \brief Gets a disparity map image using quasi-dense stereo matching.
 *
 * \param leftImage usually RGB image
 * \param rightImage usually RGB image
 * \return disparity image, same size as input images
 */
cv::Mat ComputeDisparityUsingStoyanov(
  const cv::Mat& leftImage,
  const cv::Mat& rightImage
  );

/**
 * \brief Gets matching points between left and right images.
 *
 * \param leftImage usually RGB image
 * \param rightImage usually RGB image
 * \return [Nx4] matrix (left_x, left_y, right_x, right_y)
 */
cv::Mat MatchPointsUsingStoyanov(
  const cv::Mat& leftImage,
  const cv::Mat& rightImage
  );

/**
 * \brief Full triangulation of matched points, returning a point cloud.
 *
 * \param leftImage usually RGB image
 * \param leftCameraMatrix [3x3] intrinsics
 * \param rightImage usually RGB image
 * \param rightCameraMatrix [3x3] intrinsics
 * \param leftToRightRotationMatrix [3x3] rotation
 * \param leftToRightTranslationVector [3x1] translation
 * \param useHartley if true uses Hartley method, otherwise midpoint
 * \return [Nx7] matrix (X, Y, Z, left_x, left_y, right_x, right_y)
 */
cv::Mat ReconstructPointsUsingStoyanov(
  const cv::Mat& leftImage,
  const cv::Mat& leftCameraMatrix,
  const cv::Mat& rightImage,
  const cv::Mat& rightCameraMatrix,
  const cv::Mat& leftToRightRotationMatrix,
  const cv::Mat& leftToRightTranslationVector,
  const bool useHartley
  );

} // end namespace

#endif
