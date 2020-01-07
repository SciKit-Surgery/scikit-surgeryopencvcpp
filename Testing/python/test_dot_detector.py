# -*- coding: utf-8 -*-

"""
Tests for dot detector.
"""

import os
import datetime
import logging
import cv2 as cv2
import numpy as np
import pytest
import sksurgeryopencvpython as sks


def __setup_dotty_calibration_model():
    number_of_points = 18 * 25
    model_points = np.zeros((number_of_points, 6))
    counter = 0
    for y_index in range(18):
        for x_index in range(25):
            model_points[counter][0] = counter
            model_points[counter][1] = (x_index + 1) * 50
            model_points[counter][2] = (y_index + 1) * 50
            model_points[counter][3] = x_index * 5
            model_points[counter][4] = y_index * 5
            model_points[counter][5] = 0
            counter = counter + 1
    return model_points


def __check_real_image(image_file_name,
                       intrinsics_file_name,
                       distortion_file_name
                       ):
    logging.basicConfig(level=logging.DEBUG)
    image = cv2.imread(image_file_name)
    greyscale = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    intrinsics = np.loadtxt(intrinsics_file_name)
    distortion = np.loadtxt(distortion_file_name)
    model = __setup_dotty_calibration_model()
    fiducial_indexes = np.ones((4, 1), dtype=int)
    fiducial_indexes[0][0] = 133
    fiducial_indexes[1][0] = 141
    fiducial_indexes[2][0] = 308
    fiducial_indexes[3][0] = 316

    time_before = datetime.datetime.now()

    results = sks.extract_dots(greyscale,
                               intrinsics,
                               distortion,
                               model,
                               fiducial_indexes
                               )
    time_after = datetime.datetime.now()
    time_diff = time_after - time_before

    print("__check_real_image:time_diff=" + str(time_diff))

    ids = results[:, 0]
    image_points = results[:, 1:3]

    font = cv2.FONT_HERSHEY_SIMPLEX
    for counter in range(ids.shape[0]):
        cv2.putText(image, str(ids[counter]), (int(image_points[counter][0]), int(image_points[counter][1])), font, 0.5, (0, 255, 0), 2, cv2.LINE_AA)
    split_path = os.path.splitext(image_file_name)
    previous_dir = os.path.dirname(split_path[0])
    previous_dir = os.path.basename(previous_dir)
    base_name = os.path.basename(split_path[0])
    test_output = 'Testing/output'
    output_file = os.path.join(test_output, base_name + '_' + previous_dir + '_labelled.png')
    if not os.path.exists(test_output):
        logging.debug("Creating directory: %s", test_output)
        os.makedirs(test_output)
    cv2.imwrite(output_file, image)
    return ids.shape[0]


def test_dotty_uncalibrated_1():
    number_of_points = __check_real_image('Testing/Data/calib-ucl-circles/snapshots-uncalibrated/08_54_13/left_image.png',
                                          'Testing/Data/calib-ucl-circles/calib.left.intrinsics.txt',
                                          'Testing/Data/calib-ucl-circles/calib.left.distortion.txt',
                                          )
    assert(374 == number_of_points)


def test_dotty_uncalibrated_2():
    number_of_points = __check_real_image('Testing/Data/calib-ucl-circles/snapshots-uncalibrated/08_54_13/right_image.png',
                                          'Testing/Data/calib-ucl-circles/calib.right.intrinsics.txt',
                                          'Testing/Data/calib-ucl-circles/calib.right.distortion.txt',
                                          )
    assert(363 == number_of_points)


def test_dotty_uncalibrated_3():
    number_of_points = __check_real_image('Testing/Data/calib-ucl-circles/snapshots-uncalibrated/08_54_23/left_image.png',
                                          'Testing/Data/calib-ucl-circles/calib.left.intrinsics.txt',
                                          'Testing/Data/calib-ucl-circles/calib.left.distortion.txt',
                                          )
    assert(375 == number_of_points)


def test_dotty_uncalibrated_4():
    number_of_points = __check_real_image('Testing/Data/calib-ucl-circles/snapshots-uncalibrated/08_54_23/right_image.png',
                                          'Testing/Data/calib-ucl-circles/calib.right.intrinsics.txt',
                                          'Testing/Data/calib-ucl-circles/calib.right.distortion.txt',
                                          )
    assert(364 == number_of_points)


def test_dotty_uncalibrated_5():
    number_of_points = __check_real_image('Testing/Data/calib-ucl-circles/snapshots-uncalibrated/08_54_29/left_image.png',
                                          'Testing/Data/calib-ucl-circles/calib.left.intrinsics.txt',
                                          'Testing/Data/calib-ucl-circles/calib.left.distortion.txt',
                                          )
    assert(354 == number_of_points)


def test_dotty_uncalibrated_6():
    number_of_points = __check_real_image('Testing/Data/calib-ucl-circles/snapshots-uncalibrated/08_54_29/right_image.png',
                                          'Testing/Data/calib-ucl-circles/calib.right.intrinsics.txt',
                                          'Testing/Data/calib-ucl-circles/calib.right.distortion.txt',
                                          )
    assert(353 == number_of_points)


def test_dotty_uncalibrated_7():
    number_of_points = __check_real_image('Testing/Data/calib-ucl-circles/snapshots-uncalibrated/08_54_39/left_image.png',
                                          'Testing/Data/calib-ucl-circles/calib.left.intrinsics.txt',
                                          'Testing/Data/calib-ucl-circles/calib.left.distortion.txt',
                                          )
    assert(377 == number_of_points)


def test_dotty_uncalibrated_8():
    number_of_points = __check_real_image('Testing/Data/calib-ucl-circles/snapshots-uncalibrated/08_54_39/right_image.png',
                                          'Testing/Data/calib-ucl-circles/calib.right.intrinsics.txt',
                                          'Testing/Data/calib-ucl-circles/calib.right.distortion.txt',
                                          )
    assert(362 == number_of_points)


def test_dotty_uncalibrated_9():
    number_of_points = __check_real_image('Testing/Data/calib-ucl-circles/snapshots-uncalibrated/08_54_44/left_image.png',
                                          'Testing/Data/calib-ucl-circles/calib.left.intrinsics.txt',
                                          'Testing/Data/calib-ucl-circles/calib.left.distortion.txt',
                                          )
    assert(403 == number_of_points)


def test_dotty_uncalibrated_10():
    number_of_points = __check_real_image('Testing/Data/calib-ucl-circles/snapshots-uncalibrated/08_54_44/right_image.png',
                                          'Testing/Data/calib-ucl-circles/calib.right.intrinsics.txt',
                                          'Testing/Data/calib-ucl-circles/calib.right.distortion.txt',
                                          )
    assert(391 == number_of_points)


def test_dotty_uncalibrated_11():
    number_of_points = __check_real_image('Testing/Data/calib-ucl-circles/snapshots-uncalibrated/08_54_51/left_image.png',
                                          'Testing/Data/calib-ucl-circles/calib.left.intrinsics.txt',
                                          'Testing/Data/calib-ucl-circles/calib.left.distortion.txt',
                                          )
    assert(351 == number_of_points)


def test_dotty_uncalibrated_12():
    number_of_points = __check_real_image('Testing/Data/calib-ucl-circles/snapshots-uncalibrated/08_54_51/right_image.png',
                                          'Testing/Data/calib-ucl-circles/calib.right.intrinsics.txt',
                                          'Testing/Data/calib-ucl-circles/calib.right.distortion.txt',
                                          )
    assert(344 == number_of_points)


def test_dotty_uncalibrated_13():
    number_of_points = __check_real_image('Testing/Data/calib-ucl-circles/snapshots-uncalibrated/08_54_57/left_image.png',
                                          'Testing/Data/calib-ucl-circles/calib.left.intrinsics.txt',
                                          'Testing/Data/calib-ucl-circles/calib.left.distortion.txt',
                                          )
    assert(358 == number_of_points)


def test_dotty_uncalibrated_14():
    number_of_points = __check_real_image('Testing/Data/calib-ucl-circles/snapshots-uncalibrated/08_54_57/right_image.png',
                                          'Testing/Data/calib-ucl-circles/calib.right.intrinsics.txt',
                                          'Testing/Data/calib-ucl-circles/calib.right.distortion.txt',
                                          )
    assert(354 == number_of_points)


def test_dotty_uncalibrated_15():
    number_of_points = __check_real_image('Testing/Data/calib-ucl-circles/snapshots-uncalibrated/08_55_08/left_image.png',
                                          'Testing/Data/calib-ucl-circles/calib.left.intrinsics.txt',
                                          'Testing/Data/calib-ucl-circles/calib.left.distortion.txt',
                                          )
    assert(358 == number_of_points)


def test_dotty_uncalibrated_16():
    number_of_points = __check_real_image('Testing/Data/calib-ucl-circles/snapshots-uncalibrated/08_55_08/right_image.png',
                                          'Testing/Data/calib-ucl-circles/calib.right.intrinsics.txt',
                                          'Testing/Data/calib-ucl-circles/calib.right.distortion.txt',
                                          )
    assert(363 == number_of_points)


def test_dotty_uncalibrated_17():
    number_of_points = __check_real_image('Testing/Data/calib-ucl-circles/snapshots-uncalibrated/08_55_13/left_image.png',
                                          'Testing/Data/calib-ucl-circles/calib.left.intrinsics.txt',
                                          'Testing/Data/calib-ucl-circles/calib.left.distortion.txt',
                                          )
    assert(382 == number_of_points)


def test_dotty_uncalibrated_18():
    number_of_points = __check_real_image('Testing/Data/calib-ucl-circles/snapshots-uncalibrated/08_55_13/right_image.png',
                                          'Testing/Data/calib-ucl-circles/calib.right.intrinsics.txt',
                                          'Testing/Data/calib-ucl-circles/calib.right.distortion.txt',
                                          )
    assert(373 == number_of_points)


def test_dotty_uncalibrated_19():
    number_of_points = __check_real_image('Testing/Data/calib-ucl-circles/snapshots-uncalibrated/08_55_20/left_image.png',
                                          'Testing/Data/calib-ucl-circles/calib.left.intrinsics.txt',
                                          'Testing/Data/calib-ucl-circles/calib.left.distortion.txt',
                                          )
    assert(364 == number_of_points)


def test_dotty_uncalibrated_20():
    number_of_points = __check_real_image('Testing/Data/calib-ucl-circles/snapshots-uncalibrated/08_55_20/right_image.png',
                                          'Testing/Data/calib-ucl-circles/calib.right.intrinsics.txt',
                                          'Testing/Data/calib-ucl-circles/calib.right.distortion.txt',
                                          )
    assert(345 == number_of_points)

