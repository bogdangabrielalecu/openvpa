/***************************************************************
 * Name:      BinarizationMean.cpp
 * Purpose:   Code for Binarization Class Extension
 * Author:    Bogdan Alecu
 * Created:   2011-09-16
 * Copyright: Bogdan Alecu
 * License:   GNU GPL
 **************************************************************/

#include "binarization/BinarizationMean.h"
#include "Detector.h"

namespace openvpa {

    BinarizationMean::BinarizationMean() : Binarization::Binarization() {
        reset();
    }

    BinarizationMean::~BinarizationMean() {
        //
    }

    bool BinarizationMean::reset() {
        return true;
    }

    Mat& BinarizationMean::get(Mat& frm) {
        vector<Mat> planes;
        split(frm, planes);
        Mat sum_planes = planes[0] + planes[1] + planes[2];
        int my_level = 3 * inner_level;
        if (BINARIZATION_WAY_DIRECT == way()) {
            binarized_frame = sum_planes > my_level;
            return binarized_frame;
        }
        binarized_frame = sum_planes <= my_level;
        return binarized_frame;
    }
}
