/***************************************************************
 * Name:      Model.h
 * Purpose:   Code for Model Base Class Header
 * Author:    Bogdan Alecu
 * Created:   2011-09-15
 * Copyright: Bogdan Alecu
 * License:   GNU GPL
 **************************************************************/

#ifndef OPENVPAMODEL_H
#define OPENVPAMODEL_H

#include <cv.h>
#include <highgui.h>
#include <memory>
#include <iostream>

using namespace std;
using namespace cv;

namespace openvpa {

    typedef map<string, double> doubleParams;
    typedef map<string, Mat> matParams;

    class Model {

        protected:
            int modeling_type;
            bool updated;
            doubleParams double_params;
            matParams mat_params;
            Mat current_frame;

        public:
            static const int MODELING_TYPE_BG = 1;
            static const int MODELING_TYPE_FG = 2;

            static const int MODELING_MODE_NONE = 1;
            static const int MODELING_MODE_FG_3SF_ABSDIFF_AND = 2;
            static const int MODELING_MODE_FG_3SF_ABSDIFF_MIN = 3;
            static const int MODELING_MODE_FG_NONPARAM_DIST = 10;
            static const int MODELING_MODE_FG_GAUSSIAN_MIXTURE = 11;
            static const int MODELING_MODE_BG_MEAN = 4;
            static const int MODELING_MODE_BG_SLIDING_MEAN = 5;
            static const int MODELING_MODE_BG_PSEUDOMEDIAN = 6;
            static const int MODELING_MODE_BG_APPROXMEDIAN = 7;
            static const int MODELING_MODE_BG_MEDIAN = 8;
            static const int MODELING_MODE_BG_PREV_FRAME = 9;

            Model();
            virtual ~Model() {};
            static auto_ptr<Model> factory(int mode);
            virtual Mat& get() = 0;
            virtual bool update(Mat& frame) = 0;
            virtual bool update(Mat& frame, Mat& mask) = 0;
            virtual bool reset() = 0;
            virtual bool isEmpty() = 0;
            virtual int type() = 0;
            virtual int mode() = 0;
            virtual int frameOffset() = 0;
            void paramDouble(string k, double v);
            void paramMat(string k, Mat v);
            void currentFrame(Mat& frame);
    };
}

#endif
