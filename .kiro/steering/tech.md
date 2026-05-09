# Tech Stack

## Build System

- **CMake 3.15+** with FetchContent for all dependencies
- **scikit-build-core** for Python wheel packaging (declared in `pyproject.toml`)
- No SuperBuild — dependencies are fetched and built inline

## Languages & Standards

- C++17
- Python 3.9–3.13

## Dependencies (all fetched automatically)

- **OpenCV 4.10** (static, minimal: core, imgproc, calib3d, features2d, imgcodecs, stereo)
- **opencv_contrib** (for the stereo module)
- **pybind11 2.13** (Python bindings)
- **Catch2 v3** (C++ testing, only when BUILD_TESTING=ON)

## Python Module

- Module name: `sksurgeryopencvpython`
- NumPy <-> cv::Mat conversion via custom `ndarray_converter`

## CI

- **GitHub Actions** with **cibuildwheel**
- Platforms: Linux (x86_64), macOS (x86_64 + arm64), Windows (AMD64)
- OpenCV build cached between runs

## Common Commands

```bash
# Configure and build (with tests)
cmake -B build -DBUILD_TESTING=ON -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --parallel

# Run C++ tests
ctest --test-dir build --build-config Release --output-on-failure

# Build a Python wheel locally
pip install build
python -m build --wheel

# Build wheel with verbose CMake output
pip wheel . -v --no-build-isolation
```
