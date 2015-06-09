/***************************************************************
 * Name:      BinarizationMean.h
 * Purpose:   Code for Binarization Class Extension Header
 * Author:    Bogdan Alecu
 * Created:   2011-09-16
 * Copyright: Bogdan Alecu
 * License:   GNU GPL
 **************************************************************/

#ifndef OPENVPABINARIZATIONMEAN_H
#define OPENVPABINARIZATIONMEAN_H

#include "Binarization.h"

using namespace std;

namespace openvpa {

    class BinarizationMean: virtual public Binarization {

        public:
            BinarizationMean();
            ~BinarizationMean();
            bool reset();
            Mat& get(Mat& frm);
            int mode() {return BINARIZATION_MODE_MEAN;};
            int allowedTypes() {return BINARIZE_GENERIC_LEVEL;};
    };
}

#endif
