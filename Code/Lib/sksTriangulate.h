/*=============================================================================

  SKSURGERYOPENCVCPP: Image-guided surgery functions, in C++, using OpenCV.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.

=============================================================================*/

#ifndef sksTriangulate_h
#define sksTriangulate_h

#include <opencv2/core.hpp>
#include "sksWin32ExportHeader.h"

/**
* \file sksTriangulate.h
* \brief Functions concerned with triangulation of sets of points.
* \ingroup utilities
*/
namespace sks
{

/**
 * \brief Triangulates a vector of un-distorted (i.e. already correction for distortion) 2D point pairs back into 3D.
 *
 * Taken from: http://geomalgorithms.com/a07-_distance.html
 *
 * \param inputUndistortedPoints [Nx4] matrix of 2D points, where each row is left_x, left_y, right_x, right_y.
 * \param leftCameraMatrix [3x3] left camera matrix
 * \param rightCameraMatrix [3x3] right camera matrix
 * \param leftToRightRotationMatrix [3x3] matrix representing the rotation between camera axes
 * \param leftToRightTranslationVector [3x1] translation between camera origins
 * \return [Nx3] matrix of triangulated points.
 */
extern "C++" SKSURGERYOPENCVCPP_WINEXPORT cv::Mat TriangulatePointsUsingMidpointOfShortestDistance(
  const cv::Mat& inputUndistortedPoints,
  const cv::Mat& leftCameraMatrix,
  const cv::Mat& rightCameraMatrix,
  const cv::Mat& leftToRightRotationMatrix,
  const cv::Mat& leftToRightTranslationVector
  );


/**
 * \brief Triangulates a vector of un-distorted (i.e. already correction for distortion) 2D point pairs back into 3D.
 *
 * From:
 * <ul>
 *  <li>"Triangulation", Hartley, R.I. and Sturm, P., Computer vision and image understanding, 1997.</li>
 *  <li><a href="http://www.morethantechnical.com/2012/01/04/simple-triangulation-with-opencv-from-harley-zisserman-w-code/">www.morethantechnical.com</a>.</li>
 *  <li>Price 2012, Computer Vision: Models, Learning and Inference.</li>
 * </ul>
 *
 * \param inputUndistortedPoints [Nx4] matrix of 2D points, where each row is left_x, left_y, right_x, right_y.
 * \param leftCameraMatrix [3x3] left camera matrix
 * \param rightCameraMatrix [3x3] right camera matrix
 * \param leftToRightRotationMatrix [3x3] matrix representing the rotation between camera axes
 * \param leftToRightTranslationVector [3x1] translation between camera origins
 * \return [Nx3] matrix of triangulated points.
 */
extern "C++" SKSURGERYOPENCVCPP_WINEXPORT cv::Mat TriangulatePointsUsingHartley(
  const cv::Mat& inputUndistortedPoints,
  const cv::Mat& leftCameraMatrix,
  const cv::Mat& rightCameraMatrix,
  const cv::Mat& leftToRightRotationMatrix,
  const cv::Mat& leftToRightTranslationVector
  );

} // end namespace
#endif