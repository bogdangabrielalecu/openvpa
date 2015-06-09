/***************************************************************
 * Name:      ModelBGMedianPseudo.cpp
 * Purpose:   Code for ModelBG Class Extension
 * Author:    Bogdan Alecu
 * Created:   2011-09-15
 * Copyright: Bogdan Alecu
 * License:   GNU GPL
 **************************************************************/

#include "modeling/ModelBGMedianPseudo.h"
#include "Data.h"

namespace openvpa {

    ModelBGMedianPseudo::ModelBGMedianPseudo() : ModelBG::ModelBG() {
        reset();
    }

    ModelBGMedianPseudo::~ModelBGMedianPseudo() {
        //
    }

    bool ModelBGMedianPseudo::reset() {
        low_bg = Mat();
        high_bg = Mat();
        return ModelBG::reset();
    }

    Mat& ModelBGMedianPseudo::get() {
        updated = false;
        return bg;
    }

    bool ModelBGMedianPseudo::update(Mat& frame, Mat& mask) {
        if (bg.empty()) {
            initFrames(frame);
            updated = true;
            return true;
        }
        for (int y = 0; y < frame.rows; y ++) {
            for (int x = 0; x < frame.cols; x ++) {
                if (mask.at<Vec1b>(x, y)[0] <= 0) {
                    continue;
                }
                pixelUpdate(frame, x, y);
            }
        }
        updated = true;
        return true;
    }

    bool ModelBGMedianPseudo::update(Mat& frame) {
        if (bg.empty()) {
            initFrames(frame);
            updated = true;
            return true;
        }
        for (int y = 0; y < frame.rows; y ++) {
            for (int x = 0; x < frame.cols; x ++) {
                pixelUpdate(frame, x, y);
            }
        }
        updated = true;
        return true;
    }

    void ModelBGMedianPseudo::pixelUpdate(Mat& frame, int x, int y) {
        for (int c = 0; c < 3; c ++) {
            if ((frame.at<Vec3b>(x, y)[c] <= low_bg.at<Vec3b>(x, y)[c])
                || (frame.at<Vec3b>(x, y)[c] >= high_bg.at<Vec3b>(x, y)[c])) {
                continue;
            }
            if (frame.at<Vec3b>(x, y)[c] <= bg.at<Vec3b>(x, y)[c]) {
                high_bg.at<Vec3b>(x, y)[c] = bg.at<Vec3b>(x, y)[c];
                bg.at<Vec3b>(x, y)[c] = frame.at<Vec3b>(x, y)[c];
            } else {
                low_bg.at<Vec3b>(x, y)[c] = bg.at<Vec3b>(x, y)[c];
                bg.at<Vec3b>(x, y)[c] = frame.at<Vec3b>(x, y)[c];
            }
        }
    }

    void ModelBGMedianPseudo::initFrames(Mat& frame) {
        frame.copyTo(bg);
        frame.copyTo(low_bg);
        frame.copyTo(high_bg);
    }
}
