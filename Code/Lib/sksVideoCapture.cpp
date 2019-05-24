/*=============================================================================

  SKSURGERYOPENCVCPP: Image-guided surgery functions, in C++, using OpenCV.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.

=============================================================================*/

#include "sksVideoCapture.h"
#include "sksExceptionMacro.h"

#include <iostream>

namespace sks
{

//-----------------------------------------------------------------------------
VideoCapture::VideoCapture()
{
  try
  {
    m_VideoCapture = cv::VideoCapture(0);
    if (!m_VideoCapture.isOpened())
    {
      sksExceptionThrow() << "sks::VideoCapture() did not open";
    }
  } catch (std::exception& e)
  {
    sksExceptionThrow() << "Caught OpenCV error:" << e.what();
  }

}

//-----------------------------------------------------------------------------
VideoCapture::VideoCapture(unsigned int channel,
                           unsigned int width,
                           unsigned int height)
{
  if (width < 1)
  {
    sksExceptionThrow() << "width must be positive";
  }
  if (height < 1)
  {
    sksExceptionThrow() << "height must be positive";
  }
  try
  {
    m_VideoCapture = cv::VideoCapture();
    m_VideoCapture.set(cv::CAP_PROP_FRAME_WIDTH, width);
    m_VideoCapture.set(cv::CAP_PROP_FRAME_HEIGHT, height);
    m_VideoCapture.open(channel);
    if (!m_VideoCapture.isOpened())
    {
      sksExceptionThrow() << "sks::VideoCapture("
       << channel << ", " << width << ", " << height
       << ") did not open";
    }
  } catch (std::exception& e)
  {
    sksExceptionThrow() << "Caught OpenCV error:" << e.what();
  }
}


//-----------------------------------------------------------------------------
cv::Mat VideoCapture::read()
{
  if (!m_VideoCapture.isOpened())
  {
    sksExceptionThrow() << "sks::VideoCapture is not open";
  }

  cv::Mat output;
  bool grabbed = m_VideoCapture.read(output);
  if (!grabbed)
  {
    sksExceptionThrow() << "Failed to grab image.";
  }

  return output;
}

} // end namespace