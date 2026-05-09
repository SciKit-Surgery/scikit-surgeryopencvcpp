/*=============================================================================

  SKSURGERYOPENCVCPP: Image-guided surgery functions, in C++, using OpenCV.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.

=============================================================================*/

#ifndef sks_dot_detection_h
#define sks_dot_detection_h

#include <opencv2/core.hpp>

namespace sks
{

/**
 * \brief Extracts calibration dot locations from a distorted image.
 *
 * Uses blob detection on a thresholded image, identifies four large fiducial
 * dots, computes a homography to a reference grid, and assigns grid IDs to
 * all detected dots.
 *
 * \param distortedImage greyscale distorted image
 * \param intrinsicMatrix [3x3] camera intrinsics
 * \param distortionCoefficients [1x5] distortion coefficients
 * \param gridPoints [Nx6] reference grid (id, x_pix, y_pix, x_mm, y_mm, z_mm)
 * \param indexesOfFourReferencePoints [4x1] indexes of the four fiducial dots
 * \return [Mx6] detected points (id, x_pix, y_pix, x_mm, y_mm, z_mm)
 */
cv::Mat ExtractDots(
  const cv::Mat& distortedImage,
  const cv::Mat& intrinsicMatrix,
  const cv::Mat& distortionCoefficients,
  const cv::Mat& gridPoints,
  const cv::Mat& indexesOfFourReferencePoints
  );

} // end namespace

#endif
