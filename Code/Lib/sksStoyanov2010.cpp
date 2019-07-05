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

#include <opencv2/stereo.hpp>

namespace sks
{

//------------------------------------------------------------------------------
cv::Mat GetDisparityMap(
  const cv::Mat& leftImage,
  const cv::Mat& rightImage
  )
{
  cv::Size frameSize = leftImage.size();

  cv::Ptr<cv::stereo::QuasiDenseStereo> stereo = cv::stereo::QuasiDenseStereo::create(frameSize);
  stereo->process(leftImage, rightImage);

  cv::Mat outputImage = stereo->getDisparity(80);
  return outputImage;
}


//------------------------------------------------------------------------------
cv::Mat GetStereoReconstruction(
  const cv::Mat& leftImage,
  const cv::Mat& leftCameraMatrix,
  const cv::Mat& rightImage,
  const cv::Mat& rightCameraMatrix,
  const cv::Mat& leftToRightRotationMatrix,
  const cv::Mat& leftToRightTranslationVector
  )
{
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

  cv::Mat outputPoints = sks::TriangulatePointsUsingHartley(matchedPoints,
                                                            leftCameraMatrix,
                                                            rightCameraMatrix,
                                                            leftToRightRotationMatrix,
                                                            leftToRightTranslationVector
                                                           );
  return outputPoints;
}

} // end namespace
