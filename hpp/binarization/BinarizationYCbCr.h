/***************************************************************
 * Name:      BinarizationYCbCr.h
 * Purpose:   Code for Binarization Class Extension Header
 * Author:    Bogdan Alecu
 * Created:   2011-09-15
 * Copyright: Bogdan Alecu
 * License:   GNU GPL
 **************************************************************/

#ifndef OPENVPABINARIZATIONYCBCR_H
#define OPENVPABINARIZATIONYCBCR_H

#include "Binarization.h"

using namespace std;

namespace openvpa {

    class BinarizationYCbCr: virtual public Binarization {

        public:
            BinarizationYCbCr();
            ~BinarizationYCbCr();
            bool reset();
            Mat& get(Mat& frm);
            int mode() {return BINARIZATION_MODE_YCBCR;};
            int allowedTypes() {return BINARIZE_PER_PLANE_LEVEL | BINARIZE_GENERIC_LEVEL;};
    };
}

#endif
