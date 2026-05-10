# Python is already found in the top-level CMakeLists.txt

pybind11_add_module(sksurgeryopencvpython bindings.cpp ndarray_converter.cpp)

target_link_libraries(sksurgeryopencvpython PRIVATE sksurgeryopencvcpp)
target_include_directories(sksurgeryopencvpython PRIVATE ${Python_NumPy_INCLUDE_DIRS})

# scikit-build-core sets CMAKE_INSTALL_PREFIX; DESTINATION . puts the module
# at the wheel root (site-packages level).
install(TARGETS sksurgeryopencvpython DESTINATION .)
