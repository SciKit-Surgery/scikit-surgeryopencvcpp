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
 * \brief Extracts calibration dot locations from a distorted camera image.
 *
 * This function detects a grid of circular calibration dots in a camera image
 * and returns their pixel locations along with their corresponding 3D model
 * coordinates. It is designed for use in camera calibration pipelines where
 * a known dot pattern is imaged.
 *
 * Algorithm overview:
 *   1. Gaussian blur and adaptive threshold the input image.
 *   2. Detect blobs (dots) using OpenCV's SimpleBlobDetector.
 *   3. Undistort the image using the provided intrinsics and distortion
 *      coefficients, then re-detect blobs in the undistorted image.
 *   4. Identify the four largest blobs as fiducial reference points.
 *   5. Classify fiducials as top-left, top-right, bottom-left, bottom-right
 *      based on their position relative to their centroid.
 *   6. Compute a homography from the four fiducials to the reference grid.
 *   7. Use the homography to assign each detected dot to its nearest grid point.
 *   8. Re-distort the undistorted dot locations and match them back to the
 *      original (distorted) blob detections for sub-pixel accuracy.
 *
 * If the RMS matching error exceeds 10 pixels, the function returns the
 * undistorted locations without the final re-distortion refinement step.
 *
 * \param distortedImage cv::Mat single-channel (CV_8UC1) greyscale image
 *        from the camera, with lens distortion still present.
 * \param intrinsicMatrix cv::Mat [3x3] CV_64FC1 camera intrinsic matrix
 *        containing focal lengths (fx, fy) and principal point (cx, cy).
 * \param distortionCoefficients cv::Mat [1x5] CV_64FC1 distortion coefficients
 *        in OpenCV order: (k1, k2, p1, p2, k3).
 * \param gridPoints cv::Mat [Nx6] CV_64FC1 reference grid definition where
 *        each row is (id, x_pix, y_pix, x_mm, y_mm, z_mm). The x_pix/y_pix
 *        columns define the expected pixel spacing for homography computation.
 *        The x_mm/y_mm/z_mm columns are the 3D model coordinates returned
 *        in the output.
 * \param indexesOfFourReferencePoints cv::Mat [4x1] CV_32S (int) containing
 *        row indexes into gridPoints identifying the four fiducial dots.
 *        These should correspond to top-left, top-right, bottom-left,
 *        bottom-right corners of a rectangle in the grid.
 * \return cv::Mat [Mx6] CV_64FC1 matrix where each row is
 *         (id, x_pix, y_pix, x_mm, y_mm, z_mm) for each detected dot.
 *         Returns an empty (0x6) matrix if fewer than 5 dots are detected.
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
