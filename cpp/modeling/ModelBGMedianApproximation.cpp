/***************************************************************
 * Name:      ModelBGMedianApproximation.cpp
 * Purpose:   Code for ModelBG Class Extension
 * Author:    Bogdan Alecu
 * Created:   2011-09-15
 * Copyright: Bogdan Alecu
 * License:   GNU GPL
 **************************************************************/

#include "modeling/ModelBGMedianApproximation.h"
#include "Data.h"

namespace openvpa {

    ModelBGMedianApproximation::ModelBGMedianApproximation() : ModelBG::ModelBG() {
        reset();
    }

    ModelBGMedianApproximation::~ModelBGMedianApproximation() {
        //
    }

    bool ModelBGMedianApproximation::reset() {
        ones_mask = Mat();
        return ModelBG::reset();
    }

    Mat& ModelBGMedianApproximation::get() {
        updated = false;
        return bg;
    }

    bool ModelBGMedianApproximation::update(Mat& frame, Mat& mask) {
        if (bg.empty()) {
            frame.copyTo(bg);
            return true;
        }
        Mat_<Vec3b>::iterator it = bg.begin<Vec3b>();
        Mat_<Vec3b>::iterator it_orig = frame.begin<Vec3b>();
        Mat_<Vec1b>::iterator it_mask = mask.begin<Vec1b>();
        Mat_<Vec3b>::iterator it_end = bg.end<Vec3b>();
        Mat_<Vec3b>::iterator it_orig_end = frame.end<Vec3b>();
        Mat_<Vec1b>::iterator it_mask_end = mask.end<Vec1b>();
        bool changed = false;
        for (;
            (it != it_end) && (it_orig != it_orig_end);
            it ++, it_orig ++) {
            if ((*it_mask)[0] <= 0) {
                continue;
            }
            for (int c = 0; c < 3; c ++) {
                if ((*it)[c] < (*it_orig)[c]) {
                    (*it)[c] ++;
                    changed = true;
                }
                if ((*it)[c] > (*it_orig)[c]) {
                    (*it)[c] --;
                    changed = true;
                }
            }
        }
        return changed;
    }

    bool ModelBGMedianApproximation::update(Mat& frame) {
        if (ones_mask.empty()) {
            ones_mask = Mat::ones(frame.size(), CV_8UC1);
        }
        return update(frame, ones_mask);
    }
}
