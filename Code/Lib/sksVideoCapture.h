/*=============================================================================

  SKSURGERYOPENCVCPP: Image-guided surgery functions, in C++, using OpenCV.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.

=============================================================================*/

#ifndef sksVideoCapture_h
#define sksVideoCapture_h

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>

#include "sksWin32ExportHeader.h"

/**
* \file sksVideoCapture.h
* \brief Essentially, just a wrapper class for a cv::VideoCapture, so
* we can control how OpenCV is configured and built, as we are having problems
* getting the Python/OpenCV wheels to work with an Epiphan SDI2USB.
* \ingroup utilities
*/
namespace sks
{

/**
* \class VideoCapture
* \brief Wrapper class around OpenCV VideoCapture.
*/
class SKSURGERYOPENCVCPP_WINEXPORT VideoCapture {

public:
  VideoCapture(unsigned int channel);
  VideoCapture(unsigned int channel, unsigned int width, unsigned int height);
  cv::Mat read();

private:
  cv::VideoCapture m_VideoCapture;

}; // end class

} // end namespace

#endif