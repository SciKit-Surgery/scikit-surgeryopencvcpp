/*=============================================================================

  SKSURGERYOPENCVCPP: Image-guided surgery functions, in C++, using OpenCV.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.

=============================================================================*/

#ifndef sksValidate_h
#define sksValidate_h

#include <opencv2/core.hpp>
#include "sksWin32ExportHeader.h"

/**
* \file sksValidate.h
* \brief Functions to validate common inputs, throwing exceptions on failure.
* \ingroup utilities
*/
namespace sks
{

void ValidateStereoParameters(
  const cv::Mat& leftCameraMatrix,
  const cv::Mat& rightCameraMatrix,
  const cv::Mat& leftToRightRotationMatrix,
  const cv::Mat& leftToRightTranslationVector
  );

}

#endif