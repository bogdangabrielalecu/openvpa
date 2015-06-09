/***************************************************************
 * Name:      ModelFGGaussianMixture.cpp
 * Purpose:   Code for ModelFG Class Extension
 * Author:    Bogdan Alecu
 * Created:   2011-09-16
 * Copyright: Bogdan Alecu
 * License:   GNU GPL
 **************************************************************/

#include "modeling/ModelFGGaussianMixture.h"
#include "Data.h"
#include <math.h>

namespace openvpa {

    const float ModelFGGaussianMixture::DEFAULT_VARIANCE = 0.1;
    const int ModelFGGaussianMixture::DEFAULT_VICINITY_RADIUS = 1;
    const float ModelFGGaussianMixture::PI = 3.14159;

    ModelFGGaussianMixture::ModelFGGaussianMixture() : ModelFG::ModelFG() {
        reset();
    }

    ModelFGGaussianMixture::~ModelFGGaussianMixture() {
        //
    }

    bool ModelFGGaussianMixture::reset() {
        map = IntGaussMap();
        return ModelFG::reset();
    }

    Mat& ModelFGGaussianMixture::get() {
        if (!updated) {
            return fg;
        }
        extractFromGaussianSum();
        return fg;
    }

    void ModelFGGaussianMixture::generateGaussians() {
        map = IntGaussMap();
        double gaussian_variance = gaussianVariance();
        for (int i = -255; i <= 255; i ++) {
            map[i] = gaussian(i, gaussian_variance);
        }
    }

    Mat& ModelFGGaussianMixture::extractFromGaussianSum() {
        if (fg.empty()) {
            fg = Mat::zeros(current_frame.size(), CV_32FC3);
        }
        double gaussian_variance = gaussianVariance();
        int vicinity_radius = gaussVicinityRadius();
        cout << "extracting gaussian mix, gamma = " << gaussian_variance << ", k = " << vicinity_radius << endl;
        for (int y = 0; y < current_frame.rows; y ++) {
            for (int x = 0; x < current_frame.cols; x ++) {
                for (int c = 0; c < 3; c ++) {
                    unsigned int cnt = 0;
                    double gauss_sum = 0;
                    for (int i = - vicinity_radius; i <= vicinity_radius; i ++) {
                        if ((x + i < 0)
                            || (x + i >= current_frame.cols)) {
                            continue;
                        }
                        for (int j = - vicinity_radius; j <= vicinity_radius; j ++) {
                            if (((0 == i) && (0 == j))
                                || (y + j < 0)
                                || (y + j >= current_frame.rows)) {
                                continue;
                            }
                            double diff = current_frame.at<Vec3b>(y, x)[c] - current_frame.at<Vec3b>(y + j, x + i)[c];
                            gauss_sum += gaussianMap(diff);
//                          gauss_sum += gaussian(diff, gaussian_variance);
//                          gauss_sum += gaussianMapWithGrid(diff, gaussian_variance);
                            cnt ++;
                        }
                    }
                    if (0 == cnt) {
                        continue;
                    }
                    fg.at<Vec3f>(y, x)[c] = gauss_sum / cnt;
                }
            }
        }
        updated = false;
        return fg;
    }

    bool ModelFGGaussianMixture::update(Mat& frame, Mat& mask) {
        return update(frame);// mask means nothing for this modeling method
    }

    bool ModelFGGaussianMixture::update(Mat& frame) {
        updated = true;
        return true;
    }

    double ModelFGGaussianMixture::gaussian(int x, double var) {
        double var_s = 2 * var * var;
        return exp(- x * x / var_s) / sqrt(PI * var_s);
    }

    double ModelFGGaussianMixture::gaussianMap(int x) {
        if (map.size() < 511) {
            cout << "generating gaussians \n";
            generateGaussians();
        }
        return map[x];
    }

    double ModelFGGaussianMixture::gaussianVariance() {
        if (!double_params["gauss_var"]) {
            return DEFAULT_VARIANCE;
        }
        return double_params["gauss_var"];
    }

    int ModelFGGaussianMixture::gaussVicinityRadius() {
        if (!double_params["gauss_k"]) {
            return DEFAULT_VICINITY_RADIUS;
        }
        return double_params["gauss_k"];
    }
}
