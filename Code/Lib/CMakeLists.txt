#/*============================================================================
#
#  SKSURGERYOPENCVCPP: Image-guided surgery functions, in C++, using OpenCV.
#
#  Copyright (c) University College London (UCL). All rights reserved.
#
#  This software is distributed WITHOUT ANY WARRANTY; without even
#  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
#  PURPOSE.
#
#  See LICENSE.txt in the top level directory for details.
#
#============================================================================*/

set(SKSURGERYOPENCVCPP_LIBRARY_NAME sksurgeryopencvcpp)

set(SKSURGERYOPENCVCPP_LIBRARY_SRCS
  sksBasicTypes.cpp
  sksMyFunctions.cpp
  sksException.cpp
  sksMaths.cpp
  sksTriangulate.cpp
)

set(SKSURGERYOPENCVCPP_LIBRARY_HDRS
  sksExceptionMacro.h
)

add_library(${SKSURGERYOPENCVCPP_LIBRARY_NAME} ${SKSURGERYOPENCVCPP_LIBRARY_HDRS} ${SKSURGERYOPENCVCPP_LIBRARY_SRCS})

if(BUILD_SHARED_LIBS)
  if(WIN32)
    add_definitions(-DSKSURGERYOPENCVCPP_WINDOWS_EXPORT)
  endif(WIN32)
endif(BUILD_SHARED_LIBS)

target_link_libraries(${SKSURGERYOPENCVCPP_LIBRARY_NAME} PRIVATE ${ALL_THIRD_PARTY_LIBRARIES})

SKSURGERYOPENCVCPP_INSTALL_HEADERS()
SKSURGERYOPENCVCPP_INSTALL_LIBRARY(${SKSURGERYOPENCVCPP_LIBRARY_NAME})

