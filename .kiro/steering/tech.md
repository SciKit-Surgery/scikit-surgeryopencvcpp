# Tech Stack

## Build System

- **CMake 3.15+** with FetchContent for all dependencies (no system installs required)
- **scikit-build-core** for Python wheel packaging (declared in `pyproject.toml`)
- OpenCV is built statically from source as part of the CMake configure step

## Languages & Standards

- C++17 (`CMAKE_CXX_STANDARD 17`)
- Python 3.9–3.13 (bindings only)

## Dependencies (all fetched automatically via FetchContent)

- **OpenCV 4.10** — static build, minimal modules: core, imgproc, calib3d, features2d, imgcodecs
- **pybind11 2.13** — Python bindings
- **Catch2 v3** — C++ testing (only when `BUILD_TESTING=ON`)

## CMake Options

| Option | Default | Purpose |
|---|---|---|
| `BUILD_TESTING` | OFF | Build C++ unit tests |
| `BUILD_PYTHON_BINDINGS` | ON | Build the pybind11 Python module |

## Python Module

- Module name: `sksurgeryopencvpython`
- NumPy <-> cv::Mat conversion via custom `ndarray_converter` (pybind11 type caster)
- All functions accept/return numpy ndarrays

## CI

- **GitHub Actions** with **cibuildwheel**
- C++ tests run on every push (Linux, macOS, Windows) with FetchContent caching
- Wheel builds run only on tagged releases (`v*`) or manual workflow dispatch
- Publish to PyPI via trusted publishing (OIDC, no stored tokens)

## Common Commands

```bash
# C++ development (no Python, fastest iteration)
cmake -B build -DBUILD_TESTING=ON -DBUILD_PYTHON_BINDINGS=OFF -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --parallel
ctest --test-dir build --build-config Release --output-on-failure

# Full build with Python bindings (requires numpy, pybind11 in env)
pip install numpy pybind11
cmake -B build -DBUILD_TESTING=ON -DBUILD_PYTHON_BINDINGS=ON -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --parallel

# Build a wheel for local testing
pip wheel . -v --no-build-isolation

# Clean build (when CMake cache is stale)
rm -rf build
```

## Notes for AI Assistants

- OpenCV is fetched via FetchContent and built as a subdirectory. Its include paths must be manually specified in `src/CMakeLists.txt` because OpenCV doesn't export proper `INTERFACE_INCLUDE_DIRECTORIES` when used this way.
- The generated header `opencv2/opencv_modules.hpp` lives at `${CMAKE_BINARY_DIR}/opencv2/`, not inside `_deps/opencv-build/`.
- On Windows, the MSVC runtime is forced to static (`/MT`) to match the static OpenCV build.
- `CPU_DISPATCH` is set to empty to avoid compiling AVX2/AVX512 variants (reduces build time significantly).
- When adding new OpenCV modules, add them to `BUILD_LIST` in the top-level CMakeLists.txt and add the corresponding include path in `src/CMakeLists.txt`.
