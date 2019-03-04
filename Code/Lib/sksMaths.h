/*=============================================================================

  SKSURGERYOPENCVCPP: Image-guided surgery functions, in C++, using OpenCV.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.

=============================================================================*/

#ifndef sksMaths_h
#define sksMaths_h

#include <opencv2/core.hpp>
#include "sksWin32ExportHeader.h"

/**
 * \file sksMaths.h
 * \brief General Maths functions.
 * \ingroup utilities
 */
namespace sks
{

/**
 * \brief Calculates the L2 norm of a cv::Point3D.
 * \param p1 vector as cv::Point3D
 */
extern "C++" SKSURGERYOPENCVCPP_WINEXPORT double Norm(const cv::Point3d& p1);


/**
 * \brief Calculates the cross product of two cv::Point3Ds.
 * \param p1 vector as cv::Point3D
 * \param p2 vector as cv::Point3D
 */
extern "C++" SKSURGERYOPENCVCPP_WINEXPORT cv::Point3d CrossProduct(const cv::Point3d& p1, const cv::Point3d& p2);


/**
 * \brief Calculates the dot product of two cv::Point3Ds.
 * \param p1 vector as cv::Point3D
 * \param p2 vector as cv::Point3D
 */
extern "C++" SKSURGERYOPENCVCPP_WINEXPORT double DotProduct(const cv::Point3d& p1, const cv::Point3d& p2);


/**
 * \brief Calculates the shortest distance between two lines
 * where:
 * <ul>
 * <li>line 1 is defined as l_1 = P0 + lambda u</li>
 * <li>line 2 defined as l_2 = Q0 + lambda v</li>
 * </ul>
 * \param P0 as Point3d
 * \param u as Point3d
 * \param Q0 as Point3d
 * \param v as Point3d
 * \param midpoint outputs the midpoint
 */
extern "C++" SKSURGERYOPENCVCPP_WINEXPORT double DistanceBetweenLines (
  const cv::Point3d& P0, const cv::Point3d& u,
  const cv::Point3d& Q0, const cv::Point3d& v,
  cv::Point3d& midpoint );


/**
 * \brief Calculates the RMS error between two [Nx3] matrices of corresponding points.
 * \param a [Nx3] matrix of 3D coordinates
 * \param b [Nx3] matrix of corresponding 3D coordinates
 * \return RMS residual
 */
extern "C++" SKSURGERYOPENCVCPP_WINEXPORT double ComputeRMSBetweenCorrespondingPoints(const cv::Mat& a, const cv::Mat& b);

} // end namespace

#endif