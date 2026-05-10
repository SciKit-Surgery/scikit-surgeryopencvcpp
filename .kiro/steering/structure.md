# Project Structure

```
scikit-surgeryopencvcpp/
├── CMakeLists.txt              # Top-level CMake (FetchContent for OpenCV, pybind11)
├── pyproject.toml              # Python packaging (scikit-build-core) + cibuildwheel config
├── README.md                   # User-facing documentation
├── LICENSE.txt                 # BSD-3 license
├── .gitignore
├── src/                        # All C++ source code
│   ├── CMakeLists.txt          # Static library target (sksurgeryopencvcpp)
│   ├── exception.h / .cpp     # Custom exception class with file/line capture
│   ├── exception_macro.h      # sksExceptionThrow() convenience macro
│   ├── maths.h / .cpp         # Geometry: norm, cross/dot product, line distance, RMS
│   ├── validate.h / .cpp      # Stereo parameter dimension validation
│   ├── triangulate.h / .cpp   # Stereo triangulation (Hartley SVD + midpoint methods)
│   ├── masking.h / .cpp       # Point filtering by binary mask images
│   ├── dots.h / .cpp          # Calibration dot grid extraction
│   └── python/                # pybind11 bindings
│       ├── CMakeLists.txt     # Python module target (sksurgeryopencvpython)
│       ├── bindings.cpp       # Module definition, all exposed functions
│       ├── ndarray_converter.h / .cpp  # NumPy <-> cv::Mat type caster
├── tests/                      # C++ unit tests (Catch2 v3)
│   ├── CMakeLists.txt         # Test targets, fetches Catch2
│   ├── test_maths.cpp
│   ├── test_triangulate.cpp
│   └── test_masking.cpp
├── .github/workflows/
│   └── build.yml              # CI: C++ tests + cibuildwheel + PyPI publish
└── .kiro/steering/            # AI steering files (this directory)
```

## Naming Conventions

- **Namespace**: All library code lives in `sks`
- **Header guards**: `#ifndef sks_filename_h` / `#define sks_filename_h`
- **File naming**: lowercase, single words where possible (e.g., `dots.cpp`, `maths.cpp`, `triangulate.cpp`). Use underscores only for multi-word compound names (e.g., `exception_macro.h`).
- **Functions**: PascalCase (e.g., `TriangulatePointsUsingHartley`, `ExtractDots`)
- **Local variables**: camelCase (e.g., `numberOfPoints`, `leftCameraMatrix`)
- **Constants**: UPPER_SNAKE_CASE in anonymous namespaces (e.g., `RMS_TOLERANCE`)
- **Tests**: Files named `test_<feature>.cpp`, use Catch2 `TEST_CASE("description", "[tag]")`
- **Python API**: snake_case function names (e.g., `triangulate_points_using_hartley`)

## Code Patterns

- **Exceptions**: Use `sksExceptionThrow() << "message"` — captures file/line automatically
- **Input validation**: Dedicated `Validate*` functions that throw on invalid dimensions
- **OpenMP**: `#pragma omp parallel` / `#pragma omp for` for parallelising loops over point arrays
- **cv::Mat types**: Functions generally accept CV_64FC1 (double) or CV_32FC1 (float) and handle both
- **Static helpers**: Internal functions use `static` or anonymous namespaces, not exposed in headers
- **Python bindings**: One `m.def()` per function with full docstring, `py::arg()` for all parameters

## Branching Workflow

1. Raise a GitHub issue
2. Fork the repository
3. Create branch: `<issue-number>-<short-description>`
4. Implement, push, create PR
5. Code review and merge to `main`

## Adding New Functionality

1. Create `src/newfeature.h` and `src/newfeature.cpp`
2. Add `.cpp` to the source list in `src/CMakeLists.txt`
3. Add any new OpenCV modules to `BUILD_LIST` in the top-level `CMakeLists.txt`
4. Add the module's include path to `target_include_directories` in `src/CMakeLists.txt`
5. Add a `m.def()` binding in `src/python/bindings.cpp`
6. Add a test in `tests/test_newfeature.cpp` and register it in `tests/CMakeLists.txt`
