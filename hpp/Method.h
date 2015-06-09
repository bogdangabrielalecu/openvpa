/***************************************************************
 * Name:      Method.h
 * Purpose:   Code for Method Base Class Header
 * Author:    Bogdan Alecu
 * Created:   2011-09-15
 * Copyright: Bogdan Alecu
 * License:   GNU GPL
 **************************************************************/

#ifndef OPENVPAMETHOD_H
#define OPENVPAMETHOD_H

#include <string>
#include <iostream>
#include <fstream>
#include "Data.h"
#include "Detector.h"

using namespace std;

namespace openvpa {

    class Method {

        private:

            Data data_obj;
            Detector detector_obj;
            virtual string logMethodName() = 0;
            virtual string logMethodDesc() = 0;
            virtual int logMethodParams(ofstream &fs) = 0;

        protected:
            //
            virtual int prepare() = 0;
            virtual int process(IplImage* frame) = 0;
            virtual int cleanup() = 0;

            void default_init();
            void data(Data& data);
            Data& data();
            void detector(Detector& detector);
            Detector& detector();
            int log(int logLevel = 3);
            int logMethodGenericParams(ofstream &fs);

        public:
            Method();
            Method(int src);
            ~Method();
            int execute();
            int reset();
    };
}
#endif
