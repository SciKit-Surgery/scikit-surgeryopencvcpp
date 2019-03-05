/*=============================================================================

  SKSURGERYOPENCVCPP: Image-guided surgery functions, in C++, using OpenCV.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.

=============================================================================*/

#include "sksTriangulate.h"
#include "sksExceptionMacro.h"

#include <opencv2/calib3d.hpp>
#include <boost/math/special_functions/fpclassify.hpp>
#include <iostream>

namespace sks
{

//-----------------------------------------------------------------------------
double Norm(const cv::Point3d& p1)
{
  return std::sqrt(p1.x * p1.x + p1.y * p1.y + p1.z * p1.z);
}


//-----------------------------------------------------------------------------
cv::Point3d CrossProduct(const cv::Point3d& p1, const cv::Point3d& p2)
{
  cv::Point3d cp;
  cp.x = p1.y * p2.z - p1.z * p2.y;
  cp.y = p1.z * p2.x - p1.x * p2.z;
  cp.z = p1.x * p2.y - p1.y * p2.x;
  return cp;
}


//-----------------------------------------------------------------------------
double DotProduct(const cv::Point3d& p1, const cv::Point3d& p2)
{
  return p1.x * p2.x + p1.y * p2.y + p1.z * p2.z;
}


//-----------------------------------------------------------------------------
double DistanceToLine(const std::pair<cv::Point3d, cv::Point3d>& line, const cv::Point3d& x0 )
{
  // Courtesy of Wolfram Mathworld
  cv::Point3d x1;
  cv::Point3d x2;

  x1 = line.first;
  x2 = line.second;

  cv::Point3d d1 = x1-x0;
  cv::Point3d d2 = x2-x1;

  return sks::Norm(sks::CrossProduct(d2, d1)) / (sks::Norm(d2));
}


//-----------------------------------------------------------------------------
double DistanceBetweenLines(const cv::Point3d& P0, const cv::Point3d& u,
                            const cv::Point3d& Q0, const cv::Point3d& v,
                            cv::Point3d& midpoint)
{
  // Method 1. Solve for shortest line joining two rays, then get midpoint.
  // Taken from: http://geomalgorithms.com/a07-_distance.html
  double sc, tc, a, b, c, d, e;
  double distance;

  cv::Point3d Psc;
  cv::Point3d Qtc;
  cv::Point3d W0;

  // Difference of two origins
  W0.x = P0.x - Q0.x;
  W0.y = P0.y - Q0.y;
  W0.z = P0.z - Q0.z;

  a = u.x*u.x + u.y*u.y + u.z*u.z;
  b = u.x*v.x + u.y*v.y + u.z*v.z;
  c = v.x*v.x + v.y*v.y + v.z*v.z;
  d = u.x*W0.x + u.y*W0.y + u.z*W0.z;
  e = v.x*W0.x + v.y*W0.y + v.z*W0.z;
  sc = (b*e - c*d) / (a*c - b*b);
  tc = (a*e - b*d) / (a*c - b*b);

  if ( boost::math::isnan(sc) || boost::math::isnan(tc) || boost::math::isinf(sc) || boost::math::isinf(tc) )
  {
    // Lines are parallel
    distance = sks::DistanceToLine ( std::pair<cv::Point3d, cv::Point3d> ( P0, P0 + u ), Q0 );
    midpoint.x = std::numeric_limits<double>::quiet_NaN();
    midpoint.y = std::numeric_limits<double>::quiet_NaN();
    midpoint.z = std::numeric_limits<double>::quiet_NaN();
    return distance;
  }
  Psc.x = P0.x + sc*u.x;
  Psc.y = P0.y + sc*u.y;
  Psc.z = P0.z + sc*u.z;
  Qtc.x = Q0.x + tc*v.x;
  Qtc.y = Q0.y + tc*v.y;
  Qtc.z = Q0.z + tc*v.z;

  distance = std::sqrt((Psc.x - Qtc.x)*(Psc.x - Qtc.x)
                        +(Psc.y - Qtc.y)*(Psc.y - Qtc.y)
                        +(Psc.z - Qtc.z)*(Psc.z - Qtc.z));

  midpoint.x = (Psc.x + Qtc.x)/2.0;
  midpoint.y = (Psc.y + Qtc.y)/2.0;
  midpoint.z = (Psc.z + Qtc.z)/2.0;

  return distance;
}


//-----------------------------------------------------------------------------
double ComputeRMSBetweenCorrespondingPoints(const cv::Mat& a, const cv::Mat& b)
{
  double rms = 0;
  double diff = 0;
  double squaredDiff = 0;

  if (a.rows != b.rows)
  {
    sksExceptionThrow() << "a has " << a.rows << " rows, but b has " << b.rows;
  }

  if (a.cols != 3)
  {
    sksExceptionThrow() << "a does not have 3 columns.";
  }

  if (b.cols != 3)
  {
    sksExceptionThrow() << "b does not have 3 columns.";
  }

  for (unsigned int r = 0; r < a.rows; r++)
  {
    for (unsigned int c = 0; c < a.cols; c++)
    {
      diff = (b.at<double>(r, c) - a.at<double>(r, c));
      squaredDiff = diff * diff;
      rms += squaredDiff;
    }
  }
  rms /= static_cast<double>(a.rows);
  rms = std::sqrt(rms);
  return rms;
}

} // end namespace
