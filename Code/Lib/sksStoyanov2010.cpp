/*=============================================================================

  SKSURGERYOPENCVCPP: Image-guided surgery functions, in C++, using OpenCV.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.

=============================================================================*/

#include "sksStoyanov2010.h"

namespace sks
{

//------------------------------------------------------------------------------
cv::Mat GetDisparityMap(
  const cv::Mat& leftImage,
  const cv::Mat& rightImage
  )
{
  cv::Mat outputImage = cv::Mat(1, 1, CV_8UC1);
  return outputImage;
}


//------------------------------------------------------------------------------
cv::Mat GetStereoReconstruction(
  const cv::Mat& leftImage,
  const cv::Mat& leftIntrinsics,
  const cv::Mat& rightImage,
  const cv::Mat& rightIntrinsics
  )
{
  cv::Mat outputPoints = cv::Mat(1, 3, CV_64FC1);
  return outputPoints;
}

} // end namespace
