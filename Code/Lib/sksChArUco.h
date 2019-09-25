/*=============================================================================

  SKSURGERYOPENCVCPP: Image-guided surgery functions, in C++, using OpenCV.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.

=============================================================================*/

#ifndef sksChArUco_h
#define sksChArUco_h

#include <opencv2/core.hpp>
#include "sksWin32ExportHeader.h"

/**
 * \file sksChArUco.h
 * \brief ChArUco processing functions.
 * \ingroup filters
 */
namespace sks
{

/**
* \brief Filters out erroneously detected points.
* \param inputPointList [N x 4] cv::Mat of points
* \return [1x3] cv::Mat of points
*
* Add further documentation and any command line args as necessary.
*/
extern "C++" SKSURGERYOPENCVCPP_WINEXPORT cv::Mat FilterErroneousChArUcoPoints(const cv::Mat& inputPointList);

}

#endif
