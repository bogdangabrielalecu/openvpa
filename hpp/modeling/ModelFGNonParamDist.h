/***************************************************************
 * Name:      ModelFGNonParamDist.h
 * Purpose:   Code for ModelFG Class Extension Header
 * Author:    Bogdan Alecu
 * Created:   2011-09-17
 * Copyright: Bogdan Alecu
 * License:   GNU GPL
 **************************************************************/

#ifndef OPENVPAMODELFGNONPARAMDIST_H
#define OPENVPAMODELFGNONPARAMDIST_H

#include "ModelFG.h"

using namespace std;

namespace openvpa {

    typedef map<double, double> GaussMap;
    typedef map<double, GaussMap> VarGaussMap;
    typedef vector<Mat> MatVector;

    class ModelFGNonParamDist: virtual public ModelFG {

        public:
            ModelFGNonParamDist();
            ~ModelFGNonParamDist();
            bool reset();
            Mat& get();
            bool update(Mat& frame);
            bool update(Mat& frame, Mat& mask);
            int mode() {return MODELING_MODE_FG_NONPARAM_DIST;};
            int frameOffset() {return 0;};

            static const float DEFAULT_VARIANCE;
            static const int DEFAULT_BUFFER_LENGTH;
            static const float DEFAULT_GRID_STEP;

        protected:
            static const float PI;
            unsigned int index;
            bool full_buffer;
            MatVector mv;
            VarGaussMap map;
            double gaussian(double x, double var = 1);
            double gaussianMap(double x, double var = 1);
            double gaussianMapWithGrid(double x, double var = 1);
            double gaussianVariance();
            unsigned int bufferLength();
            unsigned int indexOf(unsigned int v = 0);
            void nextIndex();
            double nearestGridValue(double x, string s);
            double gridStep(string s);
            Mat& extractFromGaussianSum();
    };
}

#endif
