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

configure_file(${CMAKE_SOURCE_DIR}/Usesksurgeryopencvcpp.cmake.in ${CMAKE_BINARY_DIR}/Usesksurgeryopencvcpp.cmake @ONLY IMMEDIATE)
configure_file(${CMAKE_SOURCE_DIR}/sksurgeryopencvcppConfig.cmake.in ${CMAKE_BINARY_DIR}/sksurgeryopencvcppConfig.cmake @ONLY IMMEDIATE)
if(NOT BUILDING_GUIS)
  install(FILES ${CMAKE_BINARY_DIR}/Usesksurgeryopencvcpp.cmake DESTINATION . COMPONENT CONFIG)
  install(FILES ${CMAKE_BINARY_DIR}/sksurgeryopencvcppConfig.cmake DESTINATION . COMPONENT CONFIG)
endif()
