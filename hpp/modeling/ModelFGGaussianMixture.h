/***************************************************************
 * Name:      ModelFGGaussianMixture.h
 * Purpose:   Code for ModelFG Class Extension Header
 * Author:    Bogdan Alecu
 * Created:   2011-09-16
 * Copyright: Bogdan Alecu
 * License:   GNU GPL
 **************************************************************/

#ifndef OPENVPAMODELFGGAUSSIANMIXTURE_H
#define OPENVPAMODELFGGAUSSIANMIXTURE_H

#include "ModelFG.h"

using namespace std;

namespace openvpa {

    typedef map<int, double> IntGaussMap;

    class ModelFGGaussianMixture: virtual public ModelFG {

        public:
            ModelFGGaussianMixture();
            ~ModelFGGaussianMixture();
            bool reset();
            Mat& get();
            bool update(Mat& frame);
            bool update(Mat& frame, Mat& mask);
            int mode() {return MODELING_MODE_FG_GAUSSIAN_MIXTURE;};
            int frameOffset() {return 0;};

            static const float DEFAULT_VARIANCE;
            static const int DEFAULT_VICINITY_RADIUS;

        protected:
            static const float PI;
            IntGaussMap map;
            double gaussian(int x, double var = 1);
            double gaussianMap(int x);
            double gaussianVariance();
            int gaussVicinityRadius();
            void generateGaussians();
            Mat& extractFromGaussianSum();
    };
}

#endif
