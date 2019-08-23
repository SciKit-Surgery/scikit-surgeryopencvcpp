/*=============================================================================

  SKSURGERYOPENCVCPP: Image-guided surgery functions, in C++, using OpenCV.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.

=============================================================================*/

#include "sksMasking.h"
#include "sksValidate.h"
#include "sksExceptionMacro.h"

#include <opencv2/calib3d.hpp>
#include <iostream>
#include <vector>

namespace sks
{

//-----------------------------------------------------------------------------
cv::Mat MaskPoints(const cv::Mat& points,
                   const cv::Mat& mask)
{

  std::vector<unsigned long int> indexes;

  for (unsigned long int i = 0; i < points.rows; i++)
  {
    if (   points.at<double>(i, 0) >= 0
        && points.at<double>(i, 1) >= 0
        && static_cast<int>(points.at<double>(i, 0)) < mask.cols
        && static_cast<int>(points.at<double>(i, 1)) < mask.rows
        && mask.at<unsigned char>(static_cast<int>(points.at<double>(i, 1)),
                                  static_cast<int>(points.at<double>(i, 0)) > 0)
                                 )
    {
      indexes.push_back(i);
    }
  }

  cv::Mat outputPoints = cv::Mat(indexes.size(), 2, CV_64FC1);

  for (unsigned long int i = 0; i < indexes.size(); i++)
  {
    outputPoints.at<double>(i, 0) = points.at<double>(indexes[i], 0);
    outputPoints.at<double>(i, 1) = points.at<double>(indexes[i], 1);
  }

  return outputPoints;
}


//-----------------------------------------------------------------------------
cv::Mat MaskStereoPoints(const cv::Mat& points,
                         const cv::Mat& leftMask,
                         const cv::Mat& rightMask)
{
  std::vector<unsigned long int> indexes;

  for (unsigned long int i = 0; i < points.rows; i++)
  {
    if (   points.at<double>(i, 0) >= 0
        && points.at<double>(i, 1) >= 0
        && points.at<double>(i, 2) >= 0
        && points.at<double>(i, 3) >= 0
        && static_cast<int>(points.at<double>(i, 0)) < leftMask.cols
        && static_cast<int>(points.at<double>(i, 1)) < leftMask.rows
        && static_cast<int>(points.at<double>(i, 2)) < rightMask.cols
        && static_cast<int>(points.at<double>(i, 3)) < rightMask.rows
        && leftMask.at<unsigned char>(static_cast<int>(points.at<double>(i, 1)),
                                      static_cast<int>(points.at<double>(i, 0)) > 0)
        && rightMask.at<unsigned char>(static_cast<int>(points.at<double>(i, 1)),
                                       static_cast<int>(points.at<double>(i, 0)) > 0)
       )
    {
      indexes.push_back(i);
    }
  }

  cv::Mat outputPoints = cv::Mat(indexes.size(), 4, CV_64FC1);

  for (unsigned long int i = 0; i < indexes.size(); i++)
  {
    outputPoints.at<double>(i, 0) = points.at<double>(indexes[i], 0);
    outputPoints.at<double>(i, 1) = points.at<double>(indexes[i], 1);
    outputPoints.at<double>(i, 2) = points.at<double>(indexes[i], 2);
    outputPoints.at<double>(i, 3) = points.at<double>(indexes[i], 3);
  }

  return outputPoints;
}

} // end namespace