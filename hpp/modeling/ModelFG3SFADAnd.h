/***************************************************************
 * Name:      ModelFG3SFADAnd.h
 * Purpose:   Code for ModelFG Class Extension Header
 * Author:    Bogdan Alecu
 * Created:   2011-09-15
 * Copyright: Bogdan Alecu
 * License:   GNU GPL
 **************************************************************/

#ifndef OPENVPAMODELFG3SFADAND_H
#define OPENVPAMODELFG3SFADAND_H

#include "ModelFG.h"

using namespace std;

namespace openvpa {

    typedef vector<Mat> MatVector;

    class ModelFG3SFADAnd: virtual public ModelFG {

        public:
            ModelFG3SFADAnd();
            ~ModelFG3SFADAnd();
            bool reset();
            Mat& get();
            bool update(Mat& frame);
            bool update(Mat& frame, Mat& mask);
            int mode() {return MODELING_MODE_FG_3SF_ABSDIFF_AND;};
            int frameOffset() {return 1;};

        protected:
            MatVector mv;
            int index;
            bool full_buffer;
            unsigned int indexOf(unsigned int v = 0);
            void nextIndex();
    };
}

#endif
