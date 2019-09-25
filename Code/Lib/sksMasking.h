/*=============================================================================

  SKSURGERYOPENCVCPP: Image-guided surgery functions, in C++, using OpenCV.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.

=============================================================================*/

#ifndef sksMasking_h
#define sksMasking_h

#include <opencv2/core.hpp>
#include "sksWin32ExportHeader.h"

/**
* \file sksMasking.h
* \brief Functions concerned with masking point lists.
* \ingroup algorithms
*/
namespace sks
{

/**
 * \brief Returns points that occur at locations with non-zero pixels in image.
 *
 * \param points [Nx2] matrix of 2D points, x, y, as doubles.
 * \param mask image
 * \return [Mx2] matrix of masked points, x, y, as doubles.
 */
extern "C++" SKSURGERYOPENCVCPP_WINEXPORT cv::Mat MaskPoints(const cv::Mat& points,
                                                             const cv::Mat& mask);



/**
 * \brief Returns points that occur at locations with non-zero pixels in both leftImage and rightImage.
 *
 * \param points [Nx4] matrix of 2D points, where each row is left_x, left_y, right_x, right_y, as doubles.
 * \param leftMask image
 * \param rightMask image
 * \return [Mx4] matrix of masked points as doubles.
 */
extern "C++" SKSURGERYOPENCVCPP_WINEXPORT cv::Mat MaskStereoPoints(const cv::Mat& points,
                                                                   const cv::Mat& leftMask,
                                                                   const cv::Mat& rightMask);

} // end namespace

#endif
