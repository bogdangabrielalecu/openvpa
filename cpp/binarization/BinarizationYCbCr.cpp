/***************************************************************
 * Name:      BinarizationYCbCr.cpp
 * Purpose:   Code for Binarization Class Extension
 * Author:    Bogdan Alecu
 * Created:   2011-09-15
 * Copyright: Bogdan Alecu
 * License:   GNU GPL
 **************************************************************/

#include "binarization/BinarizationYCbCr.h"
#include "Detector.h"

namespace openvpa {

    BinarizationYCbCr::BinarizationYCbCr() : Binarization::Binarization() {
        reset();
    }

    BinarizationYCbCr::~BinarizationYCbCr() {
        //
    }

    bool BinarizationYCbCr::reset() {
        return true;
    }

    Mat& BinarizationYCbCr::get(Mat& frm) {
        Mat ycbcr_frame;
        ycbcr_frame = Detector::rgb2ycbcr(frm);
        vector<Mat> planes;
        split(ycbcr_frame, planes);
        if (BINARIZE_PER_PLANE_LEVEL == type()) {
            if (BINARIZATION_WAY_DIRECT == way()) {
                binarized_frame = ((planes[0] > inner_levels[0])
                    & (planes[1] > inner_levels[1])
                    & (planes[2] > inner_levels[2])
                );
                return binarized_frame;
            }
            binarized_frame = ((planes[0] <= inner_levels[0])
                & (planes[1] <= inner_levels[1])
                & (planes[2] <= inner_levels[2])
            );
            return binarized_frame;
        }
        if (BINARIZE_GENERIC_LEVEL == type()) {
            if (BINARIZATION_WAY_DIRECT == way()) {
                binarized_frame = ((planes[0] > inner_level)
                    & (planes[1] > inner_level)
                    & (planes[2] > inner_level)
                );
                return binarized_frame;
            }
            binarized_frame = ((planes[0] <= inner_level)
                & (planes[1] <= inner_level)
                & (planes[2] <= inner_level)
            );
            return binarized_frame;
        }
        return binarized_frame;
    }
}
