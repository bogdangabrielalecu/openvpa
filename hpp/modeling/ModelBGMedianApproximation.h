/***************************************************************
 * Name:      ModelBGMedianApproximation.h
 * Purpose:   Code for ModelBG Class Extension Header
 * Author:    Bogdan Alecu
 * Created:   2011-09-15
 * Copyright: Bogdan Alecu
 * License:   GNU GPL
 **************************************************************/

#ifndef OPENVPAMODELBGMEDIANAPPROXIMATION_H
#define OPENVPAMODELBGMEDIANAPPROXIMATION_H

#include "ModelBG.h"

using namespace std;

namespace openvpa {

    typedef Vec<uchar, 1> Vec1b;

    class ModelBGMedianApproximation: virtual public ModelBG {

        public:
            ModelBGMedianApproximation();
            ~ModelBGMedianApproximation();
            bool reset();
            Mat& get();
            bool update(Mat& frame);
            bool update(Mat& frame, Mat& mask);
            int mode() {return MODELING_MODE_BG_APPROXMEDIAN;};
            int frameOffset() {return 0;};

        private:
            Mat ones_mask;
    };
}

#endif
