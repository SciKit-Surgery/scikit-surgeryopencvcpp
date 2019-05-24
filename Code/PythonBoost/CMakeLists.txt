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

  set(SKSURGERYOPENCVCPP_PYTHON_SRCS)

  if (BUILD_OpenCV)
    list(APPEND SKSURGERYOPENCVCPP_PYTHON_SRCS sksLibPythonWithOpenCV.cpp) #Rename as appropriate.
    message("Including pyboostcvconverter, thanks to Gregory Kramida.")

    execute_process(
      COMMAND "${PYTHON_EXECUTABLE}" -c
              "from __future__ import print_function\ntry: import numpy; print(numpy.get_include(), end='')\nexcept:pass\n"
              OUTPUT_VARIABLE NumPy_INCLUDE_DIR)
    execute_process(
      COMMAND "${PYTHON_EXECUTABLE}" -c
              "from __future__ import print_function\ntry: import numpy; print(numpy.__version__, end='')\nexcept:pass\n"
      OUTPUT_VARIABLE NumPy_VERSION)
    message("Found NumPy_INCLUDE_DIR=${NumPy_INCLUDE_DIR}, NumPy_VERSION=${NumPy_VERSION}")
    include_directories(${NumPy_INCLUDE_DIR})
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/pyboostcvconverter/include)
    list(APPEND SKSURGERYOPENCVCPP_PYTHON_SRCS pyboostcvconverter/src/pyboost_cv4_converter.cpp)
  else()
    list(APPEND SKSURGERYOPENCVCPP_PYTHON_SRCS sksLibPython.cpp) # Rename as appropriate.
  endif()

  link_libraries(${ALL_LIBRARIES})

  PYTHON_ADD_MODULE(${SKSURGERYOPENCVCPP_PYTHON_MODULE_NAME} ${SKSURGERYOPENCVCPP_PYTHON_SRCS})
  if(APPLE)
    set_target_properties(${SKSURGERYOPENCVCPP_PYTHON_MODULE_NAME} PROPERTIES LINK_FLAGS "-undefined dynamic_lookup")
  endif()

  install(TARGETS ${SKSURGERYOPENCVCPP_PYTHON_MODULE_NAME} DESTINATION ${SKSURGERYOPENCVCPP_INSTALL_LIB_DIR})

endif()
