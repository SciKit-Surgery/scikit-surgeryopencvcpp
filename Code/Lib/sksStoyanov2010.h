/*=============================================================================

  SKSURGERYOPENCVCPP: Image-guided surgery functions, in C++, using OpenCV.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.

=============================================================================*/

#ifndef sksStoyanov2010_h
#define sksStoyanov2010_h

#include <opencv2/core.hpp>
#include "sksWin32ExportHeader.h"

/**
* \file sksStoyanov2010.h
* \brief Functions concerned with surface reconstruction using
* Dan Stoyanov's MICCAI 2010 method, now implemented in OpenCV.
*
* See https://doi.org/10.1007/978-3-642-15705-9_34
* \ingroup algorithms
*/
namespace sks
{

/**
* \brief Gets a disparity map image.
*/
extern "C++" SKSURGERYOPENCVCPP_WINEXPORT cv::Mat GetDisparityMap(
  const cv::Mat& leftImage,
  const cv::Mat& rightImage
  );


/**
* \brief Does full triangulation of matched points, returning a point cloud.
* \param[in] leftImage usually RGB image
* \param[in] leftCameraMatrix 3x3 matrix containing fx, fy, cx, cy
* \param[in] rightImage usually RGB image
* \param[in] rightCameraMatrix 3x3 matrix containing fx, fy, cx, cy
* \param[in] leftToRightRotationMatrix 3x3 rotation matrix
* \param[in] leftToRightTranslationVector 3x1 translation column vector
* \param[in] useHartley if false, uses midpoint method, if true, uses hartley. See sksTriangulate.h.
* \return Nx7 matrix, where the columns are X,Y,Z (3D triangulated point), x_left, y_left, x_right, y_right (2D matches).
*/
extern "C++" SKSURGERYOPENCVCPP_WINEXPORT cv::Mat GetStereoReconstruction(
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