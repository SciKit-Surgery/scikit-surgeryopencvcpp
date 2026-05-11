# scikit-surgeryopencvcpp

Image-guided surgery functions, in C++, using OpenCV, wrapped in Python.

Part of the [SNAPPY](https://weisslab.cs.ucl.ac.uk/WEISS/PlatformManagement/SNAPPY/wikis/home) software project,
developed at the [Wellcome EPSRC Centre for Interventional and Surgical Sciences](http://www.ucl.ac.uk/weiss),
[University College London (UCL)](http://www.ucl.ac.uk/).

## Features

* Stereo triangulation (Hartley/Zisserman SVD method and midpoint-of-shortest-distance)
* Calibration dot/fiducial detection
* Point masking utilities

## Installing

```bash
pip install scikit-surgeryopencvcpp
```

## Building from source

Requires CMake 3.15+ and a C++17 compiler. All dependencies (OpenCV, pybind11) are fetched automatically via CMake FetchContent.

### Build a wheel (for pip install)

```bash
pip install build
python -m build --wheel
```

This produces a wheel in `dist/` that you can install with:

```bash
pip install dist/scikit_surgeryopencvcpp-*.whl
```

For a faster development cycle (reuses your existing build artifacts):

```bash
pip wheel . -v --no-build-isolation
```

### C++ development (no Python)

```bash
cmake -B build -DBUILD_TESTING=ON -DBUILD_PYTHON_BINDINGS=OFF -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --parallel
ctest --test-dir build --build-config Release --output-on-failure
```

### Full build with Python bindings

Requires NumPy and pybind11 in your environment:

```bash
pip install numpy pybind11
cmake -B build -DBUILD_TESTING=ON -DBUILD_PYTHON_BINDINGS=ON -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --parallel
ctest --test-dir build --build-config Release --output-on-failure
```

## Python API

```python
import numpy as np
import sksurgeryopencvpython as sks

# Triangulate stereo point pairs
points_3d = sks.triangulate_points_using_hartley(
    image_points,       # Nx4 array (left_x, left_y, right_x, right_y)
    left_intrinsics,    # 3x3 camera matrix
    right_intrinsics,   # 3x3 camera matrix
    rotation_matrix,    # 3x3 left-to-right rotation
    translation_vector  # 3x1 left-to-right translation
)
```

## Releasing

1. Update ```pyproject.toml``` with the version number
2. Tag on main branch
3. Push tag

## Contributing

1. Raise an issue on GitHub
2. Fork the repository
3. Create a feature branch: `<issue-number>-<short-description>`
4. Submit a pull request

## License

Copyright 2018 University College London.
Released under the [BSD-3 license](LICENSE.txt).

## Acknowledgements

Supported by [Wellcome](https://wellcome.ac.uk/) and the [EPSRC](https://www.epsrc.ac.uk/).
