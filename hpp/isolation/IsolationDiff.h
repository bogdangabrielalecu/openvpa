/***************************************************************
 * Name:      IsolationDiff.h
 * Purpose:   Code for Isolation Class Extension Header
 * Author:    Bogdan Alecu
 * Created:   2011-09-15
 * Copyright: Bogdan Alecu
 * License:   GNU GPL
 **************************************************************/

#ifndef OPENVPAISOLATIONDIFF_H
#define OPENVPAISOLATIONDIFF_H

#include "Isolation.h"

using namespace std;

namespace openvpa {

    class IsolationDiff: virtual public Isolation {

        public:
            IsolationDiff();
            ~IsolationDiff();
            bool reset();
            Mat& get(Mat& bg, Mat& frm);
            int mode() {return ISOLATION_MODE_DIFF;};
    };
}

#endif
