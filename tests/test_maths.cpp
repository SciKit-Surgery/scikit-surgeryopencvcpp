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
#include "maths.h"

using Catch::Approx;

TEST_CASE("Norm computes L2 length", "[maths]")
{
  cv::Point3d p{1.0, 2.0, 3.0};
  REQUIRE(sks::Norm(p) == Approx(3.7416573868));
}

TEST_CASE("CrossProduct is correct", "[maths]")
{
  cv::Point3d p1{1.0, 2.0, 3.0};
  cv::Point3d p2{4.0, 5.0, 6.0};

  cv::Point3d cross = sks::CrossProduct(p1, p2);

  REQUIRE(cross.x == Approx(-3.0));
  REQUIRE(cross.y == Approx(6.0));
  REQUIRE(cross.z == Approx(-3.0));
}

TEST_CASE("DotProduct is correct", "[maths]")
{
  cv::Point3d p1{1.0, 2.0, 3.0};
  cv::Point3d p2{4.0, 5.0, 6.0};

  REQUIRE(sks::DotProduct(p1, p2) == Approx(32.0));
}

TEST_CASE("ComputeRMSBetweenCorrespondingPoints with identical points gives zero", "[maths]")
{
  cv::Mat a = cv::Mat::ones(5, 3, CV_64FC1);
  cv::Mat b = a.clone();

  REQUIRE(sks::ComputeRMSBetweenCorrespondingPoints(a, b) == Approx(0.0));
}

TEST_CASE("ComputeRMSBetweenCorrespondingPoints throws on mismatched rows", "[maths]")
{
  cv::Mat a = cv::Mat::ones(5, 3, CV_64FC1);
  cv::Mat b = cv::Mat::ones(3, 3, CV_64FC1);

  REQUIRE_THROWS(sks::ComputeRMSBetweenCorrespondingPoints(a, b));
}
