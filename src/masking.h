/*=============================================================================

  SKSURGERYOPENCVCPP: Image-guided surgery functions, in C++, using OpenCV.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.

=============================================================================*/

#ifndef sks_masking_h
#define sks_masking_h

#include <opencv2/core.hpp>

namespace sks
{

/**
 * \brief Filters 2D points by a binary mask image, returning only those
 * at locations where the mask is non-zero.
 *
 * For each point (x, y), checks whether the pixel at mask(y, x) is non-zero.
 * Points that fall outside the mask image bounds are discarded.
 *
 * \param points cv::Mat [Nx2] CV_64FC1 matrix of 2D points, where each row
 *        is (x, y) in pixel coordinates.
 * \param mask cv::Mat single-channel (CV_8UC1) image used as the binary mask.
 *        Non-zero pixels indicate valid regions.
 * \return cv::Mat [Mx2] CV_64FC1 matrix containing only the points that
 *         passed the mask test (M <= N).
 */
cv::Mat MaskPoints(const cv::Mat& points, const cv::Mat& mask);

/**
 * \brief Filters stereo point pairs by left and right binary mask images.
 *
 * For each row (left_x, left_y, right_x, right_y), checks that both the
 * left point falls on a non-zero pixel in leftMask and the right point
 * falls on a non-zero pixel in rightMask. Points outside either mask's
 * bounds are discarded.
 *
 * \param points cv::Mat [Nx4] CV_64FC1 matrix of stereo point pairs, where
 *        each row is (left_x, left_y, right_x, right_y) in pixel coordinates.
 * \param leftMask cv::Mat single-channel (CV_8UC1) mask for the left image.
 *        Non-zero pixels indicate valid regions.
 * \param rightMask cv::Mat single-channel (CV_8UC1) mask for the right image.
 *        Non-zero pixels indicate valid regions.
 * \return cv::Mat [Mx4] CV_64FC1 matrix containing only the stereo pairs
 *         that passed both mask tests (M <= N).
 */
cv::Mat MaskStereoPoints(const cv::Mat& points,
                         const cv::Mat& leftMask,
                         const cv::Mat& rightMask);

} // end namespace

#endif
