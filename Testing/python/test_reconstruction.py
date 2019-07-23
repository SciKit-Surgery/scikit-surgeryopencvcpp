# -*- coding: utf-8 -*-

import pytest
import numpy as np
import datetime
import six
import sksurgeryopencvpython as cvpy
import cv2


def test_reconstruction():

    # Example from 2nd silicon heart phantom dataset from Hamlyn. http://hamlyn.doc.ic.ac.uk/vision/.
    # Technically, we should undistort the image first before reconstructing. 
    left_intrinsics_file = 'Testing/Data/reconstruction/calib.left.intrinsic.txt'
    left_intrinsics = np.loadtxt(left_intrinsics_file)

    right_intrinsics_file = 'Testing/Data/reconstruction/calib.right.intrinsic.txt'
    right_intrinsics = np.loadtxt(right_intrinsics_file)

    l2r_file = 'Testing/Data/reconstruction/calib.l2r.4x4'
    l2r = np.loadtxt(l2r_file)

    rotation_matrix = l2r[0:3, 0:3]
    translation_vector = l2r[0:3, 3:4]

    left_image = cv2.imread('Testing/Data/reconstruction/f7_dynamic_deint_L_0100.png')
    right_image = cv2.imread('Testing/Data/reconstruction/f7_dynamic_deint_R_0100.png')

    start_stoyanov_midpoint = datetime.datetime.now()

    points = cvpy.reconstruct_points_using_stoyanov_2010(left_image,
                                                         left_intrinsics,
                                                         right_image,
                                                         right_intrinsics,
                                                         rotation_matrix,
                                                         translation_vector,
                                                         False
                                                         )

    end_stoyanov_midpoint = datetime.datetime.now()
    number_of_points = points.shape[0]

    six.print_('Stoyanov 2010, using midpoint triangulation, in python=:'
               + str((end_stoyanov_midpoint - start_stoyanov_midpoint).total_seconds()))
    assert points.shape[1] == 7

    start_stoyanov_hartley = datetime.datetime.now()

    points = cvpy.reconstruct_points_using_stoyanov_2010(left_image,
                                                         left_intrinsics,
                                                         right_image,
                                                         right_intrinsics,
                                                         rotation_matrix,
                                                         translation_vector,
                                                         True
                                                         )

    end_stoyanov_hartley = datetime.datetime.now()

    six.print_('Stoyanov 2010, using Hartley triangulation, in python=:'
               + str((end_stoyanov_hartley - start_stoyanov_hartley).total_seconds()))
    assert points.shape[0] == number_of_points # can only check for consistency.
    assert points.shape[1] == 7
