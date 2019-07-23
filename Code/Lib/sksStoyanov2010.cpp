/*=============================================================================

  SKSURGERYOPENCVCPP: Image-guided surgery functions, in C++, using OpenCV.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.

=============================================================================*/

#include "sksStoyanov2010.h"
#include "sksTriangulate.h"
#include "sksExceptionMacro.h"
#include "sksValidate.h"

#include <opencv2/stereo.hpp>

namespace sks
{

//------------------------------------------------------------------------------
void ValidateImages(const cv::Mat& leftImage, const cv::Mat& rightImage)
{
  cv::Size leftSize = leftImage.size();
  cv::Size rightSize = rightImage.size();

  if (leftSize != rightSize)
  {
    sksExceptionThrow() << "Left size:" << leftSize
      << " is not equal to right size:" << rightSize;
  }
}


//------------------------------------------------------------------------------
cv::Mat GetDisparityMap(
  const cv::Mat& leftImage,
  const cv::Mat& rightImage
  )
{
  sks::ValidateImages(leftImage, rightImage);

  cv::Size frameSize = leftImage.size();

  cv::Ptr<cv::stereo::QuasiDenseStereo> stereo = cv::stereo::QuasiDenseStereo::create(frameSize);
  stereo->process(leftImage, rightImage);

  cv::Mat outputImage = stereo->getDisparity(80);
  return outputImage;
}


//------------------------------------------------------------------------------
cv::Mat GetMatches(
  const cv::Mat& leftImage,
  const cv::Mat& rightImage
  )
{
  sks::ValidateImages(leftImage, rightImage);

  cv::Size frameSize = leftImage.size();

  cv::Ptr<cv::stereo::QuasiDenseStereo> stereo = cv::stereo::QuasiDenseStereo::create(frameSize);
  stereo->process(leftImage, rightImage);

  std::vector<cv::stereo::Match> matches;
  stereo->getDenseMatches(matches);

  cv::Mat matchedPoints = cv::Mat(matches.size(), 4, CV_64FC1);
  for (std::vector<cv::stereo::Match>::size_type i=0; i < matches.size(); i++)
  {
    matchedPoints.at<double>(i, 0) = matches[i].p0.x;
    matchedPoints.at<double>(i, 1) = matches[i].p0.y;
    matchedPoints.at<double>(i, 2) = matches[i].p1.x;
    matchedPoints.at<double>(i, 3) = matches[i].p1.y;
  }
  return matchedPoints;
}


//------------------------------------------------------------------------------
cv::Mat GetStereoReconstruction(
  const cv::Mat& leftImage,
  const cv::Mat& leftCameraMatrix,
  const cv::Mat& rightImage,
  const cv::Mat& rightCameraMatrix,
  const cv::Mat& leftToRightRotationMatrix,
  const cv::Mat& leftToRightTranslationVector,
  const bool useHartley
  )
{
  sks::ValidateImages(leftImage, rightImage);

  sks::ValidateStereoParameters(
    leftCameraMatrix,
    rightCameraMatrix,
    leftToRightRotationMatrix,
    leftToRightTranslationVector
  );

  cv::Mat matchedPoints = sks::GetMatches(leftImage, rightImage);

  cv::Mat triangulatedPoints;

  if (useHartley)
  {
    triangulatedPoints = sks::TriangulatePointsUsingHartley(matchedPoints,
                                                            leftCameraMatrix,
                                                            rightCameraMatrix,
                                                            leftToRightRotationMatrix,
                                                            leftToRightTranslationVector
                                                           );
  }
  else
  {
    triangulatedPoints = sks::TriangulatePointsUsingMidpointOfShortestDistance(matchedPoints,
                                                                               leftCameraMatrix,
                                                                               rightCameraMatrix,
                                                                               leftToRightRotationMatrix,
                                                                               leftToRightTranslationVector
                                                                              );
  }

  cv::Mat outputPoints = cv::Mat(triangulatedPoints.rows, 7, CV_64FC1);

  cv::Mat output3D = outputPoints(cv::Rect(0, 0, triangulatedPoints.cols, triangulatedPoints.rows));
  triangulatedPoints.copyTo(output3D);

  cv::Mat output2D = outputPoints(cv::Rect(3, 0, matchedPoints.cols, matchedPoints.rows));
  matchedPoints.copyTo(output2D);

  return outputPoints;
}

} // end namespace
