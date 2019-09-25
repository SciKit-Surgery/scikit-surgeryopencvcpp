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
#include "sksMasking.h"
#include "sksMaths.h"
#include <iostream>
#include <vector>

TEST_CASE( "Mono.", "[Masking Tests]" ) {

  cv::Mat points = cv::Mat::eye(3, 2, CV_64FC1);
  points.at<double>(0, 0) = 0;
  points.at<double>(0, 1) = 1;
  points.at<double>(1, 0) = 1;
  points.at<double>(1, 1) = 1;
  points.at<double>(2, 0) = 2; // deliberately out of bounds
  points.at<double>(2, 1) = 2;

  cv::Mat image = cv::Mat::zeros(2, 2, CV_8UC1);
  image.at<unsigned char>(1, 0) = 1;

  cv::Mat maskedPoints = sks::MaskPoints(points, image);
  REQUIRE(maskedPoints.rows == 1);
  REQUIRE(points.at<double>(0, 0) == 0);
  REQUIRE(points.at<double>(0, 1) == 1);
}

TEST_CASE( "Stereo.", "[Masking Tests]" ) {

  cv::Mat points = cv::Mat::eye(3, 4, CV_64FC1);
  points.at<double>(0, 0) = 0;
  points.at<double>(0, 1) = 1;
  points.at<double>(0, 2) = 0;
  points.at<double>(0, 3) = 1;
  points.at<double>(1, 0) = 1;
  points.at<double>(1, 1) = 1;
  points.at<double>(1, 2) = 1;
  points.at<double>(1, 3) = 1;
  points.at<double>(2, 0) = 2; // deliberately out of bounds
  points.at<double>(2, 1) = 2;
  points.at<double>(2, 2) = 2; // deliberately out of bounds
  points.at<double>(2, 3) = 2;

  cv::Mat image = cv::Mat::zeros(2, 2, CV_8UC1);
  image.at<unsigned char>(1, 0) = 1;

  cv::Mat maskedPoints = sks::MaskStereoPoints(points, image, image);
  REQUIRE(maskedPoints.rows == 1);
  REQUIRE(points.at<double>(0, 0) == 0);
  REQUIRE(points.at<double>(0, 1) == 1);
}
