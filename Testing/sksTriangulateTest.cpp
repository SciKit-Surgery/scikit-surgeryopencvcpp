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
#include "sksTriangulate.h"
#include "sksMaths.h"
#include <iostream>
#include <vector>

TEST_CASE( "Empty points throws exception.", "[Triangulate Tests]" ) {

  REQUIRE_THROWS(sks::TriangulatePointsUsingHartley(cv::Mat(0, 4, CV_64FC1),
                                                    cv::Mat(3, 3, CV_64FC1),
                                                    cv::Mat(3, 3, CV_64FC1),
                                                    cv::Mat(3, 3, CV_64FC1),
                                                    cv::Mat(3, 1, CV_64FC1)
                                                    ));

  REQUIRE_THROWS(sks::TriangulatePointsUsingMidpointOfShortestDistance(cv::Mat(0, 4, CV_64FC1),
                                                                               cv::Mat(3, 3, CV_64FC1),
                                                                               cv::Mat(3, 3, CV_64FC1),
                                                                               cv::Mat(3, 3, CV_64FC1),
                                                                               cv::Mat(3, 1, CV_64FC1)
                                                                               ));

}


TEST_CASE( "Invalid matrix sizes throw exceptions.", "[Triangulate Tests]" ) {

  REQUIRE_THROWS(sks::TriangulatePointsUsingHartley(cv::Mat(1, 4, CV_64FC1),
                                                    cv::Mat(2, 3, CV_64FC1),
                                                    cv::Mat(3, 3, CV_64FC1),
                                                    cv::Mat(3, 3, CV_64FC1),
                                                    cv::Mat(3, 1, CV_64FC1)
                                                    ));

  REQUIRE_THROWS(sks::TriangulatePointsUsingHartley(cv::Mat(1, 4, CV_64FC1),
                                                    cv::Mat(3, 2, CV_64FC1),
                                                    cv::Mat(3, 3, CV_64FC1),
                                                    cv::Mat(3, 3, CV_64FC1),
                                                    cv::Mat(3, 1, CV_64FC1)
                                                    ));

  REQUIRE_THROWS(sks::TriangulatePointsUsingHartley(cv::Mat(1, 4, CV_64FC1),
                                                    cv::Mat(3, 3, CV_64FC1),
                                                    cv::Mat(1, 3, CV_64FC1),
                                                    cv::Mat(3, 3, CV_64FC1),
                                                    cv::Mat(3, 1, CV_64FC1)
                                                    ));

  REQUIRE_THROWS(sks::TriangulatePointsUsingHartley(cv::Mat(1, 4, CV_64FC1),
                                                    cv::Mat(3, 3, CV_64FC1),
                                                    cv::Mat(3, 1, CV_64FC1),
                                                    cv::Mat(3, 3, CV_64FC1),
                                                    cv::Mat(3, 1, CV_64FC1)
                                                    ));

  REQUIRE_THROWS(sks::TriangulatePointsUsingHartley(cv::Mat(1, 4, CV_64FC1),
                                                    cv::Mat(3, 3, CV_64FC1),
                                                    cv::Mat(3, 3, CV_64FC1),
                                                    cv::Mat(1, 3, CV_64FC1),
                                                    cv::Mat(3, 1, CV_64FC1)
                                                    ));

  REQUIRE_THROWS(sks::TriangulatePointsUsingHartley(cv::Mat(1, 4, CV_64FC1),
                                                    cv::Mat(3, 3, CV_64FC1),
                                                    cv::Mat(3, 3, CV_64FC1),
                                                    cv::Mat(3, 1, CV_64FC1),
                                                    cv::Mat(3, 1, CV_64FC1)
                                                    ));

  REQUIRE_THROWS(sks::TriangulatePointsUsingHartley(cv::Mat(1, 4, CV_64FC1),
                                                    cv::Mat(3, 3, CV_64FC1),
                                                    cv::Mat(3, 3, CV_64FC1),
                                                    cv::Mat(3, 1, CV_64FC1),
                                                    cv::Mat(1, 1, CV_64FC1)
                                                    ));

  REQUIRE_THROWS(sks::TriangulatePointsUsingHartley(cv::Mat(1, 4, CV_64FC1),
                                                    cv::Mat(3, 3, CV_64FC1),
                                                    cv::Mat(3, 3, CV_64FC1),
                                                    cv::Mat(3, 1, CV_64FC1),
                                                    cv::Mat(3, 2, CV_64FC1)
                                                    ));
}


TEST_CASE( "Chessboard Test.", "[Triangulate Tests]" ) {

  // Derived from test data in scikit-surgeryvtk, all hard coded.

  cv::Mat leftIntrinsic = cv::Mat::eye(3, 3, CV_64FC1);
  leftIntrinsic.at<double>(0, 0) = 2012.186314;
  leftIntrinsic.at<double>(1, 1) = 2017.966019;
  leftIntrinsic.at<double>(0, 2) = 944.7173708;
  leftIntrinsic.at<double>(1, 2) = 617.1093984;

  cv::Mat rightIntrinsic = cv::Mat::eye(3, 3, CV_64FC1);
  rightIntrinsic.at<double>(0, 0) = 2037.233928;
  rightIntrinsic.at<double>(1, 1) = 2052.018948;
  rightIntrinsic.at<double>(0, 2) = 1051.112809;
  rightIntrinsic.at<double>(1, 2) = 548.0675962;

  cv::Mat leftRotation = cv::Mat::eye(3, 3, CV_64FC1);
  leftRotation.at<double>(0, 0) = 0.966285949;
  leftRotation.at<double>(0, 1) = -0.1053020017;
  leftRotation.at<double>(0, 2) = 0.2349530874;
  leftRotation.at<double>(1, 0) = -0.005105986897;
  leftRotation.at<double>(1, 1) = 0.9045241988;
  leftRotation.at<double>(1, 2) = 0.4263917244;
  leftRotation.at<double>(2, 0) = -0.2574206552;
  leftRotation.at<double>(2, 1) = -0.4132159994;
  leftRotation.at<double>(2, 2) = 0.8734913532;

  cv::Mat leftTranslation = cv::Mat::eye(3, 1, CV_64FC1);
  leftTranslation.at<double>(0, 0) = 9.847672184;
  leftTranslation.at<double>(1, 0) = -22.45992103;
  leftTranslation.at<double>(2, 0) = 127.7836183;

  cv::Mat rightRotation = cv::Mat::eye(3, 3, CV_64FC1);
  rightRotation.at<double>(0, 0) = 0.958512625;
  rightRotation.at<double>(0, 1) = -0.1175427792;
  rightRotation.at<double>(0, 2) = 0.2596868167;
  rightRotation.at<double>(1, 0) = -0.0115032253;
  rightRotation.at<double>(1, 1) = 0.8943292319;
  rightRotation.at<double>(1, 2) = 0.4472615574;
  rightRotation.at<double>(2, 0) = -0.2848178778;
  rightRotation.at<double>(2, 1) = -0.4316930854;
  rightRotation.at<double>(2, 2) = 0.8558737386;

  cv::Mat rightTranslation = cv::Mat::eye(3, 1, CV_64FC1);
  rightTranslation.at<double>(0, 0) = 8.461514886;
  rightTranslation.at<double>(1, 0) = -19.3242747;
  rightTranslation.at<double>(2, 0) = 129.0937601;

  cv::Mat pointsIn2D = cv::Mat::zeros(4, 4, CV_64FC1);
  pointsIn2D.at<double>(0, 0) = 1100.16;
  pointsIn2D.at<double>(0, 1) = 262.974;
  pointsIn2D.at<double>(0, 2) = 1184.84;
  pointsIn2D.at<double>(0, 3) = 241.915;
  pointsIn2D.at<double>(1, 0) = 1757.74;
  pointsIn2D.at<double>(1, 1) = 228.971;
  pointsIn2D.at<double>(1, 2) = 1843.52;
  pointsIn2D.at<double>(1, 3) = 204.083;
  pointsIn2D.at<double>(2, 0) = 1065.44;
  pointsIn2D.at<double>(2, 1) = 651.593;
  pointsIn2D.at<double>(2, 2) = 1142.75;
  pointsIn2D.at<double>(2, 3) = 632.817;
  pointsIn2D.at<double>(3, 0) = 1788.22;
  pointsIn2D.at<double>(3, 1) = 650.41;
  pointsIn2D.at<double>(3, 2) = 1867.78;
  pointsIn2D.at<double>(3, 3) = 632.59;

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

  cv::Mat modelPoints = cv::Mat::eye(4, 3, CV_64FC1);
  modelPoints.at<double>(0, 0) = 0;
  modelPoints.at<double>(0, 1) = 0;
  modelPoints.at<double>(0, 2) = 0;
  modelPoints.at<double>(1, 0) = 39;
  modelPoints.at<double>(1, 1) = 0;
  modelPoints.at<double>(1, 2) = 0;
  modelPoints.at<double>(2, 0) = 0;
  modelPoints.at<double>(2, 1) = 27;
  modelPoints.at<double>(2, 2) = 0;
  modelPoints.at<double>(3, 0) = 39;
  modelPoints.at<double>(3, 1) = 27;
  modelPoints.at<double>(3, 2) = 0;

  cv::Mat pointsFromHartley = sks::TriangulatePointsUsingHartley(pointsIn2D,
                                                                 leftIntrinsic,
                                                                 rightIntrinsic,
                                                                 leftToRightRotation,
                                                                 leftToRightTranslation
                                                                );

  cv::Mat pointsFromMidpoint = sks::TriangulatePointsUsingMidpointOfShortestDistance(pointsIn2D,
                                                                                     leftIntrinsic,
                                                                                     rightIntrinsic,
                                                                                     leftToRightRotation,
                                                                                     leftToRightTranslation
                                                                                     );
  cv::Mat modelPointsTransposed;
  cv::transpose(modelPoints, modelPointsTransposed);

  cv::Mat rotatedModelPoints = cv::Mat(modelPointsTransposed.rows, modelPointsTransposed.cols, CV_64FC1);
  cv::gemm(leftRotation, modelPointsTransposed, 1, cv::Mat(), 0, rotatedModelPoints);

  cv::Mat modelPointsRotatedAndTransposed;
  cv::transpose(rotatedModelPoints, modelPointsRotatedAndTransposed);

  cv::Mat transformedModelPoints = cv::Mat(modelPointsRotatedAndTransposed.rows, modelPointsRotatedAndTransposed.cols, CV_64FC1);
  for (int r = 0; r < modelPointsRotatedAndTransposed.rows; r++)
  {
    for (int c = 0; c < modelPointsRotatedAndTransposed.cols; c++)
    {
      transformedModelPoints.at<double>(r, c) = modelPointsRotatedAndTransposed.at<double>(r, c) + leftTranslation.at<double>(c, 0);
    }
  }

  std::cerr << "pointsFromHartley=" << pointsFromHartley << std::endl;
  std::cerr << "pointsFromMidpoint=" << pointsFromMidpoint << std::endl;
  std::cerr << "transformedModelPoints=" << transformedModelPoints << std::endl;

  double rmsHartley = sks::ComputeRMSBetweenCorrespondingPoints(transformedModelPoints, pointsFromHartley);
  double rmsMidpoint = sks::ComputeRMSBetweenCorrespondingPoints(transformedModelPoints, pointsFromMidpoint);

  std::cerr << "rmsHartley=" << rmsHartley << std::endl;
  std::cerr << "rmsMidpoints=" << rmsMidpoint << std::endl;

  REQUIRE(rmsHartley < 1.5);
  REQUIRE(rmsMidpoint < 1.5);
}
