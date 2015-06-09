/***************************************************************
 * Name:      Features.cpp
 * Purpose:   Code for Feature Extraction, Tracking and Classification Base Class
 * Author:    Bogdan Alecu
 * Created:   2011-09-16
 * Copyright: Bogdan Alecu
 * License:   GNU GPL
 **************************************************************/

#include "Features.h"

//list of the Features extended implementations
#include "features/FeaturesBasic.h"

namespace openvpa {

    Features::Features() {
        resetObjects();
        double_params.clear();
        mat_params.clear();
    }

    auto_ptr<Features> Features::factory(int mode) {
        if (FEATURE_MODE_NONE == mode) {
            auto_ptr<Features> p(new FeaturesNone());
            return p;
        }
        if (FEATURE_MODE_BASIC == mode) {
            auto_ptr<Features> p(new FeaturesBasic());
            return p;
        }
        auto_ptr<Features> p(new FeaturesBasic());
        return p;
    }

    void Features::paramDouble(string k, double v) {
        double_params[k] = v;
    }

    void Features::paramMat(string k, Mat v) {
        mat_params[k] = v;
    }

    ObjAttr Features::newObjAttr() {
        ObjAttr attr;
        obj_counter ++;
        attr.id = obj_counter;
        attr.classid = 0;
        attr.seg_col = colorVec3b(obj_counter);
        attr.attrs = Attrs();
        return attr;
    }

    void Features::resetObjCounter() {
        obj_counter = 0;
    }

    void Features::resetObjects() {
        obj = FrameObjects();
        resetObjCounter();
    }

    Vec3b Features::colorVec3b(unsigned int id) {
        int i = (int)(id / 3);
        int step = 23;
        int step_offset = 13;
        Vec3b c = Vec3b(
            step_offset * i + 1 + step * (id % 3),
            step_offset * i + 1 + step * ((id + 1) % 3),
            step_offset * i + 1 + step * ((id + 2) % 3)
        );
        return c;
    }

    FeaturesNone::FeaturesNone() : Features::Features() {
        reset();
    }

    FeaturesNone::~FeaturesNone() {
        //
    }

    bool FeaturesNone::reset() {
        // dummy function for now
        out_frm = Mat();
        return true;
    }

    bool FeaturesNone::extract(Mat& frm, Mat& crt_frm) {
        frm.copyTo(out_frm);
        return true;
    }

    Mat FeaturesNone::getObjectsFrame() {
        return out_frm;
    }

    FrameObjects& FeaturesNone::getFrameObjects() {
        return obj;
    }
}
