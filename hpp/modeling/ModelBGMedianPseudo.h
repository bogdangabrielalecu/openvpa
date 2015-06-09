/***************************************************************
 * Name:      ModelBGMedianPseudo.h
 * Purpose:   Code for ModelBG Class Extension Header
 * Author:    Bogdan Alecu
 * Created:   2011-09-15
 * Copyright: Bogdan Alecu
 * License:   GNU GPL
 **************************************************************/

#ifndef OPENVPAMODELBGMEDIANPSEUDO_H
#define OPENVPAMODELBGMEDIANPSEUDO_H

#include "ModelBG.h"

using namespace std;

namespace openvpa {

    typedef Vec<uchar, 1> Vec1b;

    class ModelBGMedianPseudo: virtual public ModelBG {

        public:
            ModelBGMedianPseudo();
            ~ModelBGMedianPseudo();
            bool reset();
            Mat& get();
            bool update(Mat& frame);
            bool update(Mat& frame, Mat& mask);
            int mode() {return MODELING_MODE_BG_PSEUDOMEDIAN;};
            int frameOffset() {return 0;};

        protected:
            Mat low_bg, high_bg;

        private:
            void initFrames(Mat& frame);
            void pixelUpdate(Mat& frame, int x, int y);
    };
}

#endif
