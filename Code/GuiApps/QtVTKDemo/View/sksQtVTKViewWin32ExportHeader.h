/*=============================================================================

  SKSURGERYOPENCVCPP: Image-guided surgery functions, in C++, using OpenCV.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.

=============================================================================*/

#ifndef sksQtVTKViewWin32ExportHeader_h
#define sksQtVTKViewWin32ExportHeader_h

/**
* \file sksQtVTKViewWin32ExportHeader.h
* \brief Header to sort Windows dllexport/dllimport.
*/

#if (defined(_WIN32) || defined(WIN32)) && !defined(SKSURGERYOPENCVCPP_STATIC)
  #ifdef SKSURGERYOPENCVCPP_QTVTKVIEW_WINDOWS_EXPORT
    #define SKSURGERYOPENCVCPP_QTVTKVIEWWINEXPORT __declspec(dllexport)
  #else
    #define SKSURGERYOPENCVCPP_QTVTKVIEWWINEXPORT __declspec(dllimport)
  #endif
#else
  #define SKSURGERYOPENCVCPP_QTVTKVIEWWINEXPORT
#endif

#endif
