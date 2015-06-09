/***************************************************************
 * Name:      Enhancement.cpp
 * Purpose:   Code for Enhancement Base Class
 * Author:    Bogdan Alecu
 * Created:   2011-09-15
 * Copyright: Bogdan Alecu
 * License:   GNU GPL
 **************************************************************/

#include "Enhancement.h"
#include <iostream>

//list of the Enhancement extended implementations
#include "enhancement/EnhancementMorphology.h"

namespace openvpa {

    Enhancement::Enhancement() {
        //
    }

    auto_ptr<Enhancement> Enhancement::factory(int mode) {
        if (ENHANCEMENT_MODE_NONE == mode) {
            auto_ptr<Enhancement> p(new EnhancementNone());
            return p;
        }
        if (ENHANCEMENT_MODE_MORPHOLOGY == mode) {
            auto_ptr<Enhancement> p(new EnhancementMorphology());
            return p;
        }
        auto_ptr<Enhancement> p(new openvpa::EnhancementNone());
        return p;
    }

    void Enhancement::paramDouble(string k, double v) {
        double_params[k] = v;
    }

    void Enhancement::paramMat(string k, Mat v) {
        mat_params[k] = v;
    }


    EnhancementNone::EnhancementNone() : Enhancement::Enhancement() {
        reset();
    }

    EnhancementNone::~EnhancementNone() {
        //
    }

    bool EnhancementNone::reset() {
        // dummy function for now
        return true;
    }

    Mat& EnhancementNone::get(Mat& frm) {
        frm.copyTo(enhanced_frame);
        return enhanced_frame;
    }
}
