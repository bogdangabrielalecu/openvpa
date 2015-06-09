/***************************************************************
 * Name:      Isolation.h
 * Purpose:   Code for Isolation Base Class Header
 * Author:    Bogdan Alecu
 * Created:   2011-09-15
 * Copyright: Bogdan Alecu
 * License:   GNU GPL
 **************************************************************/

#ifndef OPENVPAISOLATION_H
#define OPENVPAISOLATION_H

#include <cv.h>
#include <highgui.h>
#include <memory>

using namespace std;
using namespace cv;

namespace openvpa {

    typedef map<string, double> doubleParams;
    typedef map<string, Mat> matParams;

    class Isolation {

        protected:
            Mat isolated_frame;

        public:
            static const int ISOLATION_MODE_NONE = 1;
            static const int ISOLATION_MODE_DIFF = 2;
            static const int ISOLATION_MODE_ABSDIFF = 3;

            Isolation();
            virtual ~Isolation() {};
            static auto_ptr<Isolation> factory(int mode);
            virtual Mat& get(Mat& bg, Mat& frm) = 0;
            virtual bool reset() = 0;
            virtual int mode() = 0;
    };

    class IsolationNone: virtual public Isolation {

        public:
            IsolationNone();
            ~IsolationNone();
            bool reset();
            Mat& get(Mat& bg, Mat& frm);
            int mode() {return ISOLATION_MODE_NONE;};
    };
}

#endif
