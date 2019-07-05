/*=============================================================================

  SKSURGERYOPENCVCPP: Image-guided surgery functions, in C++, using OpenCV.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.

=============================================================================*/

#include "sksValidate.h"
#include "sksExceptionMacro.h"

namespace sks
{

//------------------------------------------------------------------------------
void ValidateStereoParameters(
  const cv::Mat& leftCameraMatrix,
  const cv::Mat& rightCameraMatrix,
  const cv::Mat& leftToRightRotationMatrix,
  const cv::Mat& leftToRightTranslationVector
  )
{
  if (leftCameraMatrix.rows != 3)
  {
    sksExceptionThrow() << "Left camera matrix does not have 3 rows!";
  }

  if (leftCameraMatrix.cols != 3)
  {
    sksExceptionThrow() << "Left camera matrix does not have 3 columns!";
  }

  if (rightCameraMatrix.rows != 3)
  {
    sksExceptionThrow() << "Right camera matrix does not have 3 rows!";
  }

  if (rightCameraMatrix.cols != 3)
  {
    sksExceptionThrow() << "Right camera matrix does not have 3 columns!";
  }

  if (leftToRightRotationMatrix.rows != 3)
  {
    sksExceptionThrow() << "Left to right rotation matrix does not have 3 rows!";
  }

  if (leftToRightRotationMatrix.cols != 3)
  {
    sksExceptionThrow() << "Left to right rotation matrix does not have 3 columns!";
  }

  if (leftToRightTranslationVector.rows != 3)
  {
    sksExceptionThrow() << "Left to right translation vector does not have 3 rows!";
  }

  if (leftToRightTranslationVector.cols != 1)
  {
    sksExceptionThrow() << "Left to right translation vector does not have 1 column!";
  }
}

} // end namespace
