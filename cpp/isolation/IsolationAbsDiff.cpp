/***************************************************************
 * Name:      IsolationAbsDiff.cpp
 * Purpose:   Code for Isolation Class Extension
 * Author:    Bogdan Alecu
 * Created:   2011-09-15
 * Copyright: Bogdan Alecu
 * License:   GNU GPL
 **************************************************************/

#include "isolation/IsolationAbsDiff.h"
#include "Data.h"

namespace openvpa {

    IsolationAbsDiff::IsolationAbsDiff() : Isolation::Isolation() {
        reset();
    }

    IsolationAbsDiff::~IsolationAbsDiff() {
        //
    }

    bool IsolationAbsDiff::reset() {
        return true;
    }

    Mat& IsolationAbsDiff::get(Mat& bg, Mat& frm) {
        absdiff(frm, bg, isolated_frame);
        return isolated_frame;
    }
}
