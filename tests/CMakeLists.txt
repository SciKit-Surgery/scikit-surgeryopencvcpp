include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.7.1
  GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(Catch2)

list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)

set(TEST_DATA_DIR "${CMAKE_SOURCE_DIR}/tests/data")

# --- Maths tests ---
add_executable(test_maths test_maths.cpp)
target_link_libraries(test_maths PRIVATE sksurgeryopencvcpp Catch2::Catch2WithMain)
catch_discover_tests(test_maths)

# --- Triangulate tests ---
add_executable(test_triangulate test_triangulate.cpp)
target_link_libraries(test_triangulate PRIVATE sksurgeryopencvcpp Catch2::Catch2WithMain)
target_compile_definitions(test_triangulate PRIVATE TEST_DATA_DIR="${TEST_DATA_DIR}")
catch_discover_tests(test_triangulate)

# --- Masking tests ---
add_executable(test_masking test_masking.cpp)
target_link_libraries(test_masking PRIVATE sksurgeryopencvcpp Catch2::Catch2WithMain)
catch_discover_tests(test_masking)
