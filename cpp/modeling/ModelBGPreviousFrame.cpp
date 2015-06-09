/***************************************************************
 * Name:      ModelBGPreviousFrame.cpp
 * Purpose:   Code for ModelBG Class Extension
 * Author:    Bogdan Alecu
 * Created:   2011-09-15
 * Copyright: Bogdan Alecu
 * License:   GNU GPL
 **************************************************************/

#include "modeling/ModelBGPreviousFrame.h"
#include "Data.h"

namespace openvpa {

    ModelBGPreviousFrame::ModelBGPreviousFrame() : ModelBG::ModelBG() {
        reset();
    }

    ModelBGPreviousFrame::~ModelBGPreviousFrame() {
        //
    }

    bool ModelBGPreviousFrame::reset() {
        pre_bg = Mat();
        return ModelBG::reset();
    }

    Mat& ModelBGPreviousFrame::get() {
        updated = false;
        return bg;
    }

    bool ModelBGPreviousFrame::update(Mat& frame, Mat& mask) {
        updated = true;
        Mat tmp;
        frame.copyTo(tmp);
        if (bg.empty()) {
            bg = tmp;
            return true;
        }
        tmp.copyTo(bg, mask);
        return true;
    }

    bool ModelBGPreviousFrame::update(Mat& frame) {
        updated = true;
        Mat tmp;
        frame.copyTo(tmp);
        bg = tmp;
        return true;
    }
}
