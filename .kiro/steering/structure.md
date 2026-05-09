# Project Structure

```
scikit-surgeryopencvcpp/
├── CMakeLists.txt              # Top-level CMake (FetchContent for OpenCV, pybind11)
├── pyproject.toml              # Python packaging (scikit-build-core)
├── src/                        # C++ library sources
│   ├── CMakeLists.txt          # Static library target
│   ├── exception.h / .cpp     # Custom exception class
│   ├── exception_macro.h      # sksExceptionThrow() macro
│   ├── maths.h / .cpp         # Geometry utilities (norm, cross, distance)
│   ├── validate.h / .cpp      # Input validation helpers
│   ├── triangulate.h / .cpp   # Stereo triangulation (Hartley, midpoint)
│   ├── stoyanov2010.h / .cpp  # Quasi-dense stereo reconstruction
│   ├── masking.h / .cpp       # Point filtering by mask images
│   ├── dot_detection.h / .cpp # Calibration dot extraction
│   └── python/                # pybind11 bindings
│       ├── CMakeLists.txt
│       ├── bindings.cpp       # Module definition
│       ├── ndarray_converter.h / .cpp  # NumPy <-> cv::Mat conversion
├── tests/                      # C++ unit tests (Catch2 v3)
│   ├── CMakeLists.txt
│   ├── test_maths.cpp
│   ├── test_triangulate.cpp
│   └── test_masking.cpp
├── .github/workflows/
│   └── build.yml              # CI: cibuildwheel + C++ tests
└── LICENSE.txt
```

## Conventions

- **Namespace**: All library code lives in `sks`
- **Header guards**: `#ifndef sks_filename_h`
- **File naming**: lowercase with underscores (e.g., `dot_detection.cpp`)
- **Functions**: PascalCase (e.g., `TriangulatePointsUsingHartley`)
- **Variables**: camelCase
- **Tests**: Named `test_<feature>.cpp`, use Catch2 `TEST_CASE` with tags like `"[triangulate]"`
- **Exception pattern**: `sksExceptionThrow() << "message"`
- **OpenMP**: Used for parallelising loops over point sets

## Branching Workflow

1. Raise a GitHub issue
2. Fork the repository
3. Create branch: `<issue-number>-<short-description>`
4. Implement, push, create PR
5. Code review and merge to `main`
