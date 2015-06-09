/***************************************************************
 * Name:      ModelBGMean.h
 * Purpose:   Code for ModelBG Class Extension Header
 * Author:    Bogdan Alecu
 * Created:   2011-09-15
 * Copyright: Bogdan Alecu
 * License:   GNU GPL
 **************************************************************/

#ifndef OPENVPAMODELBGMEAN_H
#define OPENVPAMODELBGMEAN_H

#include "ModelBG.h"

using namespace std;

namespace openvpa {

    class ModelBGMean: virtual public ModelBG {

        public:
            ModelBGMean();
            ~ModelBGMean();
            bool reset();
            Mat& get();
            bool update(Mat& frame);
            bool update(Mat& frame, Mat& mask);
            int mode() {return MODELING_MODE_BG_MEAN;};
            int frameOffset() {return 0;};

        private:
            unsigned int processed_frames; // bad juju, will eventually reach it's limits
            Mat previous_bg;
    };
}

#endif
