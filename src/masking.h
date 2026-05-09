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
 * \brief Returns points at locations with non-zero pixels in the mask image.
 *
 * \param points [Nx2] matrix of 2D points (x, y) as doubles
 * \param mask single-channel image
 * \return [Mx2] matrix of masked points
 */
cv::Mat MaskPoints(const cv::Mat& points, const cv::Mat& mask);

/**
 * \brief Returns stereo point pairs at locations with non-zero pixels in both masks.
 *
 * \param points [Nx4] matrix (left_x, left_y, right_x, right_y) as doubles
 * \param leftMask single-channel image
 * \param rightMask single-channel image
 * \return [Mx4] matrix of masked stereo points
 */
cv::Mat MaskStereoPoints(const cv::Mat& points,
                         const cv::Mat& leftMask,
                         const cv::Mat& rightMask);

} // end namespace

#endif
