/***************************************************************
 * Name:      ModelBGMean.cpp
 * Purpose:   Code for ModelBG Class Extension
 * Author:    Bogdan Alecu
 * Created:   2011-09-15
 * Copyright: Bogdan Alecu
 * License:   GNU GPL
 **************************************************************/

#include "modeling/ModelBGMean.h"
#include "Data.h"

namespace openvpa {

    ModelBGMean::ModelBGMean() : ModelBG::ModelBG() {
        reset();
    }

    ModelBGMean::~ModelBGMean() {
        //
    }

    bool ModelBGMean::reset() {
        processed_frames = 0;
        previous_bg = Mat();
        return ModelBG::reset();
    }

    Mat& ModelBGMean::get() {
        updated = false; // looks a bit weird but it's meant to be consistent with brother classes
        return bg;
    }

    bool ModelBGMean::update(Mat& frame, Mat& mask) {
        Mat mean_updater;
        Data::multiplexImgs(frame, bg, mask, mean_updater);
        return update(mean_updater);
    }

    bool ModelBGMean::update(Mat& frame) {
        updated = true;
        processed_frames ++;
        if (isEmpty()) {
            frame.copyTo(bg);
            return true;
        }
        bg.copyTo(previous_bg);
        addWeighted(
            bg,
            (double) (processed_frames - 1) / processed_frames,
            frame,
            (double) 1 / processed_frames,
            0,
            bg);
        return true;
    }
}
