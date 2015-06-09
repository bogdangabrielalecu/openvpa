/***************************************************************
 * Name:      BinarizationChrominance.cpp
 * Purpose:   Code for Binarization Class Extension
 * Author:    Bogdan Alecu
 * Created:   2011-09-16
 * Copyright: Bogdan Alecu
 * License:   GNU GPL
 **************************************************************/

#include "binarization/BinarizationChrominance.h"
#include "Detector.h"

namespace openvpa {

    BinarizationChrominance::BinarizationChrominance() : Binarization::Binarization() {
        reset();
    }

    BinarizationChrominance::~BinarizationChrominance() {
        //
    }

    bool BinarizationChrominance::reset() {
        return true;
    }

    Mat& BinarizationChrominance::get(Mat& frm) {
        Mat ycbcr_frame;
        ycbcr_frame = Detector::rgb2ycbcr(frm);
        vector<Mat> planes;
        split(ycbcr_frame, planes);
        Mat sum_planes = planes[1] + planes[2];
        int my_level = 2 * inner_level;
        if (BINARIZATION_WAY_DIRECT == way()) {
            binarized_frame = sum_planes > my_level;
            return binarized_frame;
        }
        binarized_frame = sum_planes <= my_level;
        return binarized_frame;
    }
}
