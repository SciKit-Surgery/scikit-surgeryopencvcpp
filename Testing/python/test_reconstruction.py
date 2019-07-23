# -*- coding: utf-8 -*-

import pytest
import numpy as np
import datetime
import six
import sksurgeryopencvpython as cvpy
import cv2


def test_reconstruction():

    left_intrinsics_file = 'Testing/Data/calibration/calib.left.intrinsic.txt'
    left_intrinsics = np.loadtxt(left_intrinsics_file)

    right_intrinsics_file = 'Testing/Data/calibration/calib.right.intrinsic.txt'
    right_intrinsics = np.loadtxt(right_intrinsics_file)

    l2r_file = 'Testing/Data/calibration/calib.l2r.4x4'
    l2r = np.loadtxt(l2r_file)

    rotation_matrix = l2r[0:3, 0:3]
    translation_vector = l2r[0:3, 3:4]

    left_image = cv2.imread('Testing/Data/calibration/left-1095-undistorted.png')
    right_image = cv2.imread('Testing/Data/calibration/right-1095-undistorted.png')

    start_stoyanov = datetime.datetime.now()

    points = cvpy.reconstruct_points_using_stoyanov_2010(left_image,
                                                         left_intrinsics,
                                                         right_image,
                                                         right_intrinsics,
                                                         rotation_matrix,
                                                         translation_vector
                                                         )

    end_stoyanov = datetime.datetime.now()

    six.print_('Stoyanov 2010 in python=:' + str((end_stoyanov - start_stoyanov).total_seconds()))
    assert points.shape[0] == 237864
    assert points.shape[1] == 3

