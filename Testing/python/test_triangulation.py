# -*- coding: utf-8 -*-

import pytest
import numpy as np
import datetime
import six
import sksurgeryopencvpython as cvpy


def test_triangulate_points():

    left_intrinsics_file = 'Testing/data/triangulation/left_intrinsic.txt'
    left_intrinsics = np.loadtxt(left_intrinsics_file)

    right_intrinsics_file = 'Testing/data/triangulation/right_intrinsic.txt'
    right_intrinsics = np.loadtxt(right_intrinsics_file)

    l2r_file = 'Testing/data/triangulation/l2r.txt'
    l2r = np.loadtxt(l2r_file)

    image_points_file = 'Testing/data/triangulation/image_points.txt'
    image_points = np.loadtxt(image_points_file)

    rotation_matrix = l2r[0:3, 0:3]
    translation_vector = l2r[0:3, 3:4]

    start_hartley = datetime.datetime.now()

    hartley = cvpy.triangulate_points_using_hartley(image_points,
                                                    left_intrinsics,
                                                    right_intrinsics,
                                                    rotation_matrix,
                                                    translation_vector
                                                    )
    end_hartley = datetime.datetime.now()

    midpoint = cvpy.triangulate_points_using_midpoint(image_points,
                                                      left_intrinsics,
                                                      right_intrinsics,
                                                      rotation_matrix,
                                                      translation_vector
                                                      )

    end_midpoint = datetime.datetime.now()

    assert image_points.shape[0] == 204800
    assert hartley.shape[0] == 204800
    assert midpoint.shape[0] == 204800
    assert np.allclose(hartley, midpoint, rtol=0.05, atol=0.25)

    six.print_('Hartley=:' + str((end_hartley - start_hartley).total_seconds()))
    six.print_('Midpoint=:' + str((end_midpoint - end_hartley).total_seconds()))
