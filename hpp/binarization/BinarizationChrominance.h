/***************************************************************
 * Name:      BinarizationChrominance.h
 * Purpose:   Code for Binarization Class Extension Header
 * Author:    Bogdan Alecu
 * Created:   2011-09-16
 * Copyright: Bogdan Alecu
 * License:   GNU GPL
 **************************************************************/

#ifndef OPENVPABINARIZATIONCHROMINANCE_H
#define OPENVPABINARIZATIONCHROMINANCE_H

#include "Binarization.h"

using namespace std;

namespace openvpa {

    class BinarizationChrominance: virtual public Binarization {

        public:
            BinarizationChrominance();
            ~BinarizationChrominance();
            bool reset();
            Mat& get(Mat& frm);
            int mode() {return BINARIZATION_MODE_CHROMINANCE;};
            int allowedTypes() {return BINARIZE_GENERIC_LEVEL;};
    };
}

#endif
