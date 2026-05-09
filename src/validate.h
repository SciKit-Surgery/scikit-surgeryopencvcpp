/*=============================================================================

  SKSURGERYOPENCVCPP: Image-guided surgery functions, in C++, using OpenCV.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.

=============================================================================*/

#ifndef sks_validate_h
#define sks_validate_h

#include <opencv2/core.hpp>

namespace sks
{

/**
 * \brief Validates the dimensions of stereo camera parameters.
 *
 * Checks that the provided matrices have the expected shapes for stereo
 * geometry operations. Throws sks::Exception with a descriptive message
 * if any parameter has incorrect dimensions.
 *
 * Expected dimensions:
 *   - Camera matrices: [3x3]
 *   - Rotation matrix: [3x3]
 *   - Translation vector: [3x1]
 *
 * \param leftCameraMatrix cv::Mat [3x3] left camera intrinsic matrix
 *        containing fx, fy, cx, cy.
 * \param rightCameraMatrix cv::Mat [3x3] right camera intrinsic matrix
 *        containing fx, fy, cx, cy.
 * \param leftToRightRotationMatrix cv::Mat [3x3] rotation matrix describing
 *        the orientation of the right camera relative to the left.
 * \param leftToRightTranslationVector cv::Mat [3x1] translation vector
 *        describing the position of the right camera origin relative to the left.
 * \throws sks::Exception if any matrix has incorrect dimensions.
 */
void ValidateStereoParameters(
  const cv::Mat& leftCameraMatrix,
  const cv::Mat& rightCameraMatrix,
  const cv::Mat& leftToRightRotationMatrix,
  const cv::Mat& leftToRightTranslationVector
  );

} // end namespace

#endif
