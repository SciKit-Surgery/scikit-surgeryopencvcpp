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
 * \brief Calculates the L2 (Euclidean) norm of a 3D point.
 *
 * \param p1 cv::Point3d representing a 3D vector.
 * \return double The Euclidean length: sqrt(x^2 + y^2 + z^2).
 */
double Norm(const cv::Point3d& p1);

/**
 * \brief Calculates the cross product of two 3D vectors.
 *
 * Returns p1 x p2 using the standard right-hand rule.
 *
 * \param p1 cv::Point3d first vector.
 * \param p2 cv::Point3d second vector.
 * \return cv::Point3d The cross product vector.
 */
cv::Point3d CrossProduct(const cv::Point3d& p1, const cv::Point3d& p2);

/**
 * \brief Calculates the dot product of two 3D vectors.
 *
 * \param p1 cv::Point3d first vector.
 * \param p2 cv::Point3d second vector.
 * \return double The scalar dot product: p1.x*p2.x + p1.y*p2.y + p1.z*p2.z.
 */
double DotProduct(const cv::Point3d& p1, const cv::Point3d& p2);

/**
 * \brief Calculates the shortest distance between two 3D lines and the
 * midpoint of the shortest connecting segment.
 *
 * Given two lines defined parametrically as:
 *   Line 1: P0 + lambda * u
 *   Line 2: Q0 + mu * v
 *
 * Computes the shortest distance between them. If the lines are not
 * parallel, also computes the midpoint of the shortest segment connecting
 * them (useful for triangulation). If the lines are parallel, the midpoint
 * is set to NaN and the perpendicular distance is returned.
 *
 * Reference: http://geomalgorithms.com/a07-_distance.html
 *
 * \param P0 cv::Point3d origin of line 1.
 * \param u cv::Point3d direction vector of line 1 (should be unit length).
 * \param Q0 cv::Point3d origin of line 2.
 * \param v cv::Point3d direction vector of line 2 (should be unit length).
 * \param midpoint cv::Point3d& [output] midpoint of the shortest segment,
 *        or (NaN, NaN, NaN) if lines are parallel.
 * \return double The shortest distance between the two lines.
 */
double DistanceBetweenLines(
  const cv::Point3d& P0, const cv::Point3d& u,
  const cv::Point3d& Q0, const cv::Point3d& v,
  cv::Point3d& midpoint);

/**
 * \brief Calculates the RMS (root mean square) error between two sets of
 * corresponding 3D points.
 *
 * Both matrices must have the same number of rows and exactly 3 columns.
 * Each row represents a 3D point (x, y, z). The RMS is computed as:
 *   sqrt( sum_of_squared_differences / N )
 *
 * \param a cv::Mat [Nx3] CV_64FC1 matrix of 3D points.
 * \param b cv::Mat [Nx3] CV_64FC1 matrix of corresponding 3D points.
 * \return double The RMS distance between the two point sets.
 * \throws sks::Exception if matrices have mismatched rows or are not [Nx3].
 */
double ComputeRMSBetweenCorrespondingPoints(const cv::Mat& a, const cv::Mat& b);

} // end namespace

#endif
