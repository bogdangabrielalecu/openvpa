/***************************************************************
 * Name:      BinarizationLuminance.h
 * Purpose:   Code for Binarization Class Extension Header
 * Author:    Bogdan Alecu
 * Created:   2011-09-16
 * Copyright: Bogdan Alecu
 * License:   GNU GPL
 **************************************************************/

#ifndef OPENVPABINARIZATIONLUMINANCE_H
#define OPENVPABINARIZATIONLUMINANCE_H

#include "Binarization.h"

using namespace std;

namespace openvpa {

    class BinarizationLuminance: virtual public Binarization {

        public:
            BinarizationLuminance();
            ~BinarizationLuminance();
            bool reset();
            Mat& get(Mat& frm);
            int mode() {return BINARIZATION_MODE_LUMINANCE;};
            int allowedTypes() {return BINARIZE_GENERIC_LEVEL;};
    };
}

#endif
