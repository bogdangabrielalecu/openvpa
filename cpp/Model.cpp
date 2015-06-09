/***************************************************************
 * Name:      Model.cpp
 * Purpose:   Code for Model Base Class
 * Author:    Bogdan Alecu
 * Created:   2011-09-15
 * Copyright: Bogdan Alecu
 * License:   GNU GPL
 **************************************************************/

#include "Model.h"

//list of the Model extended implementations
#include "modeling/ModelBGMean.h"
#include "modeling/ModelBGMeanSliding.h"
#include "modeling/ModelBGMedian.h"
#include "modeling/ModelBGMedianPseudo.h"
#include "modeling/ModelBGMedianApproximation.h"
#include "modeling/ModelBGPreviousFrame.h"
#include "modeling/ModelFG3SFADMin.h"
#include "modeling/ModelFG3SFADAnd.h"
#include "modeling/ModelFGNonParamDist.h"
#include "modeling/ModelFGGaussianMixture.h"

namespace openvpa {

    Model::Model() {
        double_params.clear();
        mat_params.clear();
    }

    auto_ptr<Model> Model::factory(int mode) {
        if (MODELING_MODE_BG_MEAN == mode) {
            auto_ptr<Model> p(new ModelBGMean());
            return p;
        }
        if (MODELING_MODE_BG_SLIDING_MEAN == mode) {
            auto_ptr<Model> p(new ModelBGMeanSliding());
            return p;
        }
        if (MODELING_MODE_BG_MEDIAN == mode) {
            auto_ptr<Model> p(new ModelBGMedian());
            return p;
        }
        if (MODELING_MODE_BG_PSEUDOMEDIAN == mode) {
            auto_ptr<Model> p(new ModelBGMedianPseudo());
            return p;
        }
        if (MODELING_MODE_BG_APPROXMEDIAN == mode) {
            auto_ptr<Model> p(new ModelBGMedianApproximation());
            return p;
        }
        if (MODELING_MODE_BG_PREV_FRAME == mode) {
            auto_ptr<Model> p(new ModelBGPreviousFrame());
            return p;
        }
        if (MODELING_MODE_FG_3SF_ABSDIFF_MIN == mode) {
            auto_ptr<Model> p(new ModelFG3SFADMin());
            return p;
        }
        if (MODELING_MODE_FG_3SF_ABSDIFF_AND == mode) {
            auto_ptr<Model> p(new ModelFG3SFADAnd());
            return p;
        }
        if (MODELING_MODE_FG_NONPARAM_DIST == mode) {
            auto_ptr<Model> p(new ModelFGNonParamDist());
            return p;
        }
        if (MODELING_MODE_FG_GAUSSIAN_MIXTURE == mode) {
            auto_ptr<Model> p(new ModelFGGaussianMixture());
            return p;
        }
        // for failsafe and compiler warning free sake ..
        auto_ptr<Model> p(new openvpa::ModelBGMean());
        return p;
    }

    void Model::paramDouble(string k, double v) {
        double_params[k] = v;
    }

    void Model::paramMat(string k, Mat v) {
        mat_params[k] = v;
    }

    void Model::currentFrame(Mat& frame) {
        current_frame = frame;
    }
}
