/*=============================================================================

  SKSURGERYOPENCVCPP: Image-guided surgery functions, in C++, using OpenCV.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.

=============================================================================*/

/*=============================================================================

  Derived from pyboostcvconverter by Gregory Kramida.
  See full package, as a git submodule in ./pyboostcvconverter.

=============================================================================*/

/* This file contains doc strings for the wrapped Python functions */
#ifndef DOCSTRINGS_H
#define DOCSTRINGS_H

auto hartley_traingulate_docstring = R"(
Triangulates a vector of un-distorted (i.e. already correction for distortion) 2D point pairs back into 3D.
 
From:
 
Triangulation, Hartley, R.I. and Sturm, P., Computer vision and image understanding, 1997.
http://www.morethantechnical.com/2012/01/04/simple-triangulation-with-opencv-from-harley-zisserman-w-code/
Price 2012, Computer Vision: Models, Learning and Inference.</li>
 
:param inputUndistortedPoints: [Nx4] matrix of 2D points, where each row is left_x, left_y, right_x, right_y.
:param leftCameraMatrix: [3x3] left camera matrix
:param rightCameraMatrix: [3x3] right camera matrix
:param leftToRightRotationMatrix: [3x3] matrix representing the rotation between camera axes
:param leftToRightTranslationVector: [3x1] translation between camera origins
:return: [Nx3] matrix of triangulated points.
)";

auto midpoint_triangulate_docstring = R"(
brief Triangulates a vector of un-distorted (i.e. already correction for distortion) 2D point pairs back into 3D.
 
Taken from: http://geomalgorithms.com/a07-_distance.html
 
:param inputUndistortedPoints: [Nx4] matrix of 2D points, where each row is left_x, left_y, right_x, right_y.
:param leftCameraMatrix: [3x3] left camera matrix
:param rightCameraMatrix: [3x3] right camera matrix
:param leftToRightRotationMatrix: [3x3] matrix representing the rotation between camera axes
:param leftToRightTranslationVector: [3x1] translation between camera origins
:return: [Nx3] matrix of triangulated points.
";

auto stoyanov_disparity_docstring = R"
Gets a disparity map image.
:param leftImage: usually RGB image
:param rightImage: usually RGB image
:return: image, same size as original left and right image.
)";

auto stoyanov_match_docstring = R"(
Gets the matching points in left images.
:param leftImage: usually RGB image
:param rightImage: usually RGB image
:return: Nx4 matrix, where the columns are x_left, y_left, x_right, y_right, i.e. 2D pixel locations.
)";

auto stoyanov_recon_docstring = R"(
Does full triangulation of matched points, returning a point cloud.
:param leftImage: usually RGB image
:param leftCameraMatrix: 3x3 matrix containing fx, fy, cx, cy
:param rightImage: usually RGB image
:param rightCameraMatrix: 3x3 matrix containing fx, fy, cx, cy
:param leftToRightRotationMatrix: 3x3 rotation matrix
:param leftToRightTranslationVector: 3x1 translation column vector
:param useHartley: if false, uses midpoint method, if true, uses hartley. See sksTriangulate.h.
:return: Nx7 matrix, where the columns are X,Y,Z (3D triangulated point), x_left, y_left, x_right, y_right (2D matches).
)";

auto mask_points_docstring = R"(
Returns points that occur at locations with non-zero pixels in image.

:param points: [Nx2] matrix of 2D points, x, y, as doubles.
:param mask: image
:return: [Mx2] matrix of masked points, x, y, as doubles.
)";

auto mask_stereo_points_docstring = R"(
Returns points that occur at locations with non-zero pixels in both leftImage and rightImage.
:param points: [Nx4] matrix of 2D points, where each row is left_x, left_y, right_x, right_y, as doubles.
:param leftMask: image
:param rightMask: image
:return: [Mx4] matrix of masked points as doubles.
)";

auto extract_dots_docstring = R"(
Extracts calibration points from an image of dots.

In this method, we pass in a distorted image. This method is
intended for use in a camera calibration routine. It assumes
you have a fairly good initial guess of the camera intrinsic matrix
and distortion coefficients. These can then be used to undistort
the image, which makes identification of the dots more robust.
The points that are returned are the exact locations as detected
in the distortedImage

The four point indexes should specify top-left, top-right,
bottom-left, bottom-right of a rectangle, where these dots
are typically 3 times larger than all the other dots,
and so are easy to identify and use as fiducials for a homography.

:param distortedImage: distorted image
:param intrinsicMatrix: good guess of the intrinsic matrix
:param distortionCoefficients: good guess of the distortion coefficients
:param gridPoints: [nx6] array of rows of id, x_pix, y_pix, x_mm, y_mm, z_mm reference point locations
:param indexesOfFourReferencePoints: [4x1] list of exactly 4 point indexes.
:return: [nx6] array of rows of id, x_pix, y_pix, x_mm, y_mm, z_mm of detected point locations
)";

#endif