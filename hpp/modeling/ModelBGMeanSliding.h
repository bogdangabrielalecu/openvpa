/***************************************************************
 * Name:      ModelBGMeanSliding.cpp
 * Purpose:   Code for ModelBG Class Extension
 * Author:    Bogdan Alecu
 * Created:   2011-09-15
 * Copyright: Bogdan Alecu
 * License:   GNU GPL
 **************************************************************/

#ifndef OPENVPAMODELBGMEANSLIDING_H
#define OPENVPAMODELBGMEANSLIDING_H

#include "ModelBG.h"

using namespace std;

namespace openvpa {

    class ModelBGMeanSliding: virtual public ModelBG {

        public:
            ModelBGMeanSliding();
            ~ModelBGMeanSliding();
            bool reset();
            Mat& get();
            bool update(Mat& frame);
            bool update(Mat& frame, Mat& mask);
            int mode() {return MODELING_MODE_BG_SLIDING_MEAN;};
            int frameOffset() {return 0;};
            void alpha(double v);
            double alpha();
    };
}

#endif
