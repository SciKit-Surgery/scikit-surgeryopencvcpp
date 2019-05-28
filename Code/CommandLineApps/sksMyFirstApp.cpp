/*=============================================================================

  SKSURGERYOPENCVCPP: Image-guided surgery functions, in C++, using OpenCV.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.

=============================================================================*/

#include <sksExceptionMacro.h>
#include <sksVideoCapture.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>

/**
 * \brief Demo file to we can instantiate a sks::VideoCapture.
 */
int main(int argc, char** argv)
{

  int returnStatus = EXIT_FAILURE;

  try
  {
    sks::VideoCapture vc(0);
    cv::Mat image = vc.read();
    cv::imwrite("tmp.png", image);

    returnStatus = EXIT_SUCCESS;
  }
  catch (sks::Exception& e)
  {
    std::cerr << "Caught sks::Exception: " << e.GetDescription() << std::endl;
  }
  catch (std::exception& e)
  {
    std::cerr << "Caught std::exception: " << e.what() << std::endl;
  }

  return returnStatus;
}
