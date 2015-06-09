/***************************************************************
 * Name:      BinarizationAtLeastOne.h
 * Purpose:   Code for Binarization Class Extension Header
 * Author:    Bogdan Alecu
 * Created:   2011-09-15
 * Copyright: Bogdan Alecu
 * License:   GNU GPL
 **************************************************************/

#ifndef OPENVPABINARIZATIONATLEASTONE_H
#define OPENVPABINARIZATIONATLEASTONE_H

#include "Binarization.h"

using namespace std;

namespace openvpa {

    class BinarizationAtLeastOne: virtual public Binarization {

        public:
            BinarizationAtLeastOne();
            ~BinarizationAtLeastOne();
            bool reset();
            Mat& get(Mat& frm);
            int mode() {return BINARIZATION_MODE_AT_LEAST_ONE;};
            int allowedTypes() {return BINARIZE_PER_PLANE_LEVEL | BINARIZE_GENERIC_LEVEL;};
    };
}

#endif
