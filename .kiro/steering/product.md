# Product Overview

scikit-surgeryopencvcpp is a C++ library providing image-guided surgery algorithms using OpenCV, exposed to Python via pybind11. It is part of the SNAPPY platform developed at the Wellcome EPSRC Centre for Interventional and Surgical Sciences, University College London (UCL).

## Key Capabilities

- Stereo triangulation (Hartley/Zisserman SVD method and midpoint-of-shortest-distance)
- Dot/fiducial detection for camera calibration
- Image masking utilities for point filtering

## Distribution

- Published as a Python wheel on PyPI: `pip install scikit-surgeryopencvcpp`
- The Python module is named `sksurgeryopencvpython`
- Licensed under BSD-3

## Target Users

Researchers and developers working in computer-assisted interventions, surgical navigation, and medical image computing.
