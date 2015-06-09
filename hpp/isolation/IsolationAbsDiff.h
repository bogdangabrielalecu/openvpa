/***************************************************************
 * Name:      IsolationAbsDiff.h
 * Purpose:   Code for Isolation Class Extension Header
 * Author:    Bogdan Alecu
 * Created:   2011-09-15
 * Copyright: Bogdan Alecu
 * License:   GNU GPL
 **************************************************************/

#ifndef OPENVPAISOLATIONABSDIFF_H
#define OPENVPAISOLATIONABSDIFF_H

#include "Isolation.h"

using namespace std;

namespace openvpa {

    class IsolationAbsDiff: virtual public Isolation {

        public:
            IsolationAbsDiff();
            ~IsolationAbsDiff();
            bool reset();
            Mat& get(Mat& bg, Mat& frm);
            int mode() {return ISOLATION_MODE_ABSDIFF;};
    };
}

#endif
