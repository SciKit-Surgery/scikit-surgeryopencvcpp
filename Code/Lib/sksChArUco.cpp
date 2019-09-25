/*=============================================================================

  SKSURGERYOPENCVCPP: Image-guided surgery functions, in C++, using OpenCV.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.

=============================================================================*/

#include "sksChArUco.h"

namespace sks
{

//-----------------------------------------------------------------------------
cv::Mat FilterErroneousChArUcoPoints(const cv::Mat& inputPointList)
{
  cv::Mat outputPoints = cv::Mat(1, 3, CV_64FC1);
  return outputPoints;
}

} // end namespace
