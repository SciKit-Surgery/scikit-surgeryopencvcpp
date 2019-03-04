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

  if (NOT SKSURGERYOPENCVCPP_PYTHON_MODULE_NAME OR SKSURGERYOPENCVCPP_PYTHON_MODULE_NAME STREQUAL "")
    set(SKSURGERYOPENCVCPP_PYTHON_MODULE_NAME "sksurgeryopencvpython" CACHE STRING "Basename for python module." FORCE)
  endif()

  set(SKSURGERYOPENCVCPP_PYTHON_SRCS
    sksLibPython.cpp
  )
  add_subdirectory(pybind11)
  link_libraries(${ALL_LIBRARIES})
  pybind11_add_module(${SKSURGERYOPENCVCPP_PYTHON_MODULE_NAME} ${SKSURGERYOPENCVCPP_PYTHON_SRCS})
  if(APPLE)
    set_target_properties(${SKSURGERYOPENCVCPP_PYTHON_MODULE_NAME} PROPERTIES LINK_FLAGS "-undefined dynamic_lookup")
  endif()
  install(TARGETS ${SKSURGERYOPENCVCPP_PYTHON_MODULE_NAME} DESTINATION ${SKSURGERYOPENCVCPP_INSTALL_LIB_DIR})

endif()
