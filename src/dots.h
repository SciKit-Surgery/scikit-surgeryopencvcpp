/*=============================================================================

  SKSURGERYOPENCVCPP: Image-guided surgery functions, in C++, using OpenCV.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.

=============================================================================*/

#ifndef sks_dots_h
#define sks_dots_h

#include <opencv2/core.hpp>

namespace sks
{

/**
 * \brief Extracts calibration dot locations from a camera image.
 *
 * Detects a grid of circular calibration dots and returns their pixel
 * locations along with corresponding 3D model coordinates. Designed for
 * camera calibration pipelines using a known dot pattern with four larger
 * fiducial dots at known positions.
 *
 * Algorithm overview:
 *   1. Gaussian blur and adaptive threshold the input image.
 *   2. Detect blobs using OpenCV's SimpleBlobDetector.
 *   3. If isDistorted, undistort the detected point coordinates (not the
 *      image) using cv::undistortPoints for homography estimation.
 *   4. Identify the four largest blobs as fiducial reference points.
 *   5. Classify fiducials as top-left, top-right, bottom-left, bottom-right.
 *   6. Compute a homography from the four fiducials to the reference grid.
 *   7. Warp all undistorted point coordinates into reference space using
 *      cv::perspectiveTransform (no image warping).
 *   8. Assign each warped point to its nearest reference grid point.
 *   9. Remove duplicate ID assignments (keep only unique matches).
 *  10. Return the original detected pixel coordinates as image points.
 *
 * If the RMS matching error exceeds the tolerance, returns an empty matrix.
 *
 * \param image cv::Mat single-channel (CV_8UC1) greyscale image.
 * \param intrinsicMatrix cv::Mat [3x3] CV_64FC1 camera intrinsic matrix.
 * \param distortionCoefficients cv::Mat [1x5] CV_64FC1 distortion coefficients
 *        in OpenCV order: (k1, k2, p1, p2, k3).
 * \param gridPoints cv::Mat [Nx6] CV_64FC1 reference grid definition where
 *        each row is (id, x_pix, y_pix, x_mm, y_mm, z_mm).
 * \param indexesOfFourReferencePoints cv::Mat [4x1] CV_32S (int) containing
 *        row indexes into gridPoints identifying the four fiducial dots
 *        (top-left, top-right, bottom-left, bottom-right).
 * \param isDistorted bool if true, detected point coordinates are undistorted
 *        internally for homography estimation. If false, skips undistortion.
 * \return cv::Mat [Mx6] CV_64FC1 matrix where each row is
 *         (id, x_pix, y_pix, x_mm, y_mm, z_mm) for each detected dot.
 *         Returns an empty (0x6) matrix if detection fails.
 */
cv::Mat ExtractDots(
  const cv::Mat& image,
  const cv::Mat& intrinsicMatrix,
  const cv::Mat& distortionCoefficients,
  const cv::Mat& gridPoints,
  const cv::Mat& indexesOfFourReferencePoints,
  bool isDistorted = true
  );

} // end namespace

#endif
