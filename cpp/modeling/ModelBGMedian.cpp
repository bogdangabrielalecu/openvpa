/***************************************************************
 * Name:      ModelBGMedian.cpp
 * Purpose:   Code for ModelBG Class Extension
 * Author:    Bogdan Alecu
 * Created:   2011-09-15
 * Copyright: Bogdan Alecu
 * License:   GNU GPL
 **************************************************************/

#include "modeling/ModelBGMedian.h"
#include "Data.h"

namespace openvpa {

    ModelBGMedian::ModelBGMedian() : ModelBG::ModelBG() {
        reset();
    }

    ModelBGMedian::~ModelBGMedian() {
        //
    }

    bool ModelBGMedian::reset() {
        frames_median_list = VecPixelVec();
        previous_bg = Mat();
        bg_size = Size();
        return ModelBG::reset();
    }

    Mat& ModelBGMedian::get() {
        if (!updated) {
            return bg;
        }
        bg.copyTo(previous_bg);
        extractFromMedianList();
        return bg;
    }

    Mat& ModelBGMedian::extractFromMedianList() {
        if (bg.empty()) {
            bg.create(bg_size, CV_8UC3);
        }
        VecPixelVec::iterator im_it = frames_median_list.begin();
        VecPixelVec::iterator im_it_end = frames_median_list.end();
        Mat_<Vec3b>::iterator frm_it = bg.begin<Vec3b>();
        Mat_<Vec3b>::iterator frm_it_end = bg.end<Vec3b>();
        bVec::iterator it;
        int probe_index = (int)((*im_it)[0].size() / 2);

        for (;
            ((im_it != im_it_end) && (frm_it != frm_it_end));
            ++ im_it, ++ frm_it) {
            for (int c = 0; c < 3; c ++) {
                (*frm_it)[c] = (*im_it)[c].at(probe_index);
            }
        }
        updated = false;
        return bg;
    }

    bool ModelBGMedian::update(Mat& frame, Mat& mask) {
        if (previous_bg.empty()) { // the first frame in median list is unmasked
            return update(frame);
        }
        Mat median_updater;
        Data::multiplexImgs(frame, previous_bg, mask, median_updater);
        return update(median_updater);
    }

    bool ModelBGMedian::update(Mat& frame) {
        if (medianListEmpty()) {
            initMedianList(frame);
            bg_size = frame.size();
        }
        VecPixelVec::iterator im_it = frames_median_list.begin();
        VecPixelVec::iterator im_it_end = frames_median_list.end();
        Mat_<Vec3b>::iterator frm_it = frame.begin<Vec3b>();
        Mat_<Vec3b>::iterator frm_it_end = frame.end<Vec3b>();
        bool inserted = false;
        bVec::iterator it;

        for (;
            ((im_it != im_it_end) && (frm_it != frm_it_end));
            im_it ++, frm_it ++) {
            for (int c = 0; c < 3; c ++) {
                if ((*im_it)[c].empty()) {
                    (*im_it)[c].push_back((*frm_it)[c]);
                    continue;
                }
                inserted = false;
                for (it = (*im_it)[c].begin(); it < (*im_it)[c].end(); it ++) {
                    if ((*frm_it)[c] <= *it) {
                        (*im_it)[c].insert(it, (*frm_it)[c]);
                        inserted = true;
                        break;
                    }
                }
                if (!inserted) {
                    (*im_it)[c].push_back((*frm_it)[c]);
                }
            }
        }
        updated = true;
        return true;
    }

    int ModelBGMedian::initMedianList(Mat frame) {
        Size size = frame.size();
        frames_median_list = VecPixelVec();
        for (int y = 0; y < frame.rows; y ++) {
            for (int x = 0; x < frame.cols; x ++) {
                PixelVec pixel;
                for (int c = 0; c < 3; c ++) {
                    pixel.push_back(bVec());
                }
                frames_median_list.push_back(pixel);
            }
        }
        return 1;
    }

    bool ModelBGMedian::medianListEmpty() {
        if (frames_median_list.empty()) {
            return true;
        }
        VecPixelVec::iterator im_it = frames_median_list.begin();
        return ((*im_it)[0].size() == 0);
    }
}
