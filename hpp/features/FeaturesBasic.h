/***************************************************************
 * Name:      FeaturesBasic.h
 * Purpose:   Code for Features Class Extension Header
 * Author:    Bogdan Alecu
 * Created:   2011-09-17
 * Copyright: Bogdan Alecu
 * License:   GNU GPL
 **************************************************************/

#ifndef OPENVPAFEATURESBASIC_H
#define OPENVPAFEATURESBASIC_H

#include "Features.h"

using namespace std;

namespace openvpa {

    typedef Vec<uchar, 1> Vec1b;
    typedef vector<FrameObjects> FrameObjectsVector;
    typedef map<unsigned int, double> UintDoubleMap;
    typedef map<unsigned int, UintDoubleMap> UintUintDoubleMap;
    typedef map<unsigned int, unsigned int> UintUintMap;

    class FeaturesBasic: virtual public Features {

        public:
            static const unsigned int HIST_COMPONENT_CHUNK_NUMBER_POWER = 2;
            static const unsigned int BUFFER_LENGTH = 20;
            static const unsigned int CLASS_BUFFER_LENGTH = 2000;
            static const unsigned int LOOKBACK_BUFFER_LENGTH = 10;

            FeaturesBasic();
            ~FeaturesBasic();
            bool reset();
            bool extract(Mat& frm, Mat& crt_frm);
            int mode() {return FEATURE_MODE_BASIC;};
            Mat getObjectsFrame();
            FrameObjects& getFrameObjects();

        protected:
            bool exploreVicinity(Mat& frm, Mat& crt_frm, int x, int y, ObjAttr& new_attr);
            void newFoundObject(Mat& frm, Mat& crt_frm, int x, int y);
            void markPixel(Mat& frm, Mat& crt_frm, int x, int y, ObjAttr& new_attr);
            void finishStats(ObjAttr& new_attr);
            vector<string> rgbHistFragments();
            string histIntervalKey(string color, int value);
            string objStatsString(ObjAttr attr);
            void histSplit(ObjAttr& attr, string color, map<int, int>& m);
            void resetRGBHist();
            void addFrameObjectsToQueue();
            unsigned int newClass();
            bool classifyFrameObjects();
            bool assignClass(unsigned int classid, ObjAttr& obj_attr);
            bool assignTypeClass(ObjAttr& obj_attr);
            bool possiblyHuman(ObjAttr& obj_attr);
            bool possiblyCar(ObjAttr& obj_attr);
            double classificationDistance(ObjAttr& obj1, ObjAttr& obj2);
            UintUintMap extractClass(UintUintDoubleMap& d);
            void drawFrameObjectsInformation();
            bool isValidObject(ObjAttr& attr);
            bool ignoreObject(ObjAttr& attr);

        private:
            FrameObjectsVector mv;
            unsigned int step;
            unsigned int chunks;
            map<int, int> r, g, b;
            unsigned int queue_index;
            unsigned int class_index;
            vector<string> classification_attrs;
            ObjAttr tmp_obj;
    };
}

#endif
