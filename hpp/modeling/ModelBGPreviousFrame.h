/***************************************************************
 * Name:      ModelBGPreviousFrame.h
 * Purpose:   Code for ModelBG Class Extension Header
 * Author:    Bogdan Alecu
 * Created:   2011-09-15
 * Copyright: Bogdan Alecu
 * License:   GNU GPL
 **************************************************************/

#ifndef OPENVPAMODELBGPREVFRAME_H
#define OPENVPAMODELBGPREVFRAME_H

#include "ModelBG.h"

using namespace std;

namespace openvpa {

    class ModelBGPreviousFrame: virtual public ModelBG {

        public:
            ModelBGPreviousFrame();
            ~ModelBGPreviousFrame();
            bool reset();
            Mat& get();
            bool update(Mat& frame);
            bool update(Mat& frame, Mat& mask);
            int mode() {return MODELING_MODE_BG_PREV_FRAME;};
            int frameOffset() {return 0;};

        protected:
            Mat pre_bg, pre_mask;

    };
}

#endif
