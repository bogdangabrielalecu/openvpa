/***************************************************************
 * Name:      Binarization.h
 * Purpose:   Code for Binarization Base Class Header
 * Author:    Bogdan Alecu
 * Created:   2011-09-15
 * Copyright: Bogdan Alecu
 * License:   GNU GPL
 **************************************************************/

#ifndef OPENVPABINARIZATION_H
#define OPENVPABINARIZATION_H

#include <cv.h>
#include <highgui.h>
#include <memory>

using namespace std;
using namespace cv;

namespace openvpa {

    typedef map<string, double> doubleParams;
    typedef map<string, Mat> matParams;

    class Binarization {

        protected:
            Mat binarized_frame;
            Vec3d inner_levels;
            double inner_level;
            int inner_type;
            int inner_way;

        public:
            static const int BINARIZATION_MODE_NONE = 1;
            static const int BINARIZATION_MODE_LUMINANCE = 2;
            static const int BINARIZATION_MODE_CHROMINANCE = 3;
            static const int BINARIZATION_MODE_YCBCR = 4;
            static const int BINARIZATION_MODE_AT_LEAST_ONE = 5;
            static const int BINARIZATION_MODE_ALL = 6;
            static const int BINARIZATION_MODE_MEAN = 7;

            static const int BINARIZE_GENERIC_LEVEL = 1;
            static const int BINARIZE_PER_PLANE_LEVEL = 2;

            static const int BINARIZATION_WAY_DIRECT = 1;
            static const int BINARIZATION_WAY_INVERSE = 2;

            Binarization();
            virtual ~Binarization() {};
            static auto_ptr<Binarization> factory(int mode);
            virtual Mat& get(Mat& frm) = 0;
            virtual bool reset() = 0;
            virtual int mode() = 0;
            virtual int allowedTypes() = 0;
            void level(double val);
            double level();
            void levels(Vec3d vec);
            void levels(double v1, double v2, double v3);
            Vec3d levels();
            void type(int val);
            int type();
            void way(int val);
            int way();
    };

    class BinarizationNone: virtual public Binarization {

        public:
            BinarizationNone();
            ~BinarizationNone();
            bool reset();
            Mat& get(Mat& frm);
            int mode() {return BINARIZATION_MODE_NONE;};
            int allowedTypes() {return BINARIZE_PER_PLANE_LEVEL;};
    };
}

#endif
