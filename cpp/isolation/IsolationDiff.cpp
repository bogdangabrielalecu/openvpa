/***************************************************************
 * Name:      IsolationDiff.cpp
 * Purpose:   Code for Isolation Class Extension
 * Author:    Bogdan Alecu
 * Created:   2011-09-15
 * Copyright: Bogdan Alecu
 * License:   GNU GPL
 **************************************************************/

#include "isolation/IsolationDiff.h"
#include "Data.h"

namespace openvpa {

    IsolationDiff::IsolationDiff() : Isolation::Isolation() {
        reset();
    }

    IsolationDiff::~IsolationDiff() {
        //
    }

    bool IsolationDiff::reset() {
        return true;
    }

    Mat& IsolationDiff::get(Mat& bg, Mat& frm) {
        subtract(frm, bg, isolated_frame);
        return isolated_frame;
    }
}
