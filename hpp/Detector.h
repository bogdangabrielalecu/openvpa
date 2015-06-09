/***************************************************************
 * Name:      Detector.h
 * Purpose:   Code for Detector Mid Tier Class Header
 * Author:    Bogdan Alecu
 * Created:   2011-09-16
 * Copyright: Bogdan Alecu
 * License:   GNU GPL
 **************************************************************/

#ifndef OPENVPADETECTOR_H
#define OPENVPADETECTOR_H

#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <vector>
#include <math.h>

#include "TimeStats.h"
#include "Model.h"
#include "Isolation.h"
#include "Binarization.h"
#include "Enhancement.h"
// temporary
#include "Features.h"

using namespace cv;
using namespace std;

namespace openvpa {

    typedef Vec<uchar, 1> Vec1b;

    class Detector {

        private:
            Size source_frame_size;
            Size modeling_frame_size;
            Size generic_frame_size;
            Size isolation_frame_size;
            Size binarization_frame_size;
            Size enhancement_frame_size;
            Size feature_frame_size;
            Mat frame1;
            Mat frame2;
            Mat frame3;
            Mat pre_modeling_frame1;
            Mat pre_modeling_frame2;
            Mat pre_modeling_frame3;
            Mat bg_frame;
            Mat pre_isolation_frame3;
            Mat pre_isolation_bg;
            Mat pre_isolation_fg;
            Mat isolated_frame;
            Mat pre_binarization_frame;
            Mat binarized_frame;
            Mat pre_enhancement_frame;
            Mat pre_feature_binarized;
            Mat pre_feature_current;
            Mat masked_fg_frame;
            Mat masked_bg_frame;
            int processed_frames;
            auto_ptr<Model> model_obj;
            auto_ptr<Isolation> isolation_obj;
            auto_ptr<Binarization> binarize_obj;
            auto_ptr<Enhancement> enhance_obj;
            auto_ptr<Features> features_obj;
            int model_update_type;
            int resetModels();
            int resetIsolation();
            int preModelEvent();
            int modelNone();
            int setModelingFrames();
            int scaleModelingInputFrames();
            int isolation_mode;
            int setIsolationFrames();
            int scaleIsolationInputFrames();
            int preIsolateEvent();
            int binarization_mode;
            int binarization_level;
            bool binarization_per_plane_level_enabled;
            vector<int> binarization_levels;
            int resetBinarize();
            map<int, int> binarizationModeLevelTypeMap();
            int setBinarizationFrames();
            int scaleBinarizationInputFrame();
            int preBinarizeEvent();
            int postBinarizeEvent();
            int binarizeFeedback();
            void resetComputedFrames();
            int enhancement_mode;
            int setEnhancementFrames();
            int scaleEnhancementInputFrame();
            int preEnhanceEvent();
            int postEnhanceEvent();
            int enhanceFeedback();
            int setFeatureFrames();
            int scaleFeatureInputFrames();
            int preFeatureEvent();
            int resetEnhance();
            void default_init();
            TimeStats time_stats;

        public:
            Detector();
            ~Detector();
            static const int MODELING_UPDATE_USING_ORIGINAL = 1;
            static const int MODELING_UPDATE_USING_BINARIZED = 2;
            static const int MODELING_UPDATE_USING_ENHANCED = 3;
            static const int ISOLATION_MODE_NONE = 1;
            static const int ISOLATION_MODE_ABSDIFF = 2;
            static const int ISOLATION_MODE_DIFF = 3;
            static const int BINARIZE_MODE_LUMINANCE = 1;
            static const int BINARIZE_MODE_CHROMINANCE = 2;
            static const int BINARIZE_MODE_YCBCR = 3;
            static const int BINARIZE_MODE_ONE = 4;
            static const int BINARIZE_MODE_ALL = 5;
            static const int BINARIZE_MODE_MEAN = 6;
            static const int BINARIZE_MODE_GAUSSIAN_MIX = 7; // ??? BUGGY
            static const int BINARIZE_GENERIC_LEVEL = 1;
            static const int BINARIZE_PER_PLANE_LEVEL = 2;
            static const int BINARIZE_LEVEL_LOW = -10001;
            static const int BINARIZE_LEVEL_MED = -10002;
            static const int BINARIZE_LEVEL_HIGH = -10003;
            static const int ENHANCEMENT_MODE_NONE = 1;
            static const int ENHANCEMENT_MODE_MORPHOLOGY_OPEN_CLOSE = 2;
            static const int ENHANCEMENT_MODE_CONTOUR_FILL = 3;
            static const int ENHANCEMENT_MODE_SHRINK_ENLARGE = 4;
            static const int MORPHOLOGY_KERNEL_SHAPE_SQUARE = 1;
            static const int MORPHOLOGY_KERNEL_SHAPE_ROUND = 2;
            static const string EXPORT_DIR_EXTRACTED;
            static const string EXPORT_DIR_MODELED;
            static const string EXPORT_DIR_ISOLATED;
            static const string EXPORT_DIR_BINARIZED;
            static const string EXPORT_DIR_ENHANCED;
            static const string EXPORT_DIR_FEATURES;
            static const string EXPORT_DIR_FG_MAPPED;
            static const string EXPORT_DIR_BG_MAPPED;

            static const double PI;
            int reset();
            void sourceFrameSize(Size size);
            void sourceFrameSize(int cols, int rows);
            void sourceFrameSize(string size_str);
            Size sourceFrameSize();
            void genericFrameSize(Size size);
            void genericFrameSize(int cols, int rows);
            void genericFrameSize(string size_str);
            Size genericFrameSize();
            void modelingFrameSize(Size size);
            void modelingFrameSize(int cols, int rows);
            void modelingFrameSize(string size_str);
            Size modelingFrameSize();
            void modelingMode(int mode);
            int modelingMode();
            Mat modeledFrame();
            int model(int mode);
            int model();
            void isolationFrameSize(Size size);
            void isolationFrameSize(int cols, int rows);
            void isolationFrameSize(string size_str);
            Size isolationFrameSize();
            void binarizationFrameSize(Size size);
            void binarizationFrameSize(int cols, int rows);
            void binarizationFrameSize(string size_str);
            Size binarizationFrameSize();
            void binarizationPerPlaneLevelEnabled(bool value);
            bool binarizationPerPlaneLevelEnabled();
            void enhancementFrameSize(Size size);
            void enhancementFrameSize(int cols, int rows);
            void enhancementFrameSize(string size_str);
            Size enhancementFrameSize();
            void featureFrameSize(Size size);
            void featureFrameSize(int cols, int rows);
            void featureFrameSize(string size_str);
            Size featureFrameSize();
            int resolutionStrToIntPair(string &size_str, pair<int, int> &size_int_pair);
            void setFrame1(Mat frame);
            void setFrame2(Mat frame);
            void setFrame3(Mat frame);
            int enqueueFrame(Mat frame);
            bool threeFramesEnqueued();
            bool twoFramesEnqueued();
            bool oneFrameEnqueued();
            void modelUpdateType(int type);
            int modelUpdateType();
            int modelUpdate(Mat mask);
            int modelUpdate();
            auto_ptr<Model>& modelObj() {return model_obj;};
            void isolationMode(int mode);
            int isolationMode();
            int isolate(int mode);
            int isolate();
            Mat isolatedFrame() {return isolated_frame;};
            Mat binarizedFrame() {return binarized_frame;};
            auto_ptr<Binarization>& binarizeObj() {return binarize_obj;};
            void binarizationMode(int mode);
            int binarizationMode();
            void binarizationLevel(int level);
            void binarizationLevels(int r_level, int g_level, int b_level);
            vector<int> binarizationLevels();
            int binarizationLevel();
            int binarize(int mode, int level);
            int binarize(int mode, vector<int> levels);
            int binarize();
            void enhancementMode(int mode);
            int enhancementMode();
            int enhance(int mode);
            int enhance();
            auto_ptr<Enhancement>& enhanceObj() {return enhance_obj;};
            void morphologicalWindowSize(int size);
            int morphologicalWindowSize();
            void morphologicalWindowShape(int shape);
            int morphologicalWindowShape();
            void morphologicalOpenCount(int cnt);
            int morphologicalOpenCount();
            void morphologicalCloseCount(int cnt);
            int morphologicalCloseCount();
            int morphologyOpenClose();
            int enhancementContourFill();
            int maskFramePixels();
            Mat maskedFrame();
            static Vec1b rgb2y(Vec3b rgb);
            static Mat rgb2y(Mat frame);
            static Vec3b rgb2ycbcr(Vec3b rgb);
            static Mat rgb2ycbcr(Mat frame);
            Mat rgb2ycbcr();
            Mat scaleBy(Mat &img, float ratio);
            Mat scaleBy(IplImage *img, float ratio);
            Mat scaleAt(Mat &img, Size size);
            Mat scaleAt(IplImage *img, Size size);
            Mat scale(Mat &img);
            Mat scale(IplImage *img);
            Mat normateImg(Mat img, unsigned int lower_value = 0, unsigned int upper_value = 255);
            double gaussianDistrib(double standard_deviation) {return pow(standard_deviation, 2);};
            double bayesMotionThreshold(
                double standard_deviation,
                double variation_range = 255,
                double bg_probability = 0.5,
                double fg_probability = 0.5,
                double cost_estimator = 1
            );
            double modelingParamAlpha();
            void modelingParamAlpha(double alpha);
            double modelingParamBeta();
            void modelingParamBeta(double beta);
            TimeStats& timeStats();
            void timeStatsLevel(int level);
            int resetFeatures();
            void featuresMode(int mode);
            int featuresMode();
            int feature();
            Mat featureFrame();
            Mat feature_frame;// this probably should be private
            FrameObjects frame_objects;
    };
}
#endif

