/*=============================================================================

  SKSURGERYOPENCVCPP: Image-guided surgery functions, in C++, using OpenCV.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.

=============================================================================*/

#include "sksDotDetection.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/core/types.hpp>
#include <iostream>

namespace sks
{

class ReferencePoint
{
public:

  double x;
  double y;
  int rightOfCentroid;
  int belowCentroid;
  int score;

  ReferencePoint(double x, double y)
  {
    this->x = x;
    this->y = y;
    this->score = 0;
    this->rightOfCentroid = 0;
    this->belowCentroid = 0;
  }

  bool operator < (const ReferencePoint& rp) const
  {
    return (score < rp.score);
  }
};

cv::Point2f computeCentroid(const std::vector<ReferencePoint>& referencePoints)
{
  cv::Point2f result;
  result.x = 0;
  result.y = 0;
  for (unsigned int i = 0; i < referencePoints.size(); i++)
  {
    result.x += referencePoints[i].x;
    result.y += referencePoints[i].y;
  }
  result.x /= (static_cast<float>(referencePoints.size()));
  result.y /= (static_cast<float>(referencePoints.size()));
  return result;
}

struct KeyPointSorter
{
  inline bool operator() (const cv::KeyPoint& k1, const cv::KeyPoint& k2)
  {
    return (k1.size > k2.size);
  }
};

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
  cv::GaussianBlur(distortedImage,
                   smoothed,
                   cv::Size(5, 5),
                   0);

  cv::Mat thresholded;
  cv::adaptiveThreshold(smoothed,
                        thresholded,
                        thresholdMax,
                        cv::ADAPTIVE_THRESH_MEAN_C,
                        cv::THRESH_BINARY,
                        windowSize,
                        cOffset);

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
  cv::undistort(smoothed,
                undistortedImage,
                intrinsicMatrix,
                distortionCoefficients
               );

  cv::Mat undistortedThresholdedImage;
  cv::adaptiveThreshold(undistortedImage,
                        undistortedThresholdedImage,
                        thresholdMax,
                        cv::ADAPTIVE_THRESH_MEAN_C,
                        cv::THRESH_BINARY,
                        windowSize,
                        cOffset);

  std::vector<cv::KeyPoint> undistortedKeypoints;
  detector->detect(undistortedThresholdedImage, undistortedKeypoints);

  unsigned int numberOfKeyPoints = keypoints.size();
  unsigned int numberOfUndistortedKeyPoints = undistortedKeypoints.size();

  if (numberOfKeyPoints > 4 && numberOfUndistortedKeyPoints >4)
  {
    result = cv::Mat(numberOfUndistortedKeyPoints, 6, CV_64F);

    std::sort(undistortedKeypoints.begin(),
              undistortedKeypoints.end(),
              KeyPointSorter());

    std::vector<ReferencePoint> biggestFourPoints;
    biggestFourPoints.push_back(ReferencePoint(undistortedKeypoints[0].pt.x, undistortedKeypoints[0].pt.y));
    biggestFourPoints.push_back(ReferencePoint(undistortedKeypoints[1].pt.x, undistortedKeypoints[1].pt.y));
    biggestFourPoints.push_back(ReferencePoint(undistortedKeypoints[2].pt.x, undistortedKeypoints[2].pt.y));
    biggestFourPoints.push_back(ReferencePoint(undistortedKeypoints[3].pt.x, undistortedKeypoints[3].pt.y));

    cv::Point2f centroid = computeCentroid(biggestFourPoints);

    for (unsigned int i = 0; i < biggestFourPoints.size(); i++)
    {
      if (biggestFourPoints[i].x > centroid.x)
      {
        biggestFourPoints[i].rightOfCentroid = 1;
      }
      if (biggestFourPoints[i].y > centroid.y)
      {
        biggestFourPoints[i].belowCentroid = 1;
      }
    }
    for (unsigned int i = 0; i < biggestFourPoints.size(); i++)
    {
      biggestFourPoints[i].score = biggestFourPoints[i].rightOfCentroid + (biggestFourPoints[i].belowCentroid * 2);
    }

    // Given the score assigned above, this sorting should make the biggestFourPoints
    // be ordered, top-left, top-right, bottom-left, bottom-right.
    std::sort(biggestFourPoints.begin(),
              biggestFourPoints.end());

    // Extract fiducials, suitable for passing to findHomography.
    cv::Mat sourceFiducials = cv::Mat::zeros(4, 2, CV_64F);
    cv::Mat targetFiducials = cv::Mat::zeros(4, 2, CV_64F);
    for (unsigned int i = 0; i < 4; i++)
    {
      sourceFiducials.at<double>(i, 0) = biggestFourPoints[i].x;
      sourceFiducials.at<double>(i, 1) = biggestFourPoints[i].y;
      int gridIndexForIthFiducial = indexesOfFourReferencePoints.at<int>(i, 0);
      targetFiducials.at<double>(i, 0) = gridPoints.at<double>(gridIndexForIthFiducial, 1);
      targetFiducials.at<double>(i, 1) = gridPoints.at<double>(gridIndexForIthFiducial, 2);
    }
    cv::Mat homography = cv::findHomography(sourceFiducials,
                                            targetFiducials
                                            );

    std::vector<cv::Point2f> undistortedKeyPointsAsVector;
    cv::KeyPoint::convert(undistortedKeypoints, undistortedKeyPointsAsVector);

    std::vector<cv::Point2f> transformedPoints;
    cv::perspectiveTransform(undistortedKeyPointsAsVector, transformedPoints, homography);

    // Now for each dot, find closest point in reference grid.
    double rmsError = 0;
    for (unsigned int i = 0; i < transformedPoints.size(); i++)
    {
      double bestDistanceSoFar = std::numeric_limits<double>::max();
      int bestIndexSoFar = -1;
      for (unsigned int j = 0; j < gridPoints.rows; j++)
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
      result.at<double>(i, 0) = gridPoints.at<double>(bestIndexSoFar, 0);
      result.at<double>(i, 1) = undistortedKeyPointsAsVector[i].x;
      result.at<double>(i, 2) = undistortedKeyPointsAsVector[i].y;
      result.at<double>(i, 3) = gridPoints.at<double>(bestIndexSoFar, 3);
      result.at<double>(i, 4) = gridPoints.at<double>(bestIndexSoFar, 4);
      result.at<double>(i, 5) = gridPoints.at<double>(bestIndexSoFar, 5);
      rmsError += bestDistanceSoFar;
    }

    rmsError /= static_cast<double>(transformedPoints.size());
    rmsError = sqrt(rmsError);

    if (rmsError > 10)
    {
      return result;
    }

    for (unsigned int i = 0; i < transformedPoints.size(); i++)
    {
      // First redistort (it was undistorted earlier).

      double relativeX = (result.at<double>(i, 1) - intrinsicMatrix.at<double>(0,2)) / intrinsicMatrix.at<double>(0,0);
      double relativeY = (result.at<double>(i, 2) - intrinsicMatrix.at<double>(1,2)) / intrinsicMatrix.at<double>(1,1);
      double r2 = relativeX * relativeX + relativeY * relativeY;
      double radial = (1
        + distortionCoefficients.at<double>(0, 0) * r2
        + distortionCoefficients.at<double>(0, 1) * r2 * r2
        + distortionCoefficients.at<double>(0, 4) * r2 * r2 * r2
      );

      double distortedX = relativeX * radial;
      double distortedY = relativeY * radial;

      distortedX = distortedX + (2 * distortionCoefficients.at<double>(0, 2)
                                   * relativeX * relativeY
                              + distortionCoefficients.at<double>(0, 3)
                              * (r2 + 2 * relativeX * relativeX));

      distortedY = distortedY + (distortionCoefficients.at<double>(0, 2)
                              * (r2 + 2 * relativeY * relativeY)
                              + 2 * distortionCoefficients.at<double>(0, 3)
                              * relativeX * relativeY);

      distortedX = distortedX * intrinsicMatrix.at<double>(0,0) + intrinsicMatrix.at<double>(0,2);
      distortedY = distortedY * intrinsicMatrix.at<double>(1,1) + intrinsicMatrix.at<double>(1,2);

      // Now we find the closest point on the original set of keypoints, and return that instead.
      // The reason is that even distorting/undistorting image affects the blob detector.
      double bestDistanceSoFar = std::numeric_limits<double>::max();
      int bestIndexSoFar = -1;
      for (unsigned int j = 0; j < numberOfKeyPoints; j++)
      {
        double squaredDist = (distortedX - keypoints[j].pt.x) * (distortedX - keypoints[j].pt.x)
                           + (distortedY - keypoints[j].pt.y) * (distortedY - keypoints[j].pt.y);
        if (squaredDist < bestDistanceSoFar)
        {
          bestDistanceSoFar = squaredDist;
          bestIndexSoFar = j;
        }
      }
      result.at<double>(i, 1) = keypoints[bestIndexSoFar].pt.x;
      result.at<double>(i, 2) = keypoints[bestIndexSoFar].pt.y;

    } // end for each point

  } // end if we have enough points

  return result;
}

} // end namespace
