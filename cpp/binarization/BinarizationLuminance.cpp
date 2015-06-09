/***************************************************************
 * Name:      BinarizationLuminance.cpp
 * Purpose:   Code for Binarization Class Extension
 * Author:    Bogdan Alecu
 * Created:   2011-09-16
 * Copyright: Bogdan Alecu
 * License:   GNU GPL
 **************************************************************/

#include "binarization/BinarizationLuminance.h"
#include "Detector.h"

namespace openvpa {

    BinarizationLuminance::BinarizationLuminance() : Binarization::Binarization() {
        reset();
    }

    BinarizationLuminance::~BinarizationLuminance() {
        //
    }

    bool BinarizationLuminance::reset() {
        return true;
    }

    Mat& BinarizationLuminance::get(Mat& frm) {
        Mat gray_frame;
        gray_frame = Detector::rgb2y(frm);
        if (BINARIZATION_WAY_DIRECT == way()) {
            binarized_frame = gray_frame > inner_level;
            return binarized_frame;
        }
        binarized_frame = gray_frame <= inner_level;
        return binarized_frame;
    }
}
