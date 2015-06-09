/***************************************************************
 * Name:      ModelBGMedian.h
 * Purpose:   Code for ModelBG Class Extension Header
 * Author:    Bogdan Alecu
 * Created:   2011-09-15
 * Copyright: Bogdan Alecu
 * License:   GNU GPL
 **************************************************************/

#ifndef OPENVPAMODELBGMEDIAN_H
#define OPENVPAMODELBGMEDIAN_H

#include "ModelBG.h"

using namespace std;

namespace openvpa {

    typedef vector<uchar> bVec;
    typedef vector<bVec> PixelVec;
    typedef vector<PixelVec> VecPixelVec;

    class ModelBGMedian: virtual public ModelBG {

        public:
            ModelBGMedian();
            ~ModelBGMedian();
            bool reset();
            Mat& get();
            bool update(Mat& frame);
            bool update(Mat& frame, Mat& mask);
            int mode() {return MODELING_MODE_BG_MEDIAN;};
            int frameOffset() {return 0;};

        protected:
            VecPixelVec frames_median_list;
            Mat previous_bg;
            Size bg_size;

        private:
            Mat& extractFromMedianList();
            int initMedianList(Mat frame);
            bool medianListEmpty();
    };
}

#endif
