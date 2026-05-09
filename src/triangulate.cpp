/*=============================================================================

  SKSURGERYOPENCVCPP: Image-guided surgery functions, in C++, using OpenCV.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.

=============================================================================*/

#include "triangulate.h"
#include "maths.h"
#include "validate.h"
#include "exception_macro.h"

#ifdef _OPENMP
#include <omp.h>
#endif

#include <opencv2/calib3d.hpp>
#include <cmath>

namespace sks
{

//-----------------------------------------------------------------------------
static void ValidateTriangulationInput(
  const cv::Mat& inputUndistortedPoints,
  const cv::Mat& leftCameraIntrinsicParams,
  const cv::Mat& rightCameraIntrinsicParams,
  const cv::Mat& leftToRightRotationMatrix,
  const cv::Mat& leftToRightTranslationVector)
{
  sks::ValidateStereoParameters(
    leftCameraIntrinsicParams,
    rightCameraIntrinsicParams,
    leftToRightRotationMatrix,
    leftToRightTranslationVector
  );

  if (inputUndistortedPoints.rows < 1)
  {
    sksExceptionThrow() << "No points to triangulate!";
  }
}


//-----------------------------------------------------------------------------
cv::Mat TriangulatePointsUsingMidpointOfShortestDistance(
  const cv::Mat& inputUndistortedPoints,
  const cv::Mat& leftCameraIntrinsicParams,
  const cv::Mat& rightCameraIntrinsicParams,
  const cv::Mat& leftToRightRotationMatrix,
  const cv::Mat& leftToRightTranslationVector
  )
{
  ValidateTriangulationInput(inputUndistortedPoints,
    leftCameraIntrinsicParams,
    rightCameraIntrinsicParams,
    leftToRightRotationMatrix,
    leftToRightTranslationVector
  );

  int numberOfPoints = inputUndistortedPoints.rows;

  cv::Mat outputPoints = cv::Mat(numberOfPoints, 3, CV_64FC1);
  cv::Mat K1           = cv::Mat(3, 3, CV_64FC1);
  cv::Mat K2           = cv::Mat(3, 3, CV_64FC1);
  cv::Mat K1Inv        = cv::Mat(3, 3, CV_64FC1);
  cv::Mat K2Inv        = cv::Mat(3, 3, CV_64FC1);
  cv::Mat L2R64        = cv::Mat::eye(4, 4, CV_64FC1);

  if (leftToRightRotationMatrix.type() == CV_32FC1)
  {
    for (int r = 0; r < 3; r++)
    {
      for (int c = 0; c < 3; c++)
      {
        L2R64.at<double>(r, c) = leftToRightRotationMatrix.at<float>(r, c);
      }
      L2R64.at<double>(r, 3) = leftToRightTranslationVector.at<float>(r, 0);
    }
  }
  else
  {
    for (int r = 0; r < 3; r++)
    {
      for (int c = 0; c < 3; c++)
      {
        L2R64.at<double>(r, c) = leftToRightRotationMatrix.at<double>(r, c);
      }
      L2R64.at<double>(r, 3) = leftToRightTranslationVector.at<double>(r, 0);
    }
  }
  cv::Mat R2L64 = L2R64.inv();
  cv::Mat R2LRot64 = R2L64(cv::Rect(0, 0, 3, 3));
  cv::Mat R2LTrn64 = R2L64(cv::Rect(3, 0, 1, 3));

  for (int r = 0; r < 3; r++)
  {
    for (int c = 0; c < 3; c++)
    {
      if (leftCameraIntrinsicParams.type() == CV_32FC1)
      {
        K1.at<double>(r, c) = leftCameraIntrinsicParams.at<float>(r, c);
      }
      else
      {
        K1.at<double>(r, c) = leftCameraIntrinsicParams.at<double>(r, c);
      }
      if (rightCameraIntrinsicParams.type() == CV_32FC1)
      {
        K2.at<double>(r, c) = rightCameraIntrinsicParams.at<float>(r, c);
      }
      else
      {
        K2.at<double>(r, c) = rightCameraIntrinsicParams.at<double>(r, c);
      }
    }
  }

  K1Inv = K1.inv();
  K2Inv = K2.inv();

  #pragma omp parallel
  {
    cv::Mat p1                = cv::Mat(3, 1, CV_64FC1);
    cv::Mat p2                = cv::Mat(3, 1, CV_64FC1);
    cv::Mat p1normalised      = cv::Mat(3, 1, CV_64FC1);
    cv::Mat p2normalised      = cv::Mat(3, 1, CV_64FC1);
    cv::Mat rhsRay            = cv::Mat(3, 1, CV_64FC1);
    cv::Mat rhsRayTransformed = cv::Mat(3, 1, CV_64FC1);

    cv::Point3d P0;
    cv::Point3d u;
    cv::Point3d Q0;
    cv::Point3d v;
    cv::Point3d midPoint;
    double UNorm, VNorm;

    #pragma omp for
    for (int i = 0; i < numberOfPoints; i++)
    {
      p1.at<double>(0,0) = inputUndistortedPoints.at<double>(i, 0);
      p1.at<double>(1,0) = inputUndistortedPoints.at<double>(i, 1);
      p1.at<double>(2,0) = 1;

      p2.at<double>(0,0) = inputUndistortedPoints.at<double>(i, 2);
      p2.at<double>(1,0) = inputUndistortedPoints.at<double>(i, 3);
      p2.at<double>(2,0) = 1;

      p1normalised = K1Inv * p1;
      p2normalised = K2Inv * p2;

      P0.x = 0;
      P0.y = 0;
      P0.z = 0;

      UNorm = std::sqrt(p1normalised.at<double>(0,0)*p1normalised.at<double>(0,0)
                       + p1normalised.at<double>(1,0)*p1normalised.at<double>(1,0)
                       + p1normalised.at<double>(2,0)*p1normalised.at<double>(2,0));
      u.x = p1normalised.at<double>(0,0) / UNorm;
      u.y = p1normalised.at<double>(1,0) / UNorm;
      u.z = p1normalised.at<double>(2,0) / UNorm;

      VNorm = std::sqrt(p2normalised.at<double>(0,0)*p2normalised.at<double>(0,0)
                       + p2normalised.at<double>(1,0)*p2normalised.at<double>(1,0)
                       + p2normalised.at<double>(2,0)*p2normalised.at<double>(2,0));

      rhsRay.at<double>(0,0) = p2normalised.at<double>(0,0) / VNorm;
      rhsRay.at<double>(1,0) = p2normalised.at<double>(1,0) / VNorm;
      rhsRay.at<double>(2,0) = p2normalised.at<double>(2,0) / VNorm;

      rhsRayTransformed = R2LRot64 * rhsRay;

      Q0.x = R2LTrn64.at<double>(0,0);
      Q0.y = R2LTrn64.at<double>(1,0);
      Q0.z = R2LTrn64.at<double>(2,0);

      v.x = rhsRayTransformed.at<double>(0,0);
      v.y = rhsRayTransformed.at<double>(1,0);
      v.z = rhsRayTransformed.at<double>(2,0);

      sks::DistanceBetweenLines(P0, u, Q0, v, midPoint);

      outputPoints.at<double>(i, 0) = midPoint.x;
      outputPoints.at<double>(i, 1) = midPoint.y;
      outputPoints.at<double>(i, 2) = midPoint.z;
    }
  } // end parallel block
  return outputPoints;
}


//-----------------------------------------------------------------------------
static cv::Mat_<double> InternalTriangulatePointUsingSVD(
    const cv::Matx34d& P1,
    const cv::Matx34d& P2,
    const cv::Point3d& u1,
    const cv::Point3d& u2,
    const double& w1,
    const double& w2)
{
  cv::Matx43d A((u1.x*P1(2,0)-P1(0,0))/w1, (u1.x*P1(2,1)-P1(0,1))/w1, (u1.x*P1(2,2)-P1(0,2))/w1,
                (u1.y*P1(2,0)-P1(1,0))/w1, (u1.y*P1(2,1)-P1(1,1))/w1, (u1.y*P1(2,2)-P1(1,2))/w1,
                (u2.x*P2(2,0)-P2(0,0))/w2, (u2.x*P2(2,1)-P2(0,1))/w2, (u2.x*P2(2,2)-P2(0,2))/w2,
                (u2.y*P2(2,0)-P2(1,0))/w2, (u2.y*P2(2,1)-P2(1,1))/w2, (u2.y*P2(2,2)-P2(1,2))/w2);

  cv::Matx41d B(-(u1.x*P1(2,3)-P1(0,3))/w1,
                -(u1.y*P1(2,3)-P1(1,3))/w1,
                -(u2.x*P2(2,3)-P2(0,3))/w2,
                -(u2.y*P2(2,3)-P2(1,3))/w2);

  cv::Mat_<double> X;
  cv::solve(A, B, X, cv::DECOMP_SVD);
  return X;
}


//-----------------------------------------------------------------------------
static cv::Point3d InternalIterativeTriangulatePointUsingSVD(
    const cv::Matx34d& P1,
    const cv::Matx34d& P2,
    const cv::Point3d& u1,
    const cv::Point3d& u2)
{
  double epsilon = 0.00000000001;
  double w1 = 1, w2 = 1;
  cv::Mat_<double> X(4, 1);

  for (int i = 0; i < 10; i++)
  {
    cv::Mat_<double> X_ = InternalTriangulatePointUsingSVD(P1, P2, u1, u2, w1, w2);
    X(0) = X_(0);
    X(1) = X_(1);
    X(2) = X_(2);
    X(3) = 1.0;

    double p2x1 = cv::Mat_<double>(cv::Mat_<double>(P1).row(2)*X)(0);
    double p2x2 = cv::Mat_<double>(cv::Mat_<double>(P2).row(2)*X)(0);

    if (std::fabs(w1 - p2x1) <= epsilon && std::fabs(w2 - p2x2) <= epsilon)
      break;

    w1 = p2x1;
    w2 = p2x2;
  }

  cv::Point3d result;
  result.x = X(0);
  result.y = X(1);
  result.z = X(2);
  return result;
}


//-----------------------------------------------------------------------------
cv::Mat TriangulatePointsUsingHartley(
  const cv::Mat& inputUndistortedPoints,
  const cv::Mat& leftCameraIntrinsicParams,
  const cv::Mat& rightCameraIntrinsicParams,
  const cv::Mat& leftToRightRotationMatrix,
  const cv::Mat& leftToRightTranslationVector
  )
{
  ValidateTriangulationInput(inputUndistortedPoints,
    leftCameraIntrinsicParams,
    rightCameraIntrinsicParams,
    leftToRightRotationMatrix,
    leftToRightTranslationVector
  );

  int numberOfPoints = inputUndistortedPoints.rows;

  cv::Mat outputPoints = cv::Mat(numberOfPoints, 3, CV_64FC1);

  cv::Mat K1    = cv::Mat(3, 3, CV_64FC1);
  cv::Mat K2    = cv::Mat(3, 3, CV_64FC1);
  cv::Mat K1Inv = cv::Mat(3, 3, CV_64FC1);
  cv::Mat K2Inv = cv::Mat(3, 3, CV_64FC1);
  cv::Mat R2    = leftToRightRotationMatrix;
  cv::Mat E1    = cv::Mat::eye(4, 4, CV_64FC1);
  cv::Mat E1Inv = cv::Mat::eye(4, 4, CV_64FC1);
  cv::Mat E2    = cv::Mat::eye(4, 4, CV_64FC1);
  cv::Mat L2R   = cv::Mat(4, 4, CV_64FC1);
  cv::Matx34d P1d, P2d;

  for (int r = 0; r < 3; r++)
  {
    for (int c = 0; c < 3; c++)
    {
      K1.at<double>(r, c) = leftCameraIntrinsicParams.at<double>(r, c);
      K2.at<double>(r, c) = rightCameraIntrinsicParams.at<double>(r, c);
      E2.at<double>(r, c) = R2.at<double>(r, c);
    }
    E2.at<double>(r, 3) = leftToRightTranslationVector.at<double>(r, 0);
  }

  K1Inv = K1.inv();
  K2Inv = K2.inv();

  E1Inv = E1.inv();
  L2R = E2 * E1Inv;

  P1d(0,0) = 1; P1d(0,1) = 0; P1d(0,2) = 0; P1d(0,3) = 0;
  P1d(1,0) = 0; P1d(1,1) = 1; P1d(1,2) = 0; P1d(1,3) = 0;
  P1d(2,0) = 0; P1d(2,1) = 0; P1d(2,2) = 1; P1d(2,3) = 0;

  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      P2d(i, j) = L2R.at<double>(i, j);
    }
  }

  #pragma omp parallel
  {
    cv::Mat u1  = cv::Mat(3, 1, CV_64FC1);
    cv::Mat u2  = cv::Mat(3, 1, CV_64FC1);
    cv::Mat u1t = cv::Mat(3, 1, CV_64FC1);
    cv::Mat u2t = cv::Mat(3, 1, CV_64FC1);

    cv::Point3d u1p, u2p;
    cv::Point3d reconstructedPoint;

    #pragma omp for
    for (int i = 0; i < numberOfPoints; i++)
    {
      u1.at<double>(0,0) = inputUndistortedPoints.at<double>(i, 0);
      u1.at<double>(1,0) = inputUndistortedPoints.at<double>(i, 1);
      u1.at<double>(2,0) = 1;

      u2.at<double>(0,0) = inputUndistortedPoints.at<double>(i, 2);
      u2.at<double>(1,0) = inputUndistortedPoints.at<double>(i, 3);
      u2.at<double>(2,0) = 1;

      u1t = K1Inv * u1;
      u2t = K2Inv * u2;

      u1p.x = u1t.at<double>(0,0);
      u1p.y = u1t.at<double>(1,0);
      u1p.z = u1t.at<double>(2,0);

      u2p.x = u2t.at<double>(0,0);
      u2p.y = u2t.at<double>(1,0);
      u2p.z = u2t.at<double>(2,0);

      reconstructedPoint = InternalIterativeTriangulatePointUsingSVD(P1d, P2d, u1p, u2p);

      outputPoints.at<double>(i, 0) = reconstructedPoint.x;
      outputPoints.at<double>(i, 1) = reconstructedPoint.y;
      outputPoints.at<double>(i, 2) = reconstructedPoint.z;
    }
  } // end parallel block
  return outputPoints;
}

} // end namespace
