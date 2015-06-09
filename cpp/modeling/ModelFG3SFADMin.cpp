/***************************************************************
 * Name:      ModelFG3SFADMin.cpp
 * Purpose:   Code for ModelFG Class Extension
 * Author:    Bogdan Alecu
 * Created:   2011-09-15
 * Copyright: Bogdan Alecu
 * License:   GNU GPL
 **************************************************************/

#include "modeling/ModelFG3SFADMin.h"
#include "Data.h"

namespace openvpa {

    ModelFG3SFADMin::ModelFG3SFADMin() : ModelFG::ModelFG() {
        reset();
    }

    ModelFG3SFADMin::~ModelFG3SFADMin() {
        //
    }

    bool ModelFG3SFADMin::reset() {
        index = -1;
        full_buffer = false;
        mv = MatVector();
        return ModelFG::reset();
    }

    Mat& ModelFG3SFADMin::get() {
        if (!full_buffer) {
            return fg;
        }
        if (!updated) {
            return fg;
        }
        updated = false;
        Mat d1, d2;
        absdiff(mv[indexOf(1)], mv[indexOf(0)], d1);
        absdiff(mv[indexOf(0)], current_frame, d2);
        min(d1, d2, fg);
        return fg;
    }

    bool ModelFG3SFADMin::update(Mat& frame, Mat& mask) {
        return update(frame);// mask means nothing for this modeling method
    }

    bool ModelFG3SFADMin::update(Mat& frame) {
        nextIndex();
        Mat tmp; // dealing with references
        frame.copyTo(tmp);
        if (mv.size() < 2) {
            mv.push_back(tmp);
        } else {
            tmp.copyTo(mv[indexOf()]);
        }
        updated = true;
        return true;
    }

    unsigned int ModelFG3SFADMin::indexOf(unsigned int v) {
        return (index - v) % 2;
    }

    void ModelFG3SFADMin::nextIndex() {
        index = ++index % 2;
        if (index >= 1) {
            full_buffer = true;
        }
    }
}
