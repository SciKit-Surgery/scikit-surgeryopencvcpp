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

  // Step 2: Single blob detection on the thresholded image
  cv::Ptr<cv::SimpleBlobDetector> detector = CreateDetector();
  std::vector<cv::KeyPoint> keypoints;
  detector->detect(thresholded, keypoints);

  int numberOfKeypoints = static_cast<int>(keypoints.size());
  if (numberOfKeypoints <= 4)
  {
    return emptyResult;
  }

  // Step 3: Extract keypoint coordinates (distorted image space) and sizes
  std::vector<cv::Point2f> distortedPts(numberOfKeypoints);
  std::vector<float> keypointSizes(numberOfKeypoints);
  for (int i = 0; i < numberOfKeypoints; i++)
  {
    distortedPts[i] = keypoints[i].pt;
    keypointSizes[i] = keypoints[i].size;
  }

  // Step 4: Get points in undistorted space for homography estimation
  std::vector<cv::Point2f> ptsForHomography;
  if (isDistorted)
  {
    cv::Mat distortedMat(numberOfKeypoints, 1, CV_32FC2);
    for (int i = 0; i < numberOfKeypoints; i++)
    {
      distortedMat.at<cv::Vec2f>(i, 0) = cv::Vec2f(distortedPts[i].x,
                                                     distortedPts[i].y);
    }
    cv::Mat undistortedMat;
    cv::undistortPoints(distortedMat, undistortedMat,
                        intrinsicMatrix, distortionCoefficients,
                        cv::noArray(), intrinsicMatrix);
    ptsForHomography.resize(numberOfKeypoints);
    for (int i = 0; i < numberOfKeypoints; i++)
    {
      cv::Vec2f pt = undistortedMat.at<cv::Vec2f>(i, 0);
      ptsForHomography[i] = cv::Point2f(pt[0], pt[1]);
    }
  }
  else
  {
    ptsForHomography = distortedPts;
  }

  // Step 5: Sort by size and pick biggest 4 as fiducials
  std::vector<int> sortedIndices(numberOfKeypoints);
  for (int i = 0; i < numberOfKeypoints; i++) sortedIndices[i] = i;
  std::sort(sortedIndices.begin(), sortedIndices.end(),
            [&keypointSizes](int a, int b) {
              return keypointSizes[a] < keypointSizes[b];
            });

  std::vector<FiducialPoint> biggestFour;
  for (int i = numberOfKeypoints - 4; i < numberOfKeypoints; i++)
  {
    int idx = sortedIndices[i];
    biggestFour.emplace_back(ptsForHomography[idx].x,
                             ptsForHomography[idx].y);
  }

  // Step 6: Classify as TL, TR, BL, BR based on centroid
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

  // Step 7: Compute homography from undistorted fiducials to reference grid
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

  // Step 8: Warp all undistorted points into reference space using
  // perspectiveTransform (no image warping needed)
  std::vector<cv::Point2f> warpedPts;
  cv::perspectiveTransform(ptsForHomography, warpedPts, homography);

  // Step 9: Match each warped point to the nearest reference grid point
  std::vector<int> assignedGridIndex(numberOfKeypoints);
  double rmsError = 0;

  for (int i = 0; i < numberOfKeypoints; i++)
  {
    double bestDist = std::numeric_limits<double>::max();
    int bestIdx = -1;
    double wx = warpedPts[i].x;
    double wy = warpedPts[i].y;

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

  rmsError /= static_cast<double>(numberOfKeypoints);

  if (rmsError > RMS_TOLERANCE)
  {
    return emptyResult;
  }

  // Step 10: Remove duplicate ID assignments (keep only unique matches)
  std::vector<int> idCount(gridPoints.rows, 0);
  for (int i = 0; i < numberOfKeypoints; i++)
  {
    idCount[assignedGridIndex[i]]++;
  }

  std::vector<int> validIndexes;
  for (int i = 0; i < numberOfKeypoints; i++)
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

  // Step 11: Build result using original distorted image coordinates
  cv::Mat result(static_cast<int>(validIndexes.size()), 6, CV_64F);

  for (int i = 0; i < static_cast<int>(validIndexes.size()); i++)
  {
    int idx = validIndexes[i];
    int gridIdx = assignedGridIndex[idx];

    result.at<double>(i, 0) = gridPoints.at<double>(gridIdx, 0); // id
    result.at<double>(i, 1) = distortedPts[idx].x;               // x_pix
    result.at<double>(i, 2) = distortedPts[idx].y;               // y_pix
    result.at<double>(i, 3) = gridPoints.at<double>(gridIdx, 3); // x_mm
    result.at<double>(i, 4) = gridPoints.at<double>(gridIdx, 4); // y_mm
    result.at<double>(i, 5) = gridPoints.at<double>(gridIdx, 5); // z_mm
  }

  return result;
}

} // end namespace
