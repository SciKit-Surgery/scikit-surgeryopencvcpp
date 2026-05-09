/*=============================================================================

  SKSURGERYOPENCVCPP: Image-guided surgery functions, in C++, using OpenCV.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.

=============================================================================*/

#ifndef sks_maths_h
#define sks_maths_h

#include <opencv2/core.hpp>

namespace sks
{

/**
 * \brief Calculates the L2 norm of a cv::Point3d.
 */
double Norm(const cv::Point3d& p1);

/**
 * \brief Calculates the cross product of two cv::Point3d vectors.
 */
cv::Point3d CrossProduct(const cv::Point3d& p1, const cv::Point3d& p2);

/**
 * \brief Calculates the dot product of two cv::Point3d vectors.
 */
double DotProduct(const cv::Point3d& p1, const cv::Point3d& p2);

/**
 * \brief Calculates the shortest distance between two lines and their midpoint.
 *
 * Line 1: P0 + lambda * u
 * Line 2: Q0 + lambda * v
 */
double DistanceBetweenLines(
  const cv::Point3d& P0, const cv::Point3d& u,
  const cv::Point3d& Q0, const cv::Point3d& v,
  cv::Point3d& midpoint);

/**
 * \brief Calculates the RMS error between two [Nx3] matrices of corresponding points.
 */
double ComputeRMSBetweenCorrespondingPoints(const cv::Mat& a, const cv::Mat& b);

} // end namespace

#endif
