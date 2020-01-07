/*=============================================================================

  SKSURGERYCVCPP: scikit-surgeryopencvcpp provides opencv functions in C++

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.

=============================================================================*/

#include "catch.hpp"
#include "sksCatchMain.h"
#include <sksDotDetection.h>
#include <iostream>
#include <cstdlib>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <chrono>

TEST_CASE( "Check one file of dots.", "[Dot Detection Tests]" ) {

  int expectedNumberOfArgs = 3;
  if (sks::argc != expectedNumberOfArgs)
  {
    std::cerr << "Usage: sksDotDetectionTest image.png expectedNumberOfDots" << std::endl;
    REQUIRE( sks::argc == expectedNumberOfArgs);
  }

  std::string imageFileName = sks::argv[1];
  int expectedNumberOfDots = atoi(sks::argv[2]);
  std::cerr << "Loading:" << imageFileName << std::endl;
  std::cerr << "Expected number of dots:" << expectedNumberOfDots << std::endl;

  cv::Mat leftImage = cv::imread(imageFileName);

  cv::Mat leftCameraMatrix = cv::Mat::eye(3, 3, CV_64FC1);
  leftCameraMatrix.at<double>(0, 0) = 1766.276290;
  leftCameraMatrix.at<double>(1, 1) = 1769.623383;
  leftCameraMatrix.at<double>(0, 2) = 915.665775;
  leftCameraMatrix.at<double>(1, 2) = 458.985368;

  cv::Mat leftDistortionMatrix = cv::Mat::eye(1, 5, CV_64FC1);
  leftDistortionMatrix.at<double>(0, 0) = -0.291690;
  leftDistortionMatrix.at<double>(0, 1) = -0.001882;
  leftDistortionMatrix.at<double>(0, 2) = 0.007161;
  leftDistortionMatrix.at<double>(0, 3) = -0.000171;
  leftDistortionMatrix.at<double>(0, 4) = 0.374519;

  cv::Mat gridPoints = cv::Mat::zeros(18 * 25, 6, CV_64FC1);
  unsigned int counter = 0;
  for (unsigned int y = 0; y < 18; y++)
  {
    for (unsigned int x = 0; x < 25; x++)
    {
      gridPoints.at<double>(counter, 0) = counter;
      gridPoints.at<double>(counter, 1) = (x+1) * 50;
      gridPoints.at<double>(counter, 2) = (y+1) * 50;
      gridPoints.at<double>(counter, 3) = x * 5;
      gridPoints.at<double>(counter, 4) = y * 5;
      gridPoints.at<double>(counter, 5) = 0;
      counter++;
    }
  }

  cv::Mat referencePoints = cv::Mat::zeros(4, 1, CV_32S);
  referencePoints.at<int>(0, 0) = 133;
  referencePoints.at<int>(1, 0) = 141;
  referencePoints.at<int>(2, 0) = 308;
  referencePoints.at<int>(3, 0) = 316;

  cv::Mat greyscaleImage;
  cv::cvtColor(leftImage, greyscaleImage, cv::COLOR_BGR2GRAY);

  auto start = std::chrono::high_resolution_clock::now();

  cv::Mat result = sks::ExtractDots(
    greyscaleImage,
    leftCameraMatrix,
    leftDistortionMatrix,
    gridPoints,
    referencePoints
  );

  auto end = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  std::cerr << "Duration=" << duration.count() << std::endl;

  REQUIRE(result.rows == expectedNumberOfDots);
  REQUIRE(result.cols == 6);

}
