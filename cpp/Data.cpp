/***************************************************************
 * Name:      Data.cpp
 * Purpose:   Code for Data Layer Class
 * Author:    Bogdan Alecu
 * Created:   2011-09-15
 * Copyright: Bogdan Alecu
 * License:   GNU GPL
 **************************************************************/

#include "Data.h"

namespace openvpa {

    const string Data::MOVIE_DIR_NAME = "mov";
    const string Data::IMAGE_DIR_NAME = "img";
    const string Data::DEFAULT_CONFIG_FILE = "./data.ini";

    Data::Data() {
        default_init();
    }

    void Data::default_init() {
        configuration = readConf(DEFAULT_CONFIG_FILE);
        debug_level = 1;
        max_processed_frames = 0;
        processed_frames = 0;
        display_time_stats = false;
        display_imgs = false;
        capture = NULL;
        capture_loaded = false;
        debug_stream = NULL;
        override_output_dir = "";
    }

    Data::~Data() {
        if (capture_loaded
            && capture) {
            cvReleaseCapture(&capture);
        }
        if (debug_stream != NULL) {
            fclose(debug_stream);
        }
    }

    int Data::reset() {
        default_init();
        return 1;
    }

    int Data::lsDir(string dir, vector<string> &files) {
        DIR *dp;
        struct dirent *dirp;
        if ((dp  = opendir(dir.c_str())) == NULL) {
            char* tmp = (char*)"";
            sprintf(tmp, "Error(%d) opening %s", errno, dir.c_str());
            string res(tmp);
            raise(3, res);
            return 0;
        }
        while ((dirp = readdir(dp)) != NULL) {
            files.push_back(string(dirp->d_name));
        }
        closedir(dp);
        return 0;
    }

    void Data::debugLevel(int level) {
        debug_level = level;
    }

    void Data::debugFile(string filename) {
        debug_file = filename;
    }

    int Data::debugLevel() {
        return debug_level;
    }

    void Data::raise(int level, string message) {
        if (level < debug_level) {
            return;
        }
        stringstream ss;
        switch (level) {
            case ERROR_LEVEL_NOTIF:
                ss << "Notice: " << message << endl;
                break;
            case ERROR_LEVEL_WARN:
                ss << "Warning: " << message << endl;
                break;
            case ERROR_LEVEL_ERROR:
                ss << "Error: " << message << endl;
                break;
            default:
                ss << "Unhandled error level: " << message << endl;
        }
        if (debug_file == "") {
            cout << ss.str();
            return;
        }
        if (!debugFileOpenIfNot()) {
            return;
        }
        fputs(ss.str().c_str(), debug_stream);
        fflush(debug_stream);
    }

    int Data::debugFileOpenIfNot() {
        if (debug_stream != NULL) {
            return 1;
        }
        createPath(outputDir());
        string filename;
        filename = outputDir() + string("/") + debug_file;
        const char* f = filename.c_str();
        debug_stream = fopen(f, "w+");
        if (debug_stream == NULL) {
            return 0;
        }
        return 2;
    }

    int Data::exportImg(Mat frame, string subpath, string feature, unsigned int index, string msg) {
        if (frame.empty()) {
            raise(ERROR_LEVEL_WARN, "exportImg() : Empty Mat object");
            return 0;
        }
        string path;
        path = outputDir(subpath, detection_method, detection_version) + "/" + feature; //ss.str();
        if (path == "") {
            raise(ERROR_LEVEL_WARN, "exportImg() : No path specified");
            return 0;
        }
        if (createPath(path)) {
            raise(ERROR_LEVEL_NOTIF, "exportImg() : Created path");
        }
        char buff[12];
        sprintf(buff, "/%04d.jpg", index);
        path.append(buff);
        raise(ERROR_LEVEL_NOTIF, string("exported ") + path + string(" ") + msg);
        imwrite(path, frame);
        return 1;
    }

    int Data::exportImg(Mat frame, string feature, unsigned int index, string msg) {
        return exportImg(frame, output_dir, feature, index, msg);
    }

    int Data::exportImg(Mat frame, string feature, string msg) {
        return exportImg(frame, output_dir, feature, processed_frames, msg);
    }

    string Data::outputDir(string subpath, int method, int version) {
        if (overrideOutputDir().empty()) {
            char method_c[11];
            sprintf(method_c, "met%02d/ver%02d", method, version);
            stringstream ss;
            ss << subpath << "/" << method_c;
            return ss.str();
        }
        return overrideOutputDir();
    }

    string Data::outputDir(string subpath, int method, string version) {
        if (overrideOutputDir().empty()) {
            char method_c[40];
            sprintf(method_c, "met%02d/ver%s", method, version.c_str());
            stringstream ss;
            ss << subpath << "/" << method_c;
            return ss.str();
        }
        return overrideOutputDir();
    }

    string Data::outputDir() {
        return outputDir(output_dir, detection_method, detection_version);
    }

    void Data::overrideOutputDir(string dir) {
        override_output_dir = dir;
    }

    string Data::overrideOutputDir() {
        return override_output_dir;
    }

    int Data::createPath(string path_str) {
        if (boost::filesystem::is_directory(path_str)) {
            return 0;
        }
        boost::filesystem::path p(path_str);
        boost::filesystem::create_directories(p);
        return 1;
    }

    int Data::pathExists(string path_str) {
        return boost::filesystem::is_directory(path_str) ? 1 : 0;
    }

    int Data::fileExists(string file_str) {
        return boost::filesystem::is_regular_file(file_str);
    }

    int Data::deletePath(string path_str) {
        if (!boost::filesystem::is_directory(path_str)) {
            return 0;
        }
        boost::filesystem::path p (path_str);
        boost::filesystem::remove_all(p);
        return 1;
    }

    int Data::splitPathFile(string &file, string &path, string &filename) {
        string slash("/");
        size_t index = file.rfind(slash);
        if (string::npos == index) {
            return 0;
        }
        path = file.substr(0, index);
        filename = file.substr(index + slash.length());
        return 1;
    }

    int Data::writeConf(string file, map<string, string> data_map) {
        ofstream config(file.c_str());
        if (!config) {
            return 0;
        }
        for (map<string, string>::iterator it = data_map.begin();
            it != data_map.end();
            it ++) {
            config << (*it).first << " = " << (*it).second << endl;
        }
        config.close();
        return 1;
    }

    map<string, string> Data::readConf(string file) {
        ifstream config(file.c_str()); //, ifstream::in);
        map<string, string> params;
        if (!config) {
            return params;
        }
        set<string> options;
        options.insert("*");
        try {
            for (boost::program_options::detail::config_file_iterator i(config, options), e ;
                i != e;
                i ++) {
                params[i->string_key] = i->value[0];
            }
        } catch (exception& e) {
            stringstream ss;
            ss << "Exception: " << e.what();
            raise(ERROR_LEVEL_ERROR, ss.str());
        }
        config.close();
        return params;
    }

    void Data::displayImg(Mat frame) {
        cvNamedWindow("result", CV_WINDOW_AUTOSIZE);
        imshow("result", frame);
        waitKey(0);
        cvDestroyWindow("result");
    }

    bool Data::multiplexImgs(Mat& frame1, Mat& frame2, Mat& mask, Mat& result) {
        if (!((frame1.size() == frame2.size())
            && (frame1.type() == frame2.type()))) {
            return false;
        }
        result = Mat(frame1.size(), frame1.type());
        Mat_<Vec3b>::iterator it1 = frame1.begin<Vec3b>();
        Mat_<Vec3b>::iterator it2 = frame2.begin<Vec3b>();
        Mat_< Vec<uchar, 1> >::iterator itm = mask.begin< Vec<uchar, 1> >();
        Mat_<Vec3b>::iterator itr = result.begin<Vec3b>();
        Mat_<Vec3b>::iterator it1_end = frame1.end<Vec3b>();
        for (;
            (it1 != it1_end);
            it1 ++, it2 ++, itm ++, itr ++) {
            if ((*itm)[0] <= 0) {
                *itr = *it1;
            } else {
                *itr = *it2;
            }
        }
        return true;
    }

    void Data::setDetMethod(int method) {
        detection_method = method;
    }

    void Data::setDetVersion(int version) {
        stringstream ss;
        ss << version;
        detection_version = ss.str();
    }

    void Data::setDetVersion(string version) {
        detection_version = version;
    }

    int Data::getDetectionMethod() {
        return detection_method;
    }

    string Data::getDetectionVersion() {
        return detection_version;
    }

    bool Data::shouldDisplayTimeStats() {
        return display_time_stats;
    }

    void Data::setDisplayTimeStats(bool value) {
        display_time_stats = value;
    }

    int Data::addTimeProbe(int statsLevel) {
        clock_t now;
        now = clock();
        if (-1 == now) {
            return 0;
        }
        bool shouldNL = false;
        time_probes.push_back(now);
        if (TIME_STATS_LEVEL_ZERO <= statsLevel) {
            // do nothing
        }
        if (TIME_STATS_LEVEL_ONE <= statsLevel) {
            printf("time probe added at %ld (clocks, where %ld are executed each second) or %.6lf (seconds)", now, CLOCKS_PER_SEC, (double) (now / CLOCKS_PER_SEC));
            shouldNL = true;
        }
        if (TIME_STATS_LEVEL_TWO <= statsLevel) {
            clock_t probe;
            probe = (1 >= time_probes.size()) ?
                now :
                time_probes.at(time_probes.size() - 2);
            printf(", last probe was at %ld, %.6lf seconds elapsed since then", probe, (double) (((double)now - (double)probe) / CLOCKS_PER_SEC));
        }
        if (TIME_STATS_LEVEL_THREE <= statsLevel) {
            // to be done
        }
        if (shouldNL) {
            printf("\n");
        }
        return 1;
    }

    int Data::addTimeProbe() {
        return addTimeProbe(TIME_STATS_LEVEL_ZERO);
    }

    int Data::resetTimeProbes() {
        time_probes.clear();
        return 1;
    }

    void Data::maxProcessedFrames(unsigned int value) {
        max_processed_frames = value;
    }

    unsigned int Data::maxProcessedFrames() {
        return max_processed_frames;
    }

    unsigned int Data::processedFrames() {
        return processed_frames;
    }

    string Data::maxProcessedFramesStr() {
        stringstream ss;
        if (0 == maxProcessedFrames()) {
            ss << "All";
        } else {
            ss << maxProcessedFrames();
        }
        return ss.str();
    }

    void Data::setDisplayImgs(bool value) {
        display_imgs = value;
    }

    bool Data::shouldDisplayImgs() {
        return display_imgs;
    }

    int Data::getCapture() {
        if (!capture_loaded) {
            capture = cvCaptureFromFile(sourceAviFile().c_str());
            if (!capture) {
                return 0;
            }
            capture_loaded = true;
            return 2;
        }
        return 1;
    }

    IplImage* Data::nextFrameIpl() {
        int gc_res = getCapture();
        if (0 == gc_res) {
            raise(ERROR_LEVEL_ERROR, "Data::nextFrame(): No data source");
            return NULL;
        }
        if (1 == gc_res) {
            processed_frames ++;
        }
        if ((maxProcessedFrames() > 0)
            && (processed_frames >= maxProcessedFrames())) {
            raise(ERROR_LEVEL_NOTIF, "Data::nextFrame(): Reached the maxProcessedFrames() limit");
            return NULL;
        }
        IplImage* frame = cvQueryFrame(capture);
        if (!frame) {
            raise(ERROR_LEVEL_WARN, "Data::nextFrame(): Could not query for frames");
            return NULL;
        }
        return frame;
    }

    Mat Data::nextFrame() {
        IplImage* frame_ipl = nextFrameIpl();
        Mat frame_mat(frame_ipl);
        return frame_mat;
    }

    string Data::sourceAviFile() {
        return input_avi_file;
    }

    int Data::clearOutputDir() {
        return deletePath(outputDir());
    }

    vector<unsigned int> Data::availableSources() {
        vector<unsigned int> res;
        for (unsigned int i = 0; i < /*configuration.size()*/100; i ++) {
            stringstream ss;
            ss << "src" << i;
            if (configuration[ss.str().c_str()].empty()) {
                continue;
            }
            res.push_back(i);
        }
        return res;
    }

    void Data::setSource(unsigned int src) {
        stringstream ss;
        ss << "src" << src;
        if (configuration[ss.str().c_str()].empty()) {
            // defaults
            input_avi_file.assign(configuration["src1"]);
            output_dir.assign(configuration["export_path"]);
            return;
        }
        input_avi_file.assign(configuration[ss.str().c_str()]);
        output_dir.assign(configuration["export_path"]);
    }
}
