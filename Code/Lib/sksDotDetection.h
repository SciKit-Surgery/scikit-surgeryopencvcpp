/*=============================================================================

  SKSURGERYOPENCVCPP: Image-guided surgery functions, in C++, using OpenCV.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.

=============================================================================*/

#ifndef sksDotDetection_h
#define sksDotDetection_h

#include <opencv2/core.hpp>
#include "sksWin32ExportHeader.h"

/**
* \file sksDotDetection.h
* \brief Functions to extract the locations of a pattern
* of dots from a calibration image.
*
* \ingroup algorithms
*/
namespace sks
{

/**
* \brief Extracts calibration points from an image of dots.
*
* In this method, we pass in a distorted image. This method is
* intended for use in a camera calibration routine. It assumes
* you have a fairly good initial guess of the camera intrinsic matrix
* and distortion coefficients. These can then be used to undistort
* the image, which makes identification of the dots more robust.
* The points that are returned are the exact locations as detected
* in the distortedImage
*
* The four point indexes should specify top-left, top-right,
* bottom-left, bottom-right of a rectangle, where these dots
* are typically 3 times larger than all the other dots,
* and so are easy to identify and use as fiducials for a homography.
*
* \param distortedImage distorted image
* \param intrinsicMatrix good guess of the intrinsic matrix
* \param distortionCoefficients good guess of the distortion coefficients
* \param gridPoints [nx6] array of rows of id, x_pix, y_pix, x_mm, y_mm, z_mm reference point locations
* \param indexesOfFourReferencePoints [4x1] list of exactly 4 point indexes.
* \return [nx6] array of rows of id, x_pix, y_pix, x_mm, y_mm, z_mm of detected point locations
*/
extern "C++" SKSURGERYOPENCVCPP_WINEXPORT cv::Mat ExtractDots(
  const cv::Mat& distortedImage,
  const cv::Mat& intrinsicMatrix,
  const cv::Mat& distortionCoefficients,
  const cv::Mat& gridPoints,
  const cv::Mat& indexesOfFourReferencePoints
);

} // end namespace

#endif