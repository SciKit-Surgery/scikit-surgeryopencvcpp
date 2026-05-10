/*=============================================================================

  SKSURGERYOPENCVCPP: Image-guided surgery functions, in C++, using OpenCV.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.

=============================================================================*/

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "masking.h"

using Catch::Approx;

TEST_CASE("MaskPoints filters by mask image", "[masking]")
{
  cv::Mat points = cv::Mat(3, 2, CV_64FC1);
  points.at<double>(0, 0) = 0; points.at<double>(0, 1) = 1;
  points.at<double>(1, 0) = 1; points.at<double>(1, 1) = 1;
  points.at<double>(2, 0) = 2; points.at<double>(2, 1) = 2; // out of bounds

  cv::Mat mask = cv::Mat::zeros(2, 2, CV_8UC1);
  mask.at<unsigned char>(1, 0) = 1;

  cv::Mat maskedPoints = sks::MaskPoints(points, mask);
  REQUIRE(maskedPoints.rows == 1);
  REQUIRE(maskedPoints.at<double>(0, 0) == Approx(0.0));
  REQUIRE(maskedPoints.at<double>(0, 1) == Approx(1.0));
}

TEST_CASE("MaskStereoPoints filters by both masks", "[masking]")
{
  cv::Mat points = cv::Mat(3, 4, CV_64FC1);
  points.at<double>(0, 0) = 0; points.at<double>(0, 1) = 1;
  points.at<double>(0, 2) = 0; points.at<double>(0, 3) = 1;
  points.at<double>(1, 0) = 1; points.at<double>(1, 1) = 1;
  points.at<double>(1, 2) = 1; points.at<double>(1, 3) = 1;
  points.at<double>(2, 0) = 2; points.at<double>(2, 1) = 2;
  points.at<double>(2, 2) = 2; points.at<double>(2, 3) = 2; // out of bounds

  cv::Mat mask = cv::Mat::zeros(2, 2, CV_8UC1);
  mask.at<unsigned char>(1, 0) = 1;

  cv::Mat maskedPoints = sks::MaskStereoPoints(points, mask, mask);
  REQUIRE(maskedPoints.rows == 1);
  REQUIRE(maskedPoints.at<double>(0, 0) == Approx(0.0));
  REQUIRE(maskedPoints.at<double>(0, 1) == Approx(1.0));
}
