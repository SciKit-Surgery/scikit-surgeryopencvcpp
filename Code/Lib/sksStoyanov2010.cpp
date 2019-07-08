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
#include <chrono>
#include <iostream>

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
cv::Mat GetStereoReconstruction(
  const cv::Mat& leftImage,
  const cv::Mat& leftCameraMatrix,
  const cv::Mat& rightImage,
  const cv::Mat& rightCameraMatrix,
  const cv::Mat& leftToRightRotationMatrix,
  const cv::Mat& leftToRightTranslationVector
  )
{
  sks::ValidateImages(leftImage, rightImage);

  sks::ValidateStereoParameters(
    leftCameraMatrix,
    rightCameraMatrix,
    leftToRightRotationMatrix,
    leftToRightTranslationVector
  );

  cv::Size frameSize = leftImage.size();

  auto beforeQDS = std::chrono::high_resolution_clock::now();

  cv::Ptr<cv::stereo::QuasiDenseStereo> stereo = cv::stereo::QuasiDenseStereo::create(frameSize);
  stereo->process(leftImage, rightImage);

  auto afterQDS = std::chrono::high_resolution_clock::now();

  std::vector<cv::stereo::Match> matches;
  stereo->getDenseMatches(matches);

  auto afterRetrieveMatches = std::chrono::high_resolution_clock::now();

  cv::Mat matchedPoints = cv::Mat(matches.size(), 4, CV_64FC1);
  for (std::vector<cv::stereo::Match>::size_type i=0; i < matches.size(); i++)
  {
    matchedPoints.at<double>(i, 0) = matches[i].p0.x;
    matchedPoints.at<double>(i, 1) = matches[i].p0.y;
    matchedPoints.at<double>(i, 2) = matches[i].p1.x;
    matchedPoints.at<double>(i, 3) = matches[i].p1.y;
  }

  auto afterCopyingToCVMat = std::chrono::high_resolution_clock::now();

  cv::Mat outputPoints = sks::TriangulatePointsUsingHartley(matchedPoints,
                                                            leftCameraMatrix,
                                                            rightCameraMatrix,
                                                            leftToRightRotationMatrix,
                                                            leftToRightTranslationVector
                                                           );

  auto afterTriangulation = std::chrono::high_resolution_clock::now();

  std::cerr << "sks::GetStereoReconstruction:"
    << std::chrono::duration_cast<std::chrono::milliseconds>(afterQDS - beforeQDS).count() << ":"
    << std::chrono::duration_cast<std::chrono::milliseconds>(afterRetrieveMatches - afterQDS).count() << ":"
    << std::chrono::duration_cast<std::chrono::milliseconds>(afterCopyingToCVMat - afterRetrieveMatches).count() << ":"
    << std::chrono::duration_cast<std::chrono::milliseconds>(afterTriangulation - afterCopyingToCVMat).count()
    << std::endl;

  return outputPoints;
}

} // end namespace
