/***************************************************************
 * Name:      ModelBGMeanSliding.cpp
 * Purpose:   Code for ModelBG Class Extension
 * Author:    Bogdan Alecu
 * Created:   2011-09-15
 * Copyright: Bogdan Alecu
 * License:   GNU GPL
 **************************************************************/

#include "modeling/ModelBGMeanSliding.h"
#include "Data.h"

namespace openvpa {

    ModelBGMeanSliding::ModelBGMeanSliding() : ModelBG::ModelBG() {
        reset();
    }

    ModelBGMeanSliding::~ModelBGMeanSliding() {
        //
    }

    bool ModelBGMeanSliding::reset() {
        alpha(0.05);
        return ModelBG::reset();
    }

    Mat& ModelBGMeanSliding::get() {
        updated = false; // looks a bit weird but it's meant to be consistent with brother classes
        return bg;
    }

    bool ModelBGMeanSliding::update(Mat& frame, Mat& mask) {
        Mat mean_updater;
        Data::multiplexImgs(frame, bg, mask, mean_updater);
        return update(mean_updater);
    }

    bool ModelBGMeanSliding::update(Mat& frame) {
        updated = true;
        if (isEmpty()) {
            frame.copyTo(bg);
            return true;
        }
        double alpha_ = alpha();
        addWeighted(
            bg,
            (1 - alpha_),
            frame,
            alpha_,
            0,
            bg);
        return true;
    }

    double ModelBGMeanSliding::alpha() {
        if (!double_params["alpha"]) {
            return 0.05;
        }
        return double_params["alpha"];
    }

    void ModelBGMeanSliding::alpha(double v) {
        double_params["alpha"] = v;
    }
}
