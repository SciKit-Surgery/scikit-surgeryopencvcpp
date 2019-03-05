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
#include "sksMaths.h"
#include <iostream>
#include <vector>

double mathsTolerance = 0.0000001;

TEST_CASE( "Norm test.", "[Maths Tests]" ) {

  cv::Point3d p1;
  p1.x = 1;
  p1.y = 2;
  p1.z = 3;
  double l2 = sks::Norm(p1);
  REQUIRE(fabs(l2 -3.7416573868) < mathsTolerance);
}

TEST_CASE( "CrossProduct test.", "[Maths Tests]" ) {

  cv::Point3d p1;
  p1.x = 1;
  p1.y = 2;
  p1.z = 3;

  cv::Point3d p2;
  p2.x = 4;
  p2.y = 5;
  p2.z = 6;

  cv::Point3d cross = sks::CrossProduct(p1, p2);

  REQUIRE(fabs(cross.x - -3) < mathsTolerance);
  REQUIRE(fabs(cross.y -  6) < mathsTolerance);
  REQUIRE(fabs(cross.z - -3) < mathsTolerance);
}

