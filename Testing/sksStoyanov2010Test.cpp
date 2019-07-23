/*=============================================================================

  SKSURGERYOPENCVCPP: Image-guided surgery functions, in C++, using OpenCV.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.

=============================================================================*/

#include "catch.hpp"
#include "sksCatchMain.h"
#include "sksStoyanov2010.h"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

TEST_CASE( "Reconstruct chessboard.", "[Reconstruction Tests]" ) {

  int expectedNumberOfArgs = 3;
  if (sks::argc != expectedNumberOfArgs)
  {
    std::cerr << "Usage: mpMyFirstCatchTest fileName.txt" << std::endl;
    REQUIRE( sks::argc == expectedNumberOfArgs);
  }

  cv::Mat leftImage = cv::imread(sks::argv[1]);
  cv::Mat rightImage = cv::imread(sks::argv[2]);

  cv::Mat leftCameraMatrix = cv::Mat::eye(3, 3, CV_64FC1);
  leftCameraMatrix.at<double>(0, 0) = 2012.186314;
  leftCameraMatrix.at<double>(1, 1) = 2017.966019;
  leftCameraMatrix.at<double>(0, 2) = 944.7173708;
  leftCameraMatrix.at<double>(1, 2) = 617.1093984;

  cv::Mat rightCameraMatrix = cv::Mat::eye(3, 3, CV_64FC1);
  rightCameraMatrix.at<double>(0, 0) = 2037.233928;
  rightCameraMatrix.at<double>(1, 1) = 2052.018948;
  rightCameraMatrix.at<double>(0, 2) = 1051.112809;
  rightCameraMatrix.at<double>(1, 2) = 548.0675962;

  cv::Mat leftToRightRotation = cv::Mat::eye(3, 3, CV_64FC1);
  leftToRightRotation.at<double>(0, 0) = 0.999678;
  leftToRightRotation.at<double>(0, 1) = 0.000151;
  leftToRightRotation.at<double>(0, 2) = 0.025398;
  leftToRightRotation.at<double>(1, 0) = -0.000720;
  leftToRightRotation.at<double>(1, 1) = 0.999749;
  leftToRightRotation.at<double>(1, 2) = 0.022394;
  leftToRightRotation.at<double>(2, 0) = -0.025388;
  leftToRightRotation.at<double>(2, 1) = -0.022405;
  leftToRightRotation.at<double>(2, 2) = 0.999426;

  cv::Mat leftToRightTranslation = cv::Mat::eye(3, 1, CV_64FC1);
  leftToRightTranslation.at<double>(0, 0) = -4.631472;
  leftToRightTranslation.at<double>(1, 0) = 0.268695;
  leftToRightTranslation.at<double>(2, 0) = 1.300256;

  cv::Mat pointsIn3D = sks::GetStereoReconstruction(
    leftImage,
    leftCameraMatrix,
    rightImage,
    rightCameraMatrix,
    leftToRightRotation,
    leftToRightTranslation,
    false
  );

  REQUIRE(pointsIn3D.cols == 7);
  unsigned long int numberOfPoints = pointsIn3D.rows;
  std::cout << "Number of points reconstructed = " <<  numberOfPoints << std::endl;

}
