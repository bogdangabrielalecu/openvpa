/***************************************************************
 * Name:      Isolation.cpp
 * Purpose:   Code for Isolation Base Class
 * Author:    Bogdan Alecu
 * Created:   2011-09-15
 * Copyright: Bogdan Alecu
 * License:   GNU GPL
 **************************************************************/

#include "Isolation.h"
#include <iostream>

//list of the Isolation extended implementations
#include "isolation/IsolationDiff.h"
#include "isolation/IsolationAbsDiff.h"

namespace openvpa {

    Isolation::Isolation() {
        //
    }

    auto_ptr<Isolation> Isolation::factory(int mode) {
        if (ISOLATION_MODE_NONE == mode) {
            auto_ptr<Isolation> p(new IsolationNone());
            return p;
        }
        if (ISOLATION_MODE_DIFF == mode) {
            auto_ptr<Isolation> p(new IsolationDiff());
            return p;
        }
        if (ISOLATION_MODE_DIFF == mode) {
            auto_ptr<Isolation> p(new IsolationAbsDiff());
            return p;
        }
        auto_ptr<Isolation> p(new openvpa::IsolationDiff());
        return p;
    }

    IsolationNone::IsolationNone() : Isolation::Isolation() {
        reset();
    }

    IsolationNone::~IsolationNone() {
        //
    }

    bool IsolationNone::reset() {
        // dummy function for now
        return true;
    }

    Mat& IsolationNone::get(Mat& bg, Mat& frm) {
        frm.copyTo(isolated_frame);
        return isolated_frame;
    }
}
