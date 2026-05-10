# Product Overview

scikit-surgeryopencvcpp is a C++ library providing image-guided surgery algorithms using OpenCV, exposed to Python via pybind11. It is part of the SNAPPY platform developed at the Wellcome EPSRC Centre for Interventional and Surgical Sciences, University College London (UCL).

## Key Capabilities

- **Stereo triangulation**: Hartley/Zisserman iterative SVD method and midpoint-of-shortest-distance method. Both are OpenMP-parallelised.
- **Dot/fiducial detection**: Extracts calibration dot grids from camera images using blob detection, homography warping, and grid matching. Supports distorted and undistorted input.
- **Image masking**: Filters 2D and stereo point sets by binary mask images.

## Distribution

- Published as a Python wheel on PyPI: `pip install scikit-surgeryopencvcpp`
- The Python module is named `sksurgeryopencvpython`
- Wheels are built for Python 3.9–3.13 on Linux (x86_64), macOS (x86_64 + arm64), Windows (AMD64)
- Licensed under BSD-3

## Target Users

Researchers and developers working in computer-assisted interventions, surgical navigation, and medical image computing.

## Related Projects

- [scikit-surgeryimage](https://github.com/SciKit-Surgery/scikit-surgeryimage) — Python-only image processing (the dot detector here is a C++ port of the Python version in that project)
- [scikit-surgerycalibration](https://github.com/SciKit-Surgery/scikit-surgerycalibration) — Camera and instrument calibration using these primitives
