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

if(NOT BUILDING_GUIS)

  set(SKSURGERYOPENCVCPP_UNITY_NAME ${SKSURGERYOPENCVCPP_LIBRARY_NAME}Unity)

  set(SKSURGERYOPENCVCPP_UNITY_SRCS
    sksUnityWrapper.cpp
  )

  set(SKSURGERYOPENCVCPP_UNITY_HDRS
    sksUnityWrapper.h
  )

  # Thanks to: https://stackoverflow.com/questions/15120951/generating-a-bundle-file-with-cmake-on-mac-osx?utm_medium=organic&utm_source=google_rich_qa&utm_campaign=google_rich_qa
  add_library(${SKSURGERYOPENCVCPP_UNITY_NAME} MODULE ${SKSURGERYOPENCVCPP_UNITY_HDRS} ${SKSURGERYOPENCVCPP_UNITY_SRCS})
  if(APPLE)
    set_target_properties(${SKSURGERYOPENCVCPP_UNITY_NAME} PROPERTIES BUNDLE TRUE)
  endif()

  target_link_libraries(${SKSURGERYOPENCVCPP_UNITY_NAME} PRIVATE ${SKSURGERYOPENCVCPP_LIBRARIES} ${ALL_THIRD_PARTY_LIBRARIES})
  install(TARGETS ${SKSURGERYOPENCVCPP_UNITY_NAME} DESTINATION ${SKSURGERYOPENCVCPP_INSTALL_LIB_DIR})

  if( NOT "${SKSURGERYOPENCVCPP_UNITY_PLUGIN_DIR}" STREQUAL "")
    install(TARGETS ${SKSURGERYOPENCVCPP_UNITY_NAME} DESTINATION ${SKSURGERYOPENCVCPP_UNITY_PLUGIN_DIR})
  endif()

endif()
