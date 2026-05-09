/*=============================================================================

  SKSURGERYOPENCVCPP: Image-guided surgery functions, in C++, using OpenCV.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.

=============================================================================*/

#include "dots.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/core/types.hpp>
#include <algorithm>
#include <cmath>
#include <limits>
#include <vector>

namespace sks
{

namespace {

struct ReferencePoint
{
  double x;
  double y;
  int rightOfCentroid;
  int belowCentroid;
  int score;

  ReferencePoint(double px, double py)
    : x(px), y(py), rightOfCentroid(0), belowCentroid(0), score(0) {}

  bool operator<(const ReferencePoint& rp) const
  {
    return score < rp.score;
  }
};

cv::Point2f ComputeCentroid(const std::vector<ReferencePoint>& referencePoints)
{
  cv::Point2f result(0.0f, 0.0f);
  for (const auto& rp : referencePoints)
  {
    result.x += static_cast<float>(rp.x);
    result.y += static_cast<float>(rp.y);
  }
  float n = static_cast<float>(referencePoints.size());
  result.x /= n;
  result.y /= n;
  return result;
}

struct KeyPointSorter
{
  bool operator()(const cv::KeyPoint& k1, const cv::KeyPoint& k2) const
  {
    return k1.size > k2.size;
  }
};

} // anonymous namespace


//-----------------------------------------------------------------------------
cv::Mat ExtractDots(
  const cv::Mat& distortedImage,
  const cv::Mat& intrinsicMatrix,
  const cv::Mat& distortionCoefficients,
  const cv::Mat& gridPoints,
  const cv::Mat& indexesOfFourReferencePoints
  )
{
  cv::Mat result(0, 6, CV_64F);

  unsigned char thresholdMax = 255;
  unsigned short windowSize = 151;
  unsigned char cOffset = 20;

  cv::Mat smoothed;
  cv::GaussianBlur(distortedImage, smoothed, cv::Size(5, 5), 0);

  cv::Mat thresholded;
  cv::adaptiveThreshold(smoothed, thresholded, thresholdMax,
                        cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY,
                        windowSize, cOffset);

  cv::SimpleBlobDetector::Params params;
  params.filterByConvexity = false;
  params.filterByInertia = true;
  params.filterByCircularity = true;
  params.filterByArea = true;
  params.minArea = 50;
  params.maxArea = 50000;

  cv::Ptr<cv::SimpleBlobDetector> detector = cv::SimpleBlobDetector::create(params);
  std::vector<cv::KeyPoint> keypoints;
  detector->detect(thresholded, keypoints);

  cv::Mat undistortedImage;
  cv::undistort(smoothed, undistortedImage, intrinsicMatrix, distortionCoefficients);

  cv::Mat undistortedThresholdedImage;
  cv::adaptiveThreshold(undistortedImage, undistortedThresholdedImage, thresholdMax,
                        cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY,
                        windowSize, cOffset);

  std::vector<cv::KeyPoint> undistortedKeypoints;
  detector->detect(undistortedThresholdedImage, undistortedKeypoints);

  unsigned int numberOfKeyPoints = static_cast<unsigned int>(keypoints.size());
  unsigned int numberOfUndistortedKeyPoints = static_cast<unsigned int>(undistortedKeypoints.size());

  if (numberOfKeyPoints > 4 && numberOfUndistortedKeyPoints > 4)
  {
    result = cv::Mat(static_cast<int>(numberOfUndistortedKeyPoints), 6, CV_64F);

    std::sort(undistortedKeypoints.begin(), undistortedKeypoints.end(), KeyPointSorter());

    std::vector<ReferencePoint> biggestFourPoints;
    biggestFourPoints.emplace_back(undistortedKeypoints[0].pt.x, undistortedKeypoints[0].pt.y);
    biggestFourPoints.emplace_back(undistortedKeypoints[1].pt.x, undistortedKeypoints[1].pt.y);
    biggestFourPoints.emplace_back(undistortedKeypoints[2].pt.x, undistortedKeypoints[2].pt.y);
    biggestFourPoints.emplace_back(undistortedKeypoints[3].pt.x, undistortedKeypoints[3].pt.y);

    cv::Point2f centroid = ComputeCentroid(biggestFourPoints);

    for (auto& pt : biggestFourPoints)
    {
      if (pt.x > centroid.x) pt.rightOfCentroid = 1;
      if (pt.y > centroid.y) pt.belowCentroid = 1;
      pt.score = pt.rightOfCentroid + (pt.belowCentroid * 2);
    }

    std::sort(biggestFourPoints.begin(), biggestFourPoints.end());

    cv::Mat sourceFiducials = cv::Mat::zeros(4, 2, CV_64F);
    cv::Mat targetFiducials = cv::Mat::zeros(4, 2, CV_64F);
    for (int i = 0; i < 4; i++)
    {
      sourceFiducials.at<double>(i, 0) = biggestFourPoints[i].x;
      sourceFiducials.at<double>(i, 1) = biggestFourPoints[i].y;
      int gridIndex = indexesOfFourReferencePoints.at<int>(i, 0);
      targetFiducials.at<double>(i, 0) = gridPoints.at<double>(gridIndex, 1);
      targetFiducials.at<double>(i, 1) = gridPoints.at<double>(gridIndex, 2);
    }
    cv::Mat homography = cv::findHomography(sourceFiducials, targetFiducials);

    std::vector<cv::Point2f> undistortedKeyPointsAsVector;
    cv::KeyPoint::convert(undistortedKeypoints, undistortedKeyPointsAsVector);

    std::vector<cv::Point2f> transformedPoints;
    cv::perspectiveTransform(undistortedKeyPointsAsVector, transformedPoints, homography);

    double rmsError = 0;
    for (unsigned int i = 0; i < transformedPoints.size(); i++)
    {
      double bestDistanceSoFar = std::numeric_limits<double>::max();
      int bestIndexSoFar = -1;
      for (int j = 0; j < gridPoints.rows; j++)
      {
        double squaredDist = (transformedPoints[i].x - gridPoints.at<double>(j, 1))
                           * (transformedPoints[i].x - gridPoints.at<double>(j, 1))
                           + (transformedPoints[i].y - gridPoints.at<double>(j, 2))
                           * (transformedPoints[i].y - gridPoints.at<double>(j, 2));
        if (squaredDist < bestDistanceSoFar)
        {
          bestDistanceSoFar = squaredDist;
          bestIndexSoFar = j;
        }
      }
      result.at<double>(static_cast<int>(i), 0) = gridPoints.at<double>(bestIndexSoFar, 0);
      result.at<double>(static_cast<int>(i), 1) = undistortedKeyPointsAsVector[i].x;
      result.at<double>(static_cast<int>(i), 2) = undistortedKeyPointsAsVector[i].y;
      result.at<double>(static_cast<int>(i), 3) = gridPoints.at<double>(bestIndexSoFar, 3);
      result.at<double>(static_cast<int>(i), 4) = gridPoints.at<double>(bestIndexSoFar, 4);
      result.at<double>(static_cast<int>(i), 5) = gridPoints.at<double>(bestIndexSoFar, 5);
      rmsError += bestDistanceSoFar;
    }

    rmsError /= static_cast<double>(transformedPoints.size());
    rmsError = std::sqrt(rmsError);

    if (rmsError > 10)
    {
      return result;
    }

    for (unsigned int i = 0; i < transformedPoints.size(); i++)
    {
      double relativeX = (result.at<double>(static_cast<int>(i), 1) - intrinsicMatrix.at<double>(0,2)) / intrinsicMatrix.at<double>(0,0);
      double relativeY = (result.at<double>(static_cast<int>(i), 2) - intrinsicMatrix.at<double>(1,2)) / intrinsicMatrix.at<double>(1,1);
      double r2 = relativeX * relativeX + relativeY * relativeY;
      double radial = (1
        + distortionCoefficients.at<double>(0, 0) * r2
        + distortionCoefficients.at<double>(0, 1) * r2 * r2
        + distortionCoefficients.at<double>(0, 4) * r2 * r2 * r2);

      double distortedX = relativeX * radial;
      double distortedY = relativeY * radial;

      distortedX += (2 * distortionCoefficients.at<double>(0, 2) * relativeX * relativeY
                    + distortionCoefficients.at<double>(0, 3) * (r2 + 2 * relativeX * relativeX));

      distortedY += (distortionCoefficients.at<double>(0, 2) * (r2 + 2 * relativeY * relativeY)
                    + 2 * distortionCoefficients.at<double>(0, 3) * relativeX * relativeY);

      distortedX = distortedX * intrinsicMatrix.at<double>(0,0) + intrinsicMatrix.at<double>(0,2);
      distortedY = distortedY * intrinsicMatrix.at<double>(1,1) + intrinsicMatrix.at<double>(1,2);

      double bestDistanceSoFar = std::numeric_limits<double>::max();
      int bestIndexSoFar = -1;
      for (unsigned int j = 0; j < numberOfKeyPoints; j++)
      {
        double squaredDist = (distortedX - keypoints[j].pt.x) * (distortedX - keypoints[j].pt.x)
                           + (distortedY - keypoints[j].pt.y) * (distortedY - keypoints[j].pt.y);
        if (squaredDist < bestDistanceSoFar)
        {
          bestDistanceSoFar = squaredDist;
          bestIndexSoFar = static_cast<int>(j);
        }
      }
      result.at<double>(static_cast<int>(i), 1) = keypoints[bestIndexSoFar].pt.x;
      result.at<double>(static_cast<int>(i), 2) = keypoints[bestIndexSoFar].pt.y;
    }
  }

  return result;
}

} // end namespace
