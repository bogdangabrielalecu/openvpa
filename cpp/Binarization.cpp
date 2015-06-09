/***************************************************************
 * Name:      Binarization.cpp
 * Purpose:   Code for Binarization Base Class
 * Author:    Bogdan Alecu
 * Created:   2011-09-15
 * Copyright: Bogdan Alecu
 * License:   GNU GPL
 **************************************************************/

#include "Binarization.h"

//list of the Binarization extended implementations
#include "binarization/BinarizationLuminance.h"
#include "binarization/BinarizationChrominance.h"
#include "binarization/BinarizationAtLeastOne.h"
#include "binarization/BinarizationAll.h"
#include "binarization/BinarizationMean.h"
#include "binarization/BinarizationYCbCr.h"

namespace openvpa {

    Binarization::Binarization() {
        inner_level = 0;
        inner_levels = Vec3d(0, 0, 0);
        inner_type = BINARIZE_GENERIC_LEVEL;
        inner_way = BINARIZATION_WAY_DIRECT;
    }

    auto_ptr<Binarization> Binarization::factory(int mode) {
        if (BINARIZATION_MODE_NONE == mode) {
            auto_ptr<Binarization> p(new BinarizationNone());
            return p;
        }
        if (BINARIZATION_MODE_LUMINANCE == mode) {
            auto_ptr<Binarization> p(new BinarizationLuminance());
            return p;
        }
        if (BINARIZATION_MODE_CHROMINANCE == mode) {
            auto_ptr<Binarization> p(new BinarizationChrominance());
            return p;
        }
        if (BINARIZATION_MODE_AT_LEAST_ONE == mode) {
            auto_ptr<Binarization> p(new BinarizationAtLeastOne());
            return p;
        }
        if (BINARIZATION_MODE_ALL == mode) {
            auto_ptr<Binarization> p(new BinarizationAll());
            return p;
        }
        if (BINARIZATION_MODE_MEAN == mode) {
            auto_ptr<Binarization> p(new BinarizationMean());
            return p;
        }
        if (BINARIZATION_MODE_YCBCR == mode) {
            auto_ptr<Binarization> p(new BinarizationYCbCr());
            return p;
        }
        auto_ptr<Binarization> p(new openvpa::BinarizationLuminance());
        return p;
    }

    void Binarization::type(int val) {
        inner_type = val;
    }

    int Binarization::type() {
        return inner_type;
    }

    void Binarization::way(int val) {
        inner_way = val;
    }

    int Binarization::way() {
        return inner_way;
    }

    void Binarization::level(double val) {
        inner_level = val;
    }

    double Binarization::level() {
        return inner_level;
    }

    void Binarization::levels(Vec3d vec) {
        inner_levels = vec;
    }

    void Binarization::levels(double v1, double v2, double v3) {
        inner_levels = Vec3d(v1, v2, v3);
    }

    Vec3d Binarization::levels() {
        return inner_levels;
    }

    BinarizationNone::BinarizationNone() : Binarization::Binarization() {
        reset();
    }

    BinarizationNone::~BinarizationNone() {
        //
    }

    bool BinarizationNone::reset() {
        // dummy function for now
        return true;
    }

    Mat& BinarizationNone::get(Mat& frm) {
        frm.copyTo(binarized_frame);
        return binarized_frame;
    }
}
