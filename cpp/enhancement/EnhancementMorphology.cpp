/***************************************************************
 * Name:      EnhancementMorphology.cpp
 * Purpose:   Code for Enhancement Class Extension
 * Author:    Bogdan Alecu
 * Created:   2011-09-16
 * Copyright: Bogdan Alecu
 * License:   GNU GPL
 **************************************************************/

#include "enhancement/EnhancementMorphology.h"
#include "Data.h"

namespace openvpa {

    EnhancementMorphology::EnhancementMorphology() : Enhancement::Enhancement() {
        reset();
    }

    EnhancementMorphology::~EnhancementMorphology() {
        //
    }

    bool EnhancementMorphology::reset() {
        double_params["morphological_window_size"] = 5;
        double_params["morphological_window_shape"] = MORPHOLOGY_KERNEL_SHAPE_ROUND;
        double_params["morphological_close_count"] = 1;
        double_params["morphological_open_count"] = 1;
        return true;
    }

    Mat& EnhancementMorphology::get(Mat& frm) {
        static IplConvKernel* open_conv_kern;
        static IplConvKernel* close_conv_kern;
        if (!open_conv_kern) {
            open_conv_kern = cvCreateStructuringElementEx(
                (int) double_params["morphological_window_size"],
                (int) double_params["morphological_window_size"],
                (int) (double_params["morphological_window_size"] / 2),
                (int) (double_params["morphological_window_size"] / 2),
                (((int) double_params["morphological_window_shape"] == MORPHOLOGY_KERNEL_SHAPE_ROUND) ?
                    CV_SHAPE_ELLIPSE :
                    CV_SHAPE_RECT),
                NULL);
        }
        if (!close_conv_kern) {
            close_conv_kern = cvCreateStructuringElementEx(
                (int) double_params["morphological_window_size"],
                (int) double_params["morphological_window_size"],
                (int) (double_params["morphological_window_size"] / 2),
                (int) (double_params["morphological_window_size"] / 2),
                (((int) double_params["morphological_window_shape"] == MORPHOLOGY_KERNEL_SHAPE_ROUND) ?
                    CV_SHAPE_ELLIPSE :
                    CV_SHAPE_RECT),
                NULL);
        }
        IplImage temp, ipli_morph;
        ipli_morph = (IplImage) frm;

        cvMorphologyEx(&ipli_morph, &ipli_morph, &temp, close_conv_kern, CV_MOP_CLOSE, (int) double_params["morphological_close_count"]);
        cvMorphologyEx(&ipli_morph, &ipli_morph, &temp, open_conv_kern, CV_MOP_OPEN, (int) double_params["morphological_open_count"]);

        Mat mat_morph(&ipli_morph);
        mat_morph = &ipli_morph;
        mat_morph.copyTo(enhanced_frame);
        return enhanced_frame;
    }
}
