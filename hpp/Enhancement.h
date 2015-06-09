/***************************************************************
 * Name:      Enhancement.h
 * Purpose:   Code for Enhancement Base Class Header
 * Author:    Bogdan Alecu
 * Created:   2011-09-15
 * Copyright: Bogdan Alecu
 * License:   GNU GPL
 **************************************************************/

#ifndef OPENVPAENHANCEMENT_H
#define OPENVPAENHANCEMENT_H

#include <cv.h>
#include <highgui.h>
#include <memory>

using namespace std;
using namespace cv;

namespace openvpa {

    typedef map<string, double> doubleParams;
    typedef map<string, Mat> matParams;

    class Enhancement {

        protected:
            Mat enhanced_frame;
            doubleParams double_params;
            matParams mat_params;

        public:
            static const int ENHANCEMENT_MODE_NONE = 1;
            static const int ENHANCEMENT_MODE_MORPHOLOGY = 2;
            static const int ENHANCEMENT_MODE_CONTOUR_FILL = 3;
            static const int ENHANCEMENT_MODE_SHRINK_ENLARGE = 4;

            static const int MORPHOLOGY_KERNEL_SHAPE_SQUARE = 1;
            static const int MORPHOLOGY_KERNEL_SHAPE_ROUND = 2;

            Enhancement();
            virtual ~Enhancement() {};
            static auto_ptr<Enhancement> factory(int mode);
            virtual Mat& get(Mat& frm) = 0;
            virtual bool reset() = 0;
            virtual int mode() = 0;
            void paramDouble(string k, double v);
            void paramMat(string k, Mat v);
    };

    class EnhancementNone: virtual public Enhancement {

        public:
            EnhancementNone();
            ~EnhancementNone();
            bool reset();
            Mat& get(Mat& frm);
            int mode() {return ENHANCEMENT_MODE_NONE;};
    };
}

#endif
