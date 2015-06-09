/***************************************************************
 * Name:      BinarizationAll.h
 * Purpose:   Code for Binarization Class Extension Header
 * Author:    Bogdan Alecu
 * Created:   2011-09-15
 * Copyright: Bogdan Alecu
 * License:   GNU GPL
 **************************************************************/

#ifndef OPENVPABINARIZATIONALL_H
#define OPENVPABINARIZATIONALL_H

#include "Binarization.h"

using namespace std;

namespace openvpa {

    class BinarizationAll: virtual public Binarization {

        public:
            BinarizationAll();
            ~BinarizationAll();
            bool reset();
            Mat& get(Mat& frm);
            int mode() {return BINARIZATION_MODE_ALL;};
            int allowedTypes() {return BINARIZE_PER_PLANE_LEVEL | BINARIZE_GENERIC_LEVEL;};
    };
}

#endif
