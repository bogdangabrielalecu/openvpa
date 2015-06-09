/***************************************************************
 * Name:      FeaturesBasic.cpp
 * Purpose:   Code for Features Class Extension
 * Author:    Bogdan Alecu
 * Created:   2011-09-17
 * Copyright: Bogdan Alecu
 * License:   GNU GPL
 **************************************************************/

#include "features/FeaturesBasic.h"
#include "Data.h"

namespace openvpa {

    FeaturesBasic::FeaturesBasic() : Features::Features() {
        reset();
        classification_attrs = rgbHistFragments();
        classification_attrs.push_back("x_center");
        classification_attrs.push_back("y_center");
        classification_attrs.push_back("width");
        classification_attrs.push_back("height");
        classification_attrs.push_back("area");
    }

    FeaturesBasic::~FeaturesBasic() {
        //
    }

    bool FeaturesBasic::reset() {
        step = pow(2, (8 - HIST_COMPONENT_CHUNK_NUMBER_POWER));
        chunks = pow(2, HIST_COMPONENT_CHUNK_NUMBER_POWER);
        mv = FrameObjectsVector();
        queue_index = 0;
        class_index = 0;
        resetObjCounter();
        resetRGBHist();
        double_params["min_area"] = 20;
        return true;
    }

    bool FeaturesBasic::extract(Mat& frm, Mat& crt_frm) {
        resetObjects();
        obj.frm = Mat::zeros(frm.size(), CV_8UC3);
        cout << "exploring new frame\n";
        for (int y = 0; y < frm.rows; y ++) {
            for (int x = 0; x < frm.cols; x ++) {
                if ((int) frm.at<Vec1b>(y, x)[0] <= 0) {
                    obj.frm.at<Vec3b>(y, x)[0] = (uchar) 1;
                    obj.frm.at<Vec3b>(y, x)[1] = (uchar) 1;
                    obj.frm.at<Vec3b>(y, x)[2] = (uchar) 1;
                    continue;
                }
                if ((obj.frm.at<Vec3b>(y, x)[0] != 0)
                    || (obj.frm.at<Vec3b>(y, x)[1] != 0)
                    || (obj.frm.at<Vec3b>(y, x)[2] != 0)) {
                    continue;
                }
                newFoundObject(frm, crt_frm, x, y);
            }
        }
        if (!obj.map.empty()) {
            classifyFrameObjects();
            addFrameObjectsToQueue();
            drawFrameObjectsInformation();
        }
        return true;
    }

    void FeaturesBasic::drawFrameObjectsInformation() {
        for (ObjAttrMap::iterator m_it = obj.map.begin(); m_it != obj.map.end(); m_it ++) {
            cout << objStatsString((*m_it).second);
            string s = ((*m_it).second.attrs["type"] == 1) ?
                "Human" :
                ((*m_it).second.attrs["type"] == 2) ?
                    "Car" :
                    "Unkown";
            stringstream ss;
            ss << "#" << (*m_it).second.classid << " " << s;
            putText(
                obj.frm,
                ss.str(),
                Point((int)(*m_it).second.attrs["x_max"], (int)(*m_it).second.attrs["y_max"]),
                FONT_HERSHEY_SIMPLEX,
                0.4,
                CV_RGB((*m_it).second.seg_col[0], (*m_it).second.seg_col[1], (*m_it).second.seg_col[2])
            );
        }
    }

    void FeaturesBasic::newFoundObject(Mat& frm, Mat& crt_frm, int x, int y) {
        ObjAttr new_attr = newObjAttr();
        resetRGBHist();
        exploreVicinity(frm, crt_frm, x, y, new_attr);
        if (!isValidObject(new_attr)) {
            obj.map.erase(new_attr.id);
//            ignoreObject(new_attr);
            return;
        }
        finishStats(new_attr);
    }

    bool FeaturesBasic::isValidObject(ObjAttr& attr) {
        return ((double_params["min_area"] > 0)
            && (obj.map[attr.id].attrs["area"] > double_params["min_area"]));
    }

    string FeaturesBasic::objStatsString(ObjAttr attr) {
        stringstream ss;
        ss << "Obj #" << attr.id << ", classid=" << attr.classid << " stats: center=(" << obj.map[attr.id].attrs["x_center"] << ", " << obj.map[attr.id].attrs["y_center"] << ")";
        ss << ", S=" << obj.map[attr.id].attrs["area"] << ", w=" << obj.map[attr.id].attrs["width"]  << ", h=" << obj.map[attr.id].attrs["height"] << ", ";
        vector<string> keys = rgbHistFragments();
        if (obj.map[attr.id].attrs["ignore"] == 1) {
            ss << " Ignored! too small object...";
        } else {
            ss << "hist_elements=(";
            for (vector<string>::iterator it = keys.begin(); it < keys.end(); it ++) {
                ss << obj.map[attr.id].attrs[*it] << ", ";
            }
            ss  << "); ";
        }
        ss << endl;
        return ss.str();
    }

    FrameObjects& FeaturesBasic::getFrameObjects() {
        return obj;
    }

    Mat FeaturesBasic::getObjectsFrame() {
        return obj.frm;
    }

    bool FeaturesBasic::exploreVicinity(Mat& frm, Mat& crt_frm, int x, int y, ObjAttr& new_attr) {
        if ((x < 0)
            || (y < 0)
            || (x >= frm.cols)
            || (y >= frm.rows)) {
            return false; // beyond matrix limits
        }
        if ((int) frm.at<Vec1b>(y, x)[0] <= 0) {
            return false; // outside current object
        }
        if ((obj.frm.at<Vec3b>(y, x)[0] != 0)
            || (obj.frm.at<Vec3b>(y, x)[1] != 0)
            || (obj.frm.at<Vec3b>(y, x)[2] != 0)) {
            return false; // done already
        }
        markPixel(frm, crt_frm, x, y, new_attr);
        exploreVicinity(frm, crt_frm, x, y - 1, new_attr); // top
        exploreVicinity(frm, crt_frm, x + 1, y, new_attr); // right
        exploreVicinity(frm, crt_frm, x, y + 1, new_attr); // bottom
        exploreVicinity(frm, crt_frm, x - 1, y, new_attr); // left
        return true;
    }

    void FeaturesBasic::markPixel(Mat& frm, Mat& crt_frm, int x, int y, ObjAttr& new_attr) {
        obj.frm.at<Vec3b>(y, x) = new_attr.seg_col;

        if ((obj.map.size() < new_attr.id)) {
            new_attr.attrs["x_min"] = x;
            new_attr.attrs["x_max"] = x;
            new_attr.attrs["y_min"] = y;
            new_attr.attrs["y_max"] = y;
            new_attr.attrs["x_center"] = x;
            new_attr.attrs["y_center"] = y;
            new_attr.attrs["width"] = 0;
            new_attr.attrs["height"] = 0;
            new_attr.attrs["area"] = 0;
            vector<string> keys = rgbHistFragments();
            for (vector<string>::iterator it = keys.begin(); it < keys.end(); it ++) {
                new_attr.attrs[*it] = 0;
            }
            obj.map[new_attr.id] = new_attr;
        }
        if (obj.map[new_attr.id].attrs["x_min"] > x) {
            obj.map[new_attr.id].attrs["x_min"] = x;
        }
        if (obj.map[new_attr.id].attrs["x_max"] < x) {
            obj.map[new_attr.id].attrs["x_max"] = x;
        }
        if (obj.map[new_attr.id].attrs["y_min"] > y) {
            obj.map[new_attr.id].attrs["y_min"] = y;
        }
        if (obj.map[new_attr.id].attrs["y_max"] < y) {
            obj.map[new_attr.id].attrs["y_max"] = y;
        }
        obj.map[new_attr.id].attrs["area"] ++;
        r[(int) crt_frm.at<Vec3b>(y, x)[0]] ++;
        g[(int) crt_frm.at<Vec3b>(y, x)[1]] ++;
        b[(int) crt_frm.at<Vec3b>(y, x)[2]] ++;
    }

    vector<string> FeaturesBasic::rgbHistFragments() {
        vector<string> res, colors;
        colors.push_back("red");
        colors.push_back("green");
        colors.push_back("blue");
        for (vector<string>::iterator it = colors.begin(); it < colors.end(); it ++) {
            for (unsigned int i = 0; i < chunks; i ++) {
                stringstream ss;
                ss << *it << "_" << i;
                res.push_back(ss.str());
            }
        }
        return res;
    }

    string FeaturesBasic::histIntervalKey(string color, int value) {
        int min = 0;
        int max;
        for (unsigned int i = 0; i < chunks; i ++) {
            max = min + (int) step;
            if ((value >= min) && (value < max)) {
                stringstream ss;
                ss << color << "_" << i;
                return ss.str();
            }
            min = max;
        }
        return "missed";
    }

    void FeaturesBasic::finishStats(ObjAttr& new_attr) {
        obj.map[new_attr.id].attrs["width"] = obj.map[new_attr.id].attrs["x_max"] - obj.map[new_attr.id].attrs["x_min"];
        obj.map[new_attr.id].attrs["height"] = obj.map[new_attr.id].attrs["y_max"] - obj.map[new_attr.id].attrs["y_min"];
        obj.map[new_attr.id].attrs["x_center"] = (int) (obj.map[new_attr.id].attrs["width"] / 2) + obj.map[new_attr.id].attrs["x_min"];
        obj.map[new_attr.id].attrs["y_center"] = (int) (obj.map[new_attr.id].attrs["height"] / 2) + obj.map[new_attr.id].attrs["y_min"];
        histSplit(new_attr, "red", r);
        histSplit(new_attr, "green", g);
        histSplit(new_attr, "blue", b);
        assignTypeClass(obj.map[new_attr.id]);
    }

    bool FeaturesBasic::assignClass(unsigned int classid, ObjAttr& obj_attr) {
        obj_attr.classid = classid;
        return true;
    }

    bool FeaturesBasic::assignTypeClass(ObjAttr& obj_attr) {
        if (possiblyHuman(obj_attr)) {
            obj_attr.attrs["type"] = 1;
            return true;
        }
        if (possiblyCar(obj_attr)) {
            obj_attr.attrs["type"] = 2;
            return true;
        }
        obj_attr.attrs["type"] = 0;
        return false;
    }

    bool FeaturesBasic::possiblyHuman(ObjAttr& obj_attr) {
        double r = obj_attr.attrs["height"] / obj_attr.attrs["width"];
        return ((1.2 < r) && (r < 7));
    }

    bool FeaturesBasic::possiblyCar(ObjAttr& obj_attr) {
        double r = obj_attr.attrs["width"] / obj_attr.attrs["height"];
        return ((1.2 < r) && (r < 4));
    }

    bool FeaturesBasic::classifyFrameObjects() {
        if (mv.empty()) {
            for (ObjAttrMap::iterator m_it = obj.map.begin(); m_it != obj.map.end(); m_it ++) {
                assignClass(newClass(), (*m_it).second);
            }
            return true;
        }
        UintUintDoubleMap d;
        unsigned int it = queue_index;
        for (unsigned int i = 0; (i < mv.size()) && (i < LOOKBACK_BUFFER_LENGTH); i ++) {
            it = -- it % BUFFER_LENGTH;
            for (ObjAttrMap::iterator mv_it = mv[it].map.begin(); mv_it != mv[it].map.end(); mv_it ++) {
                for (ObjAttrMap::iterator m_it = obj.map.begin(); m_it != obj.map.end(); m_it ++) {
                    d[(*mv_it).second.classid][(*m_it).second.id] += classificationDistance((*mv_it).second, (*m_it).second) / (LOOKBACK_BUFFER_LENGTH - i);
                }
            }
        }
        UintUintMap m = extractClass(d);
        for (UintUintMap::iterator c_it = m.begin(); c_it != m.end(); c_it ++) {
            assignClass((*c_it).second, obj.map[(*c_it).first]);
        }
        for (ObjAttrMap::iterator m_it = obj.map.begin(); m_it != obj.map.end(); m_it ++) {
            if ((*m_it).second.classid <= 0) {
                assignClass(newClass(), (*m_it).second);
            }
        }
        return true;
    }

    double FeaturesBasic::classificationDistance(ObjAttr& obj1, ObjAttr& obj2) {
        double sum = 0;
        for (vector<string>::iterator it = classification_attrs.begin(); it < classification_attrs.end(); it ++) {
            sum += pow((obj1.attrs[*it] - obj2.attrs[*it]), 2);
        }
        return sqrt(sum);
    }

    UintUintMap FeaturesBasic::extractClass(UintUintDoubleMap& d) {
        UintUintMap m;
        unsigned int i,j;
        double min;
        while (m.size() < (*d.begin()).second.size()) {
            min  = 99999999;
            for (UintUintDoubleMap::iterator mv_it = d.begin(); mv_it != d.end(); mv_it ++) {
                for (UintDoubleMap::iterator m_it = (*mv_it).second.begin(); m_it != (*mv_it).second.end(); m_it ++) {
                    if (m[(*m_it).first] > 0) {
                        continue;
                    }
                    if (min > (*m_it).second) {
                        min = (*m_it).second;
                        i = (*mv_it).first;
                        j = (*m_it).first;
                    }
                }
                m[j] = i;
            }
        }
        return m;
    }

    void FeaturesBasic::addFrameObjectsToQueue() {
        queue_index = ++ queue_index % BUFFER_LENGTH;
        if (mv.size() < BUFFER_LENGTH) {
            mv.push_back(obj);
            return;
        }
        mv[queue_index] = obj;
    }

    void FeaturesBasic::histSplit(ObjAttr& attr, string color, map<int, int>& m) {
        for (map<int, int>::iterator it = m.begin(); it != m.end(); it ++) {
            stringstream ss;
            ss << color<< "_" << (int) floor((*it).first / step);
            obj.map[attr.id].attrs[ss.str()] += 100 * ((*it).second / obj.map[attr.id].attrs["area"]);
        }
    }

    void FeaturesBasic::resetRGBHist() {
        r.clear();
        g.clear();
        b.clear();
    }

    unsigned int FeaturesBasic::newClass() {
        class_index = ++ class_index % CLASS_BUFFER_LENGTH;
        return class_index;
    }
}
