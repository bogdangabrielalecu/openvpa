/***************************************************************
 * Name:      ModelFGNonParamDist.cpp
 * Purpose:   Code for ModelFG Class Extension
 * Author:    Bogdan Alecu
 * Created:   2011-09-17
 * Copyright: Bogdan Alecu
 * License:   GNU GPL
 **************************************************************/

#include "modeling/ModelFGNonParamDist.h"
#include "Data.h"
#include <math.h>

namespace openvpa {

    const float ModelFGNonParamDist::DEFAULT_VARIANCE = 0.1;
    const int ModelFGNonParamDist::DEFAULT_BUFFER_LENGTH = 3;
    const float ModelFGNonParamDist::DEFAULT_GRID_STEP = 0.25;
    const float ModelFGNonParamDist::PI = 3.14159;

    ModelFGNonParamDist::ModelFGNonParamDist() : ModelFG::ModelFG() {
        reset();
    }

    ModelFGNonParamDist::~ModelFGNonParamDist() {
        //
    }

    bool ModelFGNonParamDist::reset() {
        index = -1;
        full_buffer = false;
        mv = MatVector();
        map = VarGaussMap();
        return ModelFG::reset();
    }

    Mat& ModelFGNonParamDist::get() {
        if (!updated) {
            return fg;
        }
        extractFromGaussianSum();
        return fg;
    }

    Mat& ModelFGNonParamDist::extractFromGaussianSum() {
        if (!full_buffer) {
            return fg;
        }
        Mat tmp = Mat::zeros(current_frame.size(), CV_32FC3);
        unsigned int buf_len = bufferLength();
        for (unsigned int i = 0; i < buf_len; i ++) {
            for (int y = 0; y < current_frame.rows; y ++) {
                for (int x = 0; x < current_frame.cols; x ++) {
                    for (int c = 0; c < 3; c ++) {
                        double diff = current_frame.at<Vec3b>(y, x)[c] - mv[indexOf(i)].at<Vec3b>(y, x)[c];
                        double gauss = gaussian(diff, gaussianVariance());
                        tmp.at<Vec3f>(y, x)[c] += gauss;
                    }
                }
            }
        }
        fg.create(current_frame.size(), CV_8UC3);
        fg = tmp / buf_len;
        updated = false;
        return fg;
    }

    bool ModelFGNonParamDist::update(Mat& frame, Mat& mask) {
        return update(frame);// mask means nothing for this modeling method
    }

    bool ModelFGNonParamDist::update(Mat& frame) {
        nextIndex();
        Mat tmp; // dealing with references
        frame.copyTo(tmp);
        if (mv.size() < bufferLength()) {
            mv.push_back(tmp);
        } else {
            tmp.copyTo(mv[indexOf()]);
        }
        updated = true;
        return true;
    }

    double ModelFGNonParamDist::gaussian(double x, double var) {
        double var_s = var * var;
        return exp(- x * x / (2 * var_s)) / sqrt(2 * PI * var_s);
    }

    double ModelFGNonParamDist::gaussianMap(double x, double var) {
        if (!map[var].empty()) {
            if (map[var][x]) {
                return map[var][x];
            }
            map[var][x] = gaussian(x, var);
            return map[var][x];
        }
        map[var] = GaussMap();
        map[var][x] = gaussian(x, var);
        return map[var][x];
    }

    double ModelFGNonParamDist::gaussianMapWithGrid(double x, double var) {
        double x_grid = nearestGridValue(x, "x");
        double var_grid = nearestGridValue(var, "var");
        return gaussianMap(x_grid, var_grid);
    }

    double ModelFGNonParamDist::nearestGridValue(double x, string s) {
        if (1 == gridStep(s)) {
            return (float) ((int) x);
        }
        return (float) (((int) (x * gridStep(s))) / gridStep(s));
    }

    unsigned int ModelFGNonParamDist::indexOf(unsigned int v) {
        return (index - v) % bufferLength();
    }

    void ModelFGNonParamDist::nextIndex() {
        index = ++index % bufferLength();
        if (index >= bufferLength() - 1) {
            full_buffer = true;
        }
    }

    double ModelFGNonParamDist::gridStep(string s) {
        string sv = "grid_step_" + s;
        if (!double_params[sv]) {
            return DEFAULT_GRID_STEP;
        }
        return double_params[sv];
    }

    unsigned int ModelFGNonParamDist::bufferLength() {
        if (!double_params["gauss_k"]) {
            return DEFAULT_BUFFER_LENGTH;
        }
        return double_params["gauss_k"];
    }

    double ModelFGNonParamDist::gaussianVariance() {
        if (!double_params["gauss_var"]) {
            return DEFAULT_VARIANCE;
        }
        return double_params["gauss_var"];
    }
}
