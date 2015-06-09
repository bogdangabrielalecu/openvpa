/***************************************************************
 * Name:      Detector.cpp
 * Purpose:   Code for Detector Mid Tier Class
 * Author:    Bogdan Alecu
 * Created:   2011-09-16
 * Copyright: Bogdan Alecu
 * License:   GNU GPL
 **************************************************************/

#include "Detector.h"
#include "Data.h"
#include "Model.h"

namespace openvpa {

    const string Detector::EXPORT_DIR_EXTRACTED = string("extracted");
    const string Detector::EXPORT_DIR_MODELED = string("modeled");
    const string Detector::EXPORT_DIR_ISOLATED = string("isolated");
    const string Detector::EXPORT_DIR_BINARIZED = string("binarized");
    const string Detector::EXPORT_DIR_ENHANCED = string("enhanced");
    const string Detector::EXPORT_DIR_FEATURES = string("features");
    const string Detector::EXPORT_DIR_FG_MAPPED = string("fg_mapped");
    const string Detector::EXPORT_DIR_BG_MAPPED = string("bg_mapped");

    const double Detector::PI = 3.14159;

    Detector::Detector() {
        default_init();
    }

    void Detector::default_init() {
        processed_frames = 0;
        time_stats = TimeStats();
        sourceFrameSize(0, 0);
    }

    Detector::~Detector() {
        //
    }

    int Detector::reset() {
        default_init();
        resetModels();
        resetIsolation();
        resetBinarize();
        resetFeatures();
        resetComputedFrames();
        frame1 = Mat();
        frame2 = Mat();
        frame3 = Mat();
        return 1;
    }

    int Detector::resetModels() {
        if (model_obj.get()) {
            model_obj->reset();
        }
        return 1;
    }

    int Detector::resetIsolation() {
        if (isolation_obj.get()) {
            isolation_obj->reset();
        }
        return 1;
    }

    int Detector::resetBinarize() {
        if (binarize_obj.get()) {
            binarize_obj->reset();
        }
        return 1;
    }

    int Detector::resetEnhance() {
        if (enhance_obj.get()) {
            enhance_obj->reset();
        }
        return 1;
    }

    int Detector::resetFeatures() {
        if (features_obj.get()) {
            features_obj->reset();
        }
        return 1;
    }

    void Detector::resetComputedFrames() {
        Mat empty_mat;
        empty_mat.copyTo(bg_frame);
        empty_mat.copyTo(isolated_frame);
        empty_mat.copyTo(binarized_frame);
        empty_mat.copyTo(masked_fg_frame);
        empty_mat.copyTo(masked_bg_frame);
        sourceFrameSize(0, 0);
    }

    void Detector::setFrame1(Mat frame) {
        frame.copyTo(frame1);
        resetComputedFrames();
    }

    void Detector::setFrame2(Mat frame) {
        frame.copyTo(frame2);
        resetComputedFrames();
    }

    void Detector::setFrame3(Mat frame) {
        frame.copyTo(frame3);
        resetComputedFrames();
    }

    int Detector::enqueueFrame(Mat frame) {
        if (frame.empty()) {
            return 0;
        }
        if (!frame2.empty()) {
            frame2.copyTo(frame1);
        }
        if (!frame3.empty()) {
            frame3.copyTo(frame2);
        }
        frame.copyTo(frame3);
        resetComputedFrames();
        processed_frames ++;
        return 1;
    }

    Mat Detector::normateImg(Mat img, unsigned int lower_value, unsigned int upper_value) {
        if (img.empty()) {
            return img;
        }
        double min_val;
        double max_val;
        double minim, maxim;
        vector<Mat> planes;
        split(img, planes);
        for (unsigned int i = 0; i < planes.size(); i ++) {
            minMaxLoc(planes.at(i), &min_val, &max_val);//, NULL, NULL);
            if (0 == i) {
                // first run
                minim = min_val;
                maxim = max_val;
                continue;
            }
            if (minim < min_val) {
                minim = min_val;
            }
            if (maxim > max_val) {
                maxim = max_val;
            }
        }
        double width = 1 + (maxim - minim);
        double required_width = 1 + (upper_value - lower_value);
        double ratio = required_width / width;
        Mat res;
        res = img * ratio;
        double offset = minim * ratio - lower_value;
        subtract(res, offset, res);
        return res;
    }

    double Detector::bayesMotionThreshold(
        double standard_deviation,
        double variation_range, // = 255
        double bg_probability, // = 0.5
        double fg_probability, // = 0.5,
        double cost_estimator // = 1
    ) {
        double sd = gaussianDistrib(standard_deviation);
        return 2 * sd * log((2 * variation_range * bg_probability * cost_estimator) / (fg_probability * sqrt(2 * PI * sd)));
    }

    bool Detector::threeFramesEnqueued() {
        return (!frame3.empty()
            && !frame2.empty()
            && !frame1.empty());
    }

    bool Detector::twoFramesEnqueued() {
        return (!frame3.empty()
            && !frame2.empty());
    }

    bool Detector::oneFrameEnqueued() {
        return (!frame3.empty());
    }

    Mat Detector::rgb2ycbcr() {
        return rgb2ycbcr(frame3);
    }

    Mat Detector::rgb2ycbcr(Mat frame) {
        if (frame.empty()) {
            return frame;
        }
        if (frame.type() != CV_8UC3) {
            cout << "Detector::rgb2ycbcr() - frame type mismatch" << endl;
            return frame;
        }
        Mat frame_ycbcr;
        if (1) {
            frame_ycbcr.create(frame.rows, frame.cols, CV_8UC3);
            Mat_<Vec3b>::iterator it = frame.begin<Vec3b>();
            Mat_<Vec3b>::iterator it_end = frame.end<Vec3b>();
            Mat_<Vec3b>::iterator r_it = frame_ycbcr.begin<Vec3b>();
            Mat_<Vec3b>::iterator r_it_end = frame_ycbcr.end<Vec3b>();

            for (;
                ((it != it_end) && (r_it != r_it_end));
                ++ it, ++ r_it) {
                *r_it = rgb2ycbcr(*it);
            }
        } else {
            cvtColor(frame, frame_ycbcr, CV_RGB2YCrCb); // buggy due to ffmpeg-cv bad conjunction
        }
        return frame_ycbcr;
    }

    Mat Detector::rgb2y(Mat frame) {
        if (frame.empty()) {
            return frame;
        }
        if (frame.type() != CV_8UC3) {
            cout << "Detector::rgb2yc() - frame type mismatch" << endl;
            return frame;
        }
        Mat frame_y;
        if (1) {
            frame_y.create(frame.rows, frame.cols, CV_8UC1);
            Mat_<Vec3b>::iterator it = frame.begin<Vec3b>();
            Mat_<Vec3b>::iterator it_end = frame.end<Vec3b>();
            Mat_<Vec1b>::iterator r_it = frame_y.begin<Vec1b>();
            Mat_<Vec1b>::iterator r_it_end = frame_y.end<Vec1b>();

            for (;
                ((it != it_end) && (r_it != r_it_end));
                ++ it, ++ r_it) {
                *r_it = rgb2y(*it);
            }
        } else {
            cvtColor(frame, frame_y, CV_RGB2GRAY); // buggy due to ffmpeg-cv bad conjunction
        }
        return frame_y;
    }

    Vec3b Detector::rgb2ycbcr(Vec3b rgb) {
        // http://en.wikipedia.org/wiki/YCbCr#JPEG_Conversion, for 0-255 values
        Vec3b ycbcr;
        ycbcr[0] = .299 * rgb[0] + .587 * rgb[1] + .114 * rgb[2];
        ycbcr[1] = 128 - .168736 * rgb[0] - .331264 * rgb[1] + .5 * rgb[2];
        ycbcr[2] = 128 + .5 * rgb[0] - .418688 * rgb[1] - .081312 * rgb[2];
        return ycbcr;
    }

    Vec1b Detector::rgb2y(Vec3b rgb) {
        Vec<double, 1> y;
        y[0] = .299 * rgb[0] + .587 * rgb[1] + .114 * rgb[2];
        return y;
    }

    void Detector::modelingMode(int mode) {
        model_obj.reset();
        model_obj = Model::factory(mode);
    }

    int Detector::modelingMode() {
        return model_obj->mode();
    }

    void Detector::modelUpdateType(int type) {
        model_update_type = type;
    }

    int Detector::modelUpdateType() {
        return model_update_type;
    }

    int Detector::setModelingFrames() {
        if (!frame1.empty()) {
            frame1.copyTo(pre_modeling_frame1);
        }
        if (!frame2.empty()) {
            frame2.copyTo(pre_modeling_frame2);
        }
        if (!frame3.empty()) {
            frame3.copyTo(pre_modeling_frame3);
        }
        return 1;
    }

    int Detector::scaleModelingInputFrames() {
        Size size = modelingFrameSize();
        if ((size.width <= 0)
            || (size.height <= 0)) {
            return 0;
        }
        if (!pre_modeling_frame1.empty()) {
            scaleAt(pre_modeling_frame1, size);
        }
        if (!pre_modeling_frame2.empty()) {
            scaleAt(pre_modeling_frame2, size);
        }
        if (!pre_modeling_frame3.empty()) {
            scaleAt(pre_modeling_frame3, size);
        }
        return 1;
    }

    int Detector::preModelEvent() {
        setModelingFrames();
        scaleModelingInputFrames();
        return 1;
    }

    int Detector::model() {
        preModelEvent();
        int res = 1;
        timeStats().start(EXPORT_DIR_MODELED);
        if (Model::MODELING_TYPE_BG == model_obj->type()) {
            bg_frame = model_obj->get();
            if (bg_frame.empty()) {
                res = 0;
            }
        } else {
            model_obj->currentFrame(pre_modeling_frame3);
            isolated_frame = model_obj->get();
            if (isolated_frame.empty()) {
                res = 0;
            }
        }
        timeStats().stop();
        if ((MODELING_UPDATE_USING_ORIGINAL == modelUpdateType())
            || (0 == res)) {
            modelUpdate();
        }
        return res;
    }

    int Detector::modelUpdate() {
        model_obj->update(pre_modeling_frame3);
        return 1;
    }

    int Detector::modelUpdate(Mat mask) {
        if (mask.size() != pre_modeling_frame3.size()) {
            scaleAt(mask, pre_modeling_frame3.size());
        }
        model_obj->update(pre_modeling_frame3, mask);
        return 1;
    }

    void Detector::isolationMode(int mode) {
        isolation_obj.reset();
        isolation_obj = Isolation::factory(mode);
    }

    void Detector::featuresMode(int mode) {
        features_obj.reset();
        features_obj = Features::factory(mode);
    }

    int Detector::isolationMode() {
        return isolation_obj->mode();
    }

    int Detector::featuresMode() {
        return features_obj->mode();
    }

    int Detector::setIsolationFrames() {
        if (!isolated_frame.empty()) {
            isolated_frame.copyTo(pre_isolation_frame3);
            return 1;
        }
        frame3.copyTo(pre_isolation_frame3);
        bg_frame.copyTo(pre_isolation_bg);
        return 1;
    }

    int Detector::setFeatureFrames() {
        binarized_frame.copyTo(pre_feature_binarized);
        frame3.copyTo(pre_feature_current);
        return 1;
    }

    Mat Detector::modeledFrame() {
        if (!isolated_frame.empty()) {
            return isolated_frame;
        }
        return bg_frame;
    }

    int Detector::scaleIsolationInputFrames() {
        Size size = isolationFrameSize();
        if ((size.width <= 0)
            || (size.height <= 0)) {
            return 0;
        }
        scaleAt(pre_isolation_frame3, size);
        if (!pre_isolation_bg.empty()) {
            scaleAt(pre_isolation_bg, size);
        }
        return 1;
    }

    int Detector::scaleFeatureInputFrames() {
        Size size = featureFrameSize();
        if ((size.width <= 0)
            || (size.height <= 0)) {
            return 0;
        }
        scaleAt(pre_feature_binarized, size);
        scaleAt(pre_feature_current, size);
        return 1;
    }

    int Detector::preIsolateEvent() {
        setIsolationFrames();
        scaleIsolationInputFrames();
        return 1;
    }

    int Detector::preFeatureEvent() {
        setFeatureFrames();
        scaleFeatureInputFrames();
        return 1;
    }

    int Detector::isolate() {
        preIsolateEvent();
        if ((Model::MODELING_TYPE_FG == model_obj->type())) {
            if (isolation_obj->mode() != Isolation::ISOLATION_MODE_NONE) {
                isolationMode(Isolation::ISOLATION_MODE_NONE);
            }
        }
        timeStats().start(EXPORT_DIR_ISOLATED);
        isolated_frame = isolation_obj->get(pre_isolation_bg, pre_isolation_frame3);
        timeStats().stop();
        return 1;
    }

    int Detector::feature() {
        preFeatureEvent();
        timeStats().start(EXPORT_DIR_FEATURES);
        features_obj->extract(pre_feature_binarized, pre_feature_current);
        timeStats().stop();
        feature_frame = Mat(features_obj->getObjectsFrame());
        return 1;
    }

    Mat Detector::featureFrame() {
        return feature_frame;
    }

    int Detector::setBinarizationFrames() {
        isolated_frame.copyTo(pre_binarization_frame);
        return 1;
    }

    int Detector::scaleBinarizationInputFrame() {
        Size size = binarizationFrameSize();
        if ((size.width <= 0)
            || (size.height <= 0)) {
            return 0;
        }
        scaleAt(pre_binarization_frame, size);
        return 1;
    }

    int Detector::preBinarizeEvent() {
        setBinarizationFrames();
        scaleBinarizationInputFrame();
        return 1;
    }

    int Detector::postBinarizeEvent() {
        binarizeFeedback();
        return 1;
    }

    int Detector::binarizeFeedback() {
        if (MODELING_UPDATE_USING_BINARIZED == modelUpdateType()) {
            Mat complementary_frame = (binarized_frame <= 0);
            modelUpdate(complementary_frame);
        }
        return 1;
    }

    int Detector::binarize() {
        if (isolated_frame.empty()) {
            return 0;
        }
        preBinarizeEvent();
        int res = 1;
        timeStats().start(EXPORT_DIR_BINARIZED);
        binarized_frame = binarize_obj->get(pre_binarization_frame);
        if (binarized_frame.empty()) {
            res = 0;
        }
        timeStats().stop();
        postBinarizeEvent();
        return res;
    }

    void Detector::binarizationMode(int mode) {
        binarize_obj.reset();
        binarize_obj = Binarization::factory(mode);
    }

    int Detector::binarizationMode() {
        return binarize_obj->mode();
    }

    void Detector::binarizationLevel(int level) {
        binarization_level = level;
    }

    int Detector::binarizationLevel() {
        return binarization_level;
    }

    void Detector::binarizationLevels(int r_level, int g_level, int b_level) {
        binarization_levels.push_back(r_level);
        binarization_levels.push_back(g_level);
        binarization_levels.push_back(b_level);
    }

    vector<int> Detector::binarizationLevels() {
        return binarization_levels;
    }

    map<int, int> Detector::binarizationModeLevelTypeMap() {
        map<int, int> m;
        m[(int)BINARIZE_MODE_LUMINANCE] = BINARIZE_GENERIC_LEVEL;
        m[(int)BINARIZE_MODE_CHROMINANCE] = BINARIZE_GENERIC_LEVEL;
        m[(int)BINARIZE_MODE_YCBCR] = BINARIZE_GENERIC_LEVEL | BINARIZE_PER_PLANE_LEVEL;
        m[(int)BINARIZE_MODE_ONE] = BINARIZE_GENERIC_LEVEL | BINARIZE_PER_PLANE_LEVEL;
        m[(int)BINARIZE_MODE_ALL] = BINARIZE_GENERIC_LEVEL | BINARIZE_PER_PLANE_LEVEL;
        m[(int)BINARIZE_MODE_MEAN] = BINARIZE_GENERIC_LEVEL;
        return m;
    }

    void Detector::binarizationPerPlaneLevelEnabled(bool value) {
        binarization_per_plane_level_enabled = value;
    }

    bool Detector::binarizationPerPlaneLevelEnabled() {
        return binarization_per_plane_level_enabled;
    }

    int Detector::setEnhancementFrames() {
        binarized_frame.copyTo(pre_enhancement_frame);
        return 1;
    }

    int Detector::scaleEnhancementInputFrame() {
        Size size = enhancementFrameSize();
        if ((size.width <= 0)
            || (size.height <= 0)) {
            return 0;
        }
        scaleAt(pre_enhancement_frame, size);
        return 1;
    }

    void Detector::enhancementMode(int mode) {
        enhance_obj.reset();
        enhance_obj = Enhancement::factory(mode);
    }

    int Detector::enhancementMode() {
        return enhance_obj->mode();
    }

    int Detector::postEnhanceEvent() {
        enhanceFeedback();
        return 1;
    }

    int Detector::enhanceFeedback() {
        if (MODELING_UPDATE_USING_ENHANCED == modelUpdateType()) {
            Mat complementary_frame = (binarized_frame <= 0);
            modelUpdate(complementary_frame);
        }
        return 1;
    }

    int Detector::enhance() {
        if (binarized_frame.empty()) {
            return 0;
        }
        preEnhanceEvent();
        int res;
        timeStats().start(EXPORT_DIR_ENHANCED);
        binarized_frame = enhance_obj->get(pre_enhancement_frame);
        if (binarized_frame.empty()) {
            res = 0;
        }
        timeStats().stop();
        postEnhanceEvent();
        return res;
    }

    int Detector::preEnhanceEvent() {
        setEnhancementFrames();
        scaleEnhancementInputFrame();
        return 1;
    }

    int Detector::maskFramePixels() {
        if (frame2.empty()
            || binarized_frame.empty()) {
            return 0;
        }
        Mat reversed_binarized_frame = (binarized_frame <= 0);
        if (model_obj->frameOffset() == 0) {
            frame3.copyTo(masked_fg_frame, binarized_frame);
            frame3.copyTo(masked_bg_frame, reversed_binarized_frame);
        } else if (model_obj->frameOffset() == 1) {
            frame2.copyTo(masked_fg_frame, binarized_frame);
            frame2.copyTo(masked_bg_frame, reversed_binarized_frame);
        } else {
            frame1.copyTo(masked_fg_frame, binarized_frame);
            frame1.copyTo(masked_bg_frame, reversed_binarized_frame);
        }
        return 1;
    }

    Mat Detector::maskedFrame() {
        return masked_fg_frame;
    }

    Mat Detector::scaleBy(Mat &img, float ratio) {
        Mat frame_scaled;
        resize(img, frame_scaled, Size(), ratio, ratio); //, INTER_LANCZOS4); linear instead
        return frame_scaled;
    }

    Mat Detector::scaleBy(IplImage *img, float ratio) {
        Mat frame_orig(img, true);
        Mat frame_scaled;
        resize(frame_orig, frame_scaled, Size(), ratio, ratio);//, INTER_LANCZOS4); linear instead
        return frame_scaled;
    }

    Mat Detector::scaleAt(Mat &img, Size size) {
        Mat frame_scaled;
        resize(img, frame_scaled, size);
        frame_scaled.copyTo(img);
        return frame_scaled;
    }

    Mat Detector::scale(Mat &img) {
        if (genericFrameSize().width <= 0) {
            if (sourceFrameSize().width <= 0) {
                return img;
            }
            genericFrameSize(sourceFrameSize());
        }
        return scaleAt(img, generic_frame_size);
    }

    Mat Detector::scale(IplImage *img) {
        if (genericFrameSize().width <= 0) {
            if (sourceFrameSize().width <= 0) {
                return img;
            }
            genericFrameSize(sourceFrameSize());
        }
        return scaleAt(img, generic_frame_size);
    }

    Mat Detector::scaleAt(IplImage *img, Size size) {
        Mat frame_orig(img);
        Mat frame_scaled;
        resize(frame_orig, frame_scaled, size);
        return frame_scaled;
    }

    void Detector::sourceFrameSize(Size size) {
        source_frame_size = size;
    }

    void Detector::sourceFrameSize(string size_str) {
        pair<int, int> size_int_pair;
        if (!resolutionStrToIntPair(size_str, size_int_pair)) {
            return;
        }
        return sourceFrameSize(size_int_pair.first, size_int_pair.second);
    }

    void Detector::sourceFrameSize(int cols, int rows) {
        source_frame_size = Size(cols, rows);
    }

    Size Detector::sourceFrameSize() {
        return source_frame_size;
    }

    void Detector::genericFrameSize(Size size) {
        generic_frame_size = size;
    }

    void Detector::genericFrameSize(string size_str) {
        pair<int, int> size_int_pair;
        if (!resolutionStrToIntPair(size_str, size_int_pair)) {
            return;
        }
        return genericFrameSize(size_int_pair.first, size_int_pair.second);
    }

    void Detector::genericFrameSize(int cols, int rows) {
        generic_frame_size = Size(cols, rows);
    }

    Size Detector::genericFrameSize() {
        return generic_frame_size;
    }

    void Detector::modelingFrameSize(Size size) {
        modeling_frame_size = size;
    }

    Size Detector::modelingFrameSize() {
        return modeling_frame_size;
    }

    void Detector::modelingFrameSize(string size_str) {
        pair<int, int> size_int_pair;
        if (!resolutionStrToIntPair(size_str, size_int_pair)) {
            return;
        }
        return modelingFrameSize(size_int_pair.first, size_int_pair.second);
    }

    void Detector::modelingFrameSize(int cols, int rows) {
        modeling_frame_size = Size(cols, rows);
    }

    void Detector::isolationFrameSize(Size size) {
        isolation_frame_size = size;
    }

    void Detector::isolationFrameSize(string size_str) {
        pair<int, int> size_int_pair;
        if (!resolutionStrToIntPair(size_str, size_int_pair)) {
            return;
        }
        return isolationFrameSize(size_int_pair.first, size_int_pair.second);
    }

    void Detector::isolationFrameSize(int cols, int rows) {
        isolation_frame_size = Size(cols, rows);
    }

    Size Detector::isolationFrameSize() {
        return isolation_frame_size;
    }

    void Detector::binarizationFrameSize(Size size) {
        binarization_frame_size = size;
    }

    void Detector::binarizationFrameSize(string size_str) {
        pair<int, int> size_int_pair;
        if (!resolutionStrToIntPair(size_str, size_int_pair)) {
            return;
        }
        return binarizationFrameSize(size_int_pair.first, size_int_pair.second);
    }

    void Detector::binarizationFrameSize(int cols, int rows) {
        binarization_frame_size = Size(cols, rows);
    }

    Size Detector::binarizationFrameSize() {
        return binarization_frame_size;
    }

    void Detector::enhancementFrameSize(Size size) {
        enhancement_frame_size = size;
    }

    void Detector::enhancementFrameSize(string size_str) {
        pair<int, int> size_int_pair;
        if (!resolutionStrToIntPair(size_str, size_int_pair)) {
            return;
        }
        return enhancementFrameSize(size_int_pair.first, size_int_pair.second);
    }

    void Detector::enhancementFrameSize(int cols, int rows) {
        enhancement_frame_size = Size(cols, rows);
    }

    Size Detector::enhancementFrameSize() {
        return enhancement_frame_size;
    }

    void Detector::featureFrameSize(Size size) {
        feature_frame_size = size;
    }

    void Detector::featureFrameSize(string size_str) {
        pair<int, int> size_int_pair;
        if (!resolutionStrToIntPair(size_str, size_int_pair)) {
            return;
        }
        return featureFrameSize(size_int_pair.first, size_int_pair.second);
    }

    void Detector::featureFrameSize(int cols, int rows) {
        feature_frame_size = Size(cols, rows);
    }

    Size Detector::featureFrameSize() {
        return feature_frame_size;
    }

    int Detector::resolutionStrToIntPair(string &size_str, pair<int, int> &size_int_pair) {
        if (size_str == "Generic") {
            Size size = genericFrameSize();
            size_int_pair.first = size.width;
            size_int_pair.second = size.height;
            return 1;
        }
        if (size_str == "Original") {
            Size size = sourceFrameSize();
            size_int_pair.first = size.width;
            size_int_pair.second = size.height;
            return 1;
        }
        size_t found;
        found = size_str.find("x");
        if (string::npos == found) {
            return 0;
        }
        string l_str = size_str.substr(0, found);
        string h_str = size_str.substr(found + 1);
        if (l_str.empty()
            || h_str.empty()) {
            return 0;
        }
        int l_int = atoi(l_str.c_str());
        int h_int = atoi(h_str.c_str());
        if ((l_int <= 0)
            || (h_int <= 0)) {
            return 0;
        }
        size_int_pair.first = l_int;
        size_int_pair.second = h_int;
        return 1;
    }

    TimeStats& Detector::timeStats() {
        return time_stats;
    }

    void Detector::timeStatsLevel(int level) {
        time_stats = TimeStats(level);
    }
}
