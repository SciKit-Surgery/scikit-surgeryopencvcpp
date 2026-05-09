/*=============================================================================

  SKSURGERYOPENCVCPP: Image-guided surgery functions, in C++, using OpenCV.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.

=============================================================================*/

#include "masking.h"
#include "exception_macro.h"

#include <vector>

namespace sks
{

//-----------------------------------------------------------------------------
cv::Mat MaskPoints(const cv::Mat& points, const cv::Mat& mask)
{
  std::vector<int> indexes;

  for (int i = 0; i < points.rows; i++)
  {
    int x = static_cast<int>(points.at<double>(i, 0));
    int y = static_cast<int>(points.at<double>(i, 1));

    if (x >= 0 && y >= 0 && x < mask.cols && y < mask.rows
        && mask.at<unsigned char>(y, x) > 0)
    {
      indexes.push_back(i);
    }
  }

  cv::Mat outputPoints = cv::Mat(static_cast<int>(indexes.size()), 2, CV_64FC1);

  for (size_t i = 0; i < indexes.size(); i++)
  {
    outputPoints.at<double>(static_cast<int>(i), 0) = points.at<double>(indexes[i], 0);
    outputPoints.at<double>(static_cast<int>(i), 1) = points.at<double>(indexes[i], 1);
  }

  return outputPoints;
}


//-----------------------------------------------------------------------------
cv::Mat MaskStereoPoints(const cv::Mat& points,
                         const cv::Mat& leftMask,
                         const cv::Mat& rightMask)
{
  std::vector<int> indexes;

  for (int i = 0; i < points.rows; i++)
  {
    int lx = static_cast<int>(points.at<double>(i, 0));
    int ly = static_cast<int>(points.at<double>(i, 1));
    int rx = static_cast<int>(points.at<double>(i, 2));
    int ry = static_cast<int>(points.at<double>(i, 3));

    if (lx >= 0 && ly >= 0 && rx >= 0 && ry >= 0
        && lx < leftMask.cols && ly < leftMask.rows
        && rx < rightMask.cols && ry < rightMask.rows
        && leftMask.at<unsigned char>(ly, lx) > 0
        && rightMask.at<unsigned char>(ry, rx) > 0)
    {
      indexes.push_back(i);
    }
  }

  cv::Mat outputPoints = cv::Mat(static_cast<int>(indexes.size()), 4, CV_64FC1);

  for (size_t i = 0; i < indexes.size(); i++)
  {
    outputPoints.at<double>(static_cast<int>(i), 0) = points.at<double>(indexes[i], 0);
    outputPoints.at<double>(static_cast<int>(i), 1) = points.at<double>(indexes[i], 1);
    outputPoints.at<double>(static_cast<int>(i), 2) = points.at<double>(indexes[i], 2);
    outputPoints.at<double>(static_cast<int>(i), 3) = points.at<double>(indexes[i], 3);
  }

  return outputPoints;
}

} // end namespace
