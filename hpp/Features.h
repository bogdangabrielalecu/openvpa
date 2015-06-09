/***************************************************************
 * Name:      Features.h
 * Purpose:   Code for Feature Extraction, Tracking and Classification Base Class Header
 * Author:    Bogdan Alecu
 * Created:   2011-09-16
 * Copyright: Bogdan Alecu
 * License:   GNU GPL
 **************************************************************/

#ifndef OPENVPAFEATURES_H
#define OPENVPAFEATURES_H

#include <cv.h>
#include <highgui.h>
#include <memory>
#include <iostream>

using namespace std;
using namespace cv;

namespace openvpa {

    typedef map<string, double> doubleParams;
    typedef map<string, Mat> matParams;
    typedef map<string, double> Attrs;
    struct ObjAttr {
//        unsigned int S, xc, yc, xl, yl, xh, yh, r_avg, g_avg, b_avg;
        unsigned int id, classid;
        Attrs attrs;
        Vec3b seg_col;
    };
    typedef map<unsigned int, ObjAttr> ObjAttrMap;
    struct FrameObjects {
        ObjAttrMap map;
        Mat frm;
    };

    class Features {

        protected:
            FrameObjects obj;
            doubleParams double_params;
            matParams mat_params;
            unsigned int obj_counter;
            ObjAttr newObjAttr();
            Vec3b colorVec3b(unsigned int id);

        public:
            static const int FEATURE_MODE_NONE = 1;
            static const int FEATURE_MODE_BASIC = 2;
            Features();
            virtual ~Features() {};
            static auto_ptr<Features> factory(int mode);
            virtual bool extract(Mat& frm, Mat& crt_frm) = 0;
            virtual Mat getObjectsFrame() = 0;
            virtual FrameObjects& getFrameObjects() = 0;
            virtual bool reset() = 0;
            virtual int mode() = 0;
            void resetObjCounter();
            void resetObjects();
            void paramDouble(string k, double v);
            void paramMat(string k, Mat v);
    };

    class FeaturesNone: virtual public Features {

        public:
            FeaturesNone();
            ~FeaturesNone();
            bool reset();
            bool extract(Mat& frm, Mat& crt_frm);
            Mat getObjectsFrame();
            FrameObjects& getFrameObjects();
            int mode() {return FEATURE_MODE_NONE;};

        protected:
            Mat out_frm;
    };

}

#endif
