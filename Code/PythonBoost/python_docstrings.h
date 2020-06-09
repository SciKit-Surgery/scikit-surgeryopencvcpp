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

auto hartley_traingulate_docstring = "
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
"

auto midpoint_triangulate_docstring = "
 brief Triangulates a vector of un-distorted (i.e. already correction for distortion) 2D point pairs back into 3D.
 
 Taken from: http://geomalgorithms.com/a07-_distance.html
 
 :param inputUndistortedPoints: [Nx4] matrix of 2D points, where each row is left_x, left_y, right_x, right_y.
 :param leftCameraMatrix: [3x3] left camera matrix
 :param rightCameraMatrix: [3x3] right camera matrix
 :param leftToRightRotationMatrix: [3x3] matrix representing the rotation between camera axes
 :param leftToRightTranslationVector: [3x1] translation between camera origins
 :return: [Nx3] matrix of triangulated points.
 */"

 auto stoyanov_disparity_docstring = "
 Gets a disparity map image.
:param leftImage: usually RGB image
:param rightImage: usually RGB image
:return: image, same size as original left and right image.
 "

