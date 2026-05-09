/*=============================================================================

  SKSURGERYOPENCVCPP: Image-guided surgery functions, in C++, using OpenCV.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.

=============================================================================*/

#ifndef sks_exception_macro_h
#define sks_exception_macro_h

#include "exception.h"

/**
 * \brief Convenience macro to throw an sks::Exception with automatic
 * file name and line number capture.
 *
 * Usage:
 *   sksExceptionThrow() << "Something went wrong: " << details;
 *
 * The macro creates an sks::Exception object pre-populated with the
 * current source file (__FILE__) and line (__LINE__), then returns it
 * so that additional context can be streamed in via operator<<.
 */
#define sksExceptionThrow() throw sks::Exception(__FILE__, __LINE__)

#endif
