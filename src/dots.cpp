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
#include <set>

namespace sks
{

namespace {

// Hardcoded detector parameters matching the Python implementation defaults.
static constexpr int GAUSSIAN_SIGMA = 5;
static constexpr int THRESHOLD_WINDOW_SIZE = 151;
static constexpr int THRESHOLD_OFFSET = 20;
static constexpr float MIN_AREA = 50.0f;
static constexpr float MAX_AREA = 50000.0f;
static constexpr float MIN_CIRCULARITY = 0.5f;
static constexpr double RMS_TOLERANCE = 30.0;

struct FiducialPoint
{
  double x;
  double y;
  int belowCentroid;
  int rightOfCentroid;
  int score;

  FiducialPoint(double px, double py)
    : x(px), y(py), belowCentroid(0), rightOfCentroid(0), score(0) {}

  bool operator<(const FiducialPoint& rp) const
  {
    return score < rp.score;
  }
};

struct KeyPointSizeSorter
{
  bool operator()(const cv::KeyPoint& k1, const cv::KeyPoint& k2) const
  {
    return k1.size > k2.size;
  }
};

cv::Ptr<cv::SimpleBlobDetector> CreateDetector()
{
  cv::SimpleBlobDetector::Params params;
  params.filterByConvexity = false;
  params.filterByInertia = true;
  params.filterByCircularity = true;
  params.minCircularity = MIN_CIRCULARITY;
  params.filterByArea = true;
  params.minArea = MIN_AREA;
  params.maxArea = MAX_AREA;
  return cv::SimpleBlobDetector::create(params);
}

} // anonymous namespace


//-----------------------------------------------------------------------------
cv::Mat ExtractDots(
  const cv::Mat& image,
  const cv::Mat& intrinsicMatrix,
  const cv::Mat& distortionCoefficients,
  const cv::Mat& gridPoints,
  const cv::Mat& indexesOfFourReferencePoints,
  int referenceImageWidth,
  int referenceImageHeight,
  bool isDistorted
  )
{
  cv::Mat emptyResult(0, 6, CV_64F);

  // Step 1: Smooth and threshold
  cv::Mat smoothed;
  cv::GaussianBlur(image, smoothed,
                   cv::Size(GAUSSIAN_SIGMA, GAUSSIAN_SIGMA), 0);

  cv::Mat thresholded;
  cv::adaptiveThreshold(smoothed, thresholded, 255,
                        cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY,
                        THRESHOLD_WINDOW_SIZE, THRESHOLD_OFFSET);

  // Step 2: Detect blobs in the (possibly distorted) thresholded image
  cv::Ptr<cv::SimpleBlobDetector> detector = CreateDetector();
  std::vector<cv::KeyPoint> keypoints;
  detector->detect(thresholded, keypoints);

  // Step 3: Undistort and re-detect if needed
  cv::Mat undistortedImage;
  std::vector<cv::KeyPoint> undistortedKeypoints;

  if (isDistorted)
  {
    cv::undistort(smoothed, undistortedImage, intrinsicMatrix, distortionCoefficients);

    cv::Mat undistortedThresholded;
    cv::adaptiveThreshold(undistortedImage, undistortedThresholded, 255,
                          cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY,
                          THRESHOLD_WINDOW_SIZE, THRESHOLD_OFFSET);

    detector->detect(undistortedThresholded, undistortedKeypoints);
  }
  else
  {
    undistortedImage = smoothed;
    undistortedKeypoints = keypoints;
  }

  // Need at least 5 points (4 fiducials + at least 1 other)
  if (keypoints.size() <= 4 || undistortedKeypoints.size() <= 4)
  {
    return emptyResult;
  }

  // Step 4: Find the four largest blobs (fiducials) in undistorted image
  std::sort(undistortedKeypoints.begin(), undistortedKeypoints.end(),
            KeyPointSizeSorter());

  std::vector<FiducialPoint> biggestFour;
  biggestFour.emplace_back(undistortedKeypoints[0].pt.x, undistortedKeypoints[0].pt.y);
  biggestFour.emplace_back(undistortedKeypoints[1].pt.x, undistortedKeypoints[1].pt.y);
  biggestFour.emplace_back(undistortedKeypoints[2].pt.x, undistortedKeypoints[2].pt.y);
  biggestFour.emplace_back(undistortedKeypoints[3].pt.x, undistortedKeypoints[3].pt.y);

  // Step 5: Classify as TL, TR, BL, BR based on centroid
  double cx = 0, cy = 0;
  for (const auto& pt : biggestFour) { cx += pt.x; cy += pt.y; }
  cx /= 4.0; cy /= 4.0;

  for (auto& pt : biggestFour)
  {
    if (pt.y > cy) pt.belowCentroid = 1;
    if (pt.x > cx) pt.rightOfCentroid = 1;
    pt.score = pt.belowCentroid * 2 + pt.rightOfCentroid;
  }

  // Sort by score: TL=0, TR=1, BL=2, BR=3
  std::sort(biggestFour.begin(), biggestFour.end());

  // Step 6: Compute homography from detected fiducials to reference grid
  cv::Mat sourceFiducials = cv::Mat::zeros(4, 2, CV_64F);
  cv::Mat targetFiducials = cv::Mat::zeros(4, 2, CV_64F);
  for (int i = 0; i < 4; i++)
  {
    sourceFiducials.at<double>(i, 0) = biggestFour[i].x;
    sourceFiducials.at<double>(i, 1) = biggestFour[i].y;
    int gridIndex = indexesOfFourReferencePoints.at<int>(i, 0);
    targetFiducials.at<double>(i, 0) = gridPoints.at<double>(gridIndex, 1);
    targetFiducials.at<double>(i, 1) = gridPoints.at<double>(gridIndex, 2);
  }
  cv::Mat homography = cv::findHomography(sourceFiducials, targetFiducials);

  if (homography.empty())
  {
    return emptyResult;
  }

  // Step 7: Warp undistorted image to canonical face-on view, re-detect
  cv::Size refSize(referenceImageWidth, referenceImageHeight);
  cv::Mat warped;
  cv::warpPerspective(undistortedImage, warped, homography, refSize);

  cv::Mat warpedThresholded;
  cv::adaptiveThreshold(warped, warpedThresholded, 255,
                        cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY,
                        THRESHOLD_WINDOW_SIZE, THRESHOLD_OFFSET);

  std::vector<cv::KeyPoint> warpedKeypoints;
  detector->detect(warpedThresholded, warpedKeypoints);

  int numberOfWarpedKeypoints = static_cast<int>(warpedKeypoints.size());
  if (numberOfWarpedKeypoints == 0)
  {
    return emptyResult;
  }

  // Step 8: For each warped dot, find closest point in reference grid
  std::vector<int> assignedGridIndex(numberOfWarpedKeypoints);
  double rmsError = 0;

  for (int i = 0; i < numberOfWarpedKeypoints; i++)
  {
    double bestDist = std::numeric_limits<double>::max();
    int bestIdx = -1;
    double wx = warpedKeypoints[i].pt.x;
    double wy = warpedKeypoints[i].pt.y;

    for (int j = 0; j < gridPoints.rows; j++)
    {
      double dx = wx - gridPoints.at<double>(j, 1);
      double dy = wy - gridPoints.at<double>(j, 2);
      double dist = dx * dx + dy * dy;
      if (dist < bestDist)
      {
        bestDist = dist;
        bestIdx = j;
      }
    }
    assignedGridIndex[i] = bestIdx;
    rmsError += std::sqrt(bestDist);
  }

  rmsError /= static_cast<double>(numberOfWarpedKeypoints);

  if (rmsError > RMS_TOLERANCE)
  {
    return emptyResult;
  }

  // Step 9: Inverse-transform warped points back to undistorted coordinates
  std::vector<cv::Point2f> warpedPoints;
  cv::KeyPoint::convert(warpedKeypoints, warpedPoints);

  cv::Mat homographyInv = homography.inv();
  std::vector<cv::Point2f> undistortedPoints;
  cv::perspectiveTransform(warpedPoints, undistortedPoints, homographyInv);

  // Step 10: If distorted input, re-distort and match to original keypoints
  std::vector<cv::Point2f> finalImagePoints(numberOfWarpedKeypoints);

  if (isDistorted)
  {
    double fx = intrinsicMatrix.at<double>(0, 0);
    double fy = intrinsicMatrix.at<double>(1, 1);
    double pcx = intrinsicMatrix.at<double>(0, 2);
    double pcy = intrinsicMatrix.at<double>(1, 2);
    double k1 = distortionCoefficients.at<double>(0, 0);
    double k2 = distortionCoefficients.at<double>(0, 1);
    double p1 = distortionCoefficients.at<double>(0, 2);
    double p2 = distortionCoefficients.at<double>(0, 3);
    double k3 = distortionCoefficients.at<double>(0, 4);

    for (int i = 0; i < numberOfWarpedKeypoints; i++)
    {
      // Re-distort the undistorted point
      double relX = (undistortedPoints[i].x - pcx) / fx;
      double relY = (undistortedPoints[i].y - pcy) / fy;
      double r2 = relX * relX + relY * relY;
      double r4 = r2 * r2;
      double r6 = r2 * r4;
      double radial = 1.0 + k1 * r2 + k2 * r4 + k3 * r6;

      double distX = relX * radial + (2.0 * p1 * relX * relY + p2 * (r2 + 2.0 * relX * relX));
      double distY = relY * radial + (p1 * (r2 + 2.0 * relY * relY) + 2.0 * p2 * relX * relY);

      distX = distX * fx + pcx;
      distY = distY * fy + pcy;

      finalImagePoints[i] = cv::Point2f(static_cast<float>(distX),
                                         static_cast<float>(distY));
    }
  }
  else
  {
    finalImagePoints = undistortedPoints;
  }

  // Step 11: Remove duplicate ID assignments
  // Find which IDs appear more than once and exclude them
  std::vector<int> idCount(gridPoints.rows, 0);
  for (int i = 0; i < numberOfWarpedKeypoints; i++)
  {
    idCount[assignedGridIndex[i]]++;
  }

  // Build final result with only unique assignments
  std::vector<int> validIndexes;
  for (int i = 0; i < numberOfWarpedKeypoints; i++)
  {
    if (idCount[assignedGridIndex[i]] == 1)
    {
      validIndexes.push_back(i);
    }
  }

  if (validIndexes.empty())
  {
    return emptyResult;
  }

  cv::Mat result(static_cast<int>(validIndexes.size()), 6, CV_64F);

  for (int i = 0; i < static_cast<int>(validIndexes.size()); i++)
  {
    int idx = validIndexes[i];
    int gridIdx = assignedGridIndex[idx];

    result.at<double>(i, 0) = gridPoints.at<double>(gridIdx, 0); // id
    result.at<double>(i, 1) = finalImagePoints[idx].x;           // x_pix
    result.at<double>(i, 2) = finalImagePoints[idx].y;           // y_pix
    result.at<double>(i, 3) = gridPoints.at<double>(gridIdx, 3); // x_mm
    result.at<double>(i, 4) = gridPoints.at<double>(gridIdx, 4); // y_mm
    result.at<double>(i, 5) = gridPoints.at<double>(gridIdx, 5); // z_mm
  }

  return result;
}

} // end namespace
