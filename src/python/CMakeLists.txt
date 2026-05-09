find_package(Python REQUIRED COMPONENTS Interpreter Development.Module NumPy)

pybind11_add_module(sksurgeryopencvpython bindings.cpp ndarray_converter.cpp)

target_link_libraries(sksurgeryopencvpython PRIVATE sksurgeryopencvcpp)
target_include_directories(sksurgeryopencvpython PRIVATE ${Python_NumPy_INCLUDE_DIRS})

install(TARGETS sksurgeryopencvpython
  LIBRARY DESTINATION .
  RUNTIME DESTINATION .
  MODULE DESTINATION .
)
