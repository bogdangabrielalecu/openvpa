/***************************************************************
 * Name:      Data.h
 * Purpose:   Code for Data Layer Class Header
 * Author:    Bogdan Alecu
 * Created:   2011-09-15
 * Copyright: Bogdan Alecu
 * License:   GNU GPL
 **************************************************************/

#ifndef OPENVPADATA_H
#define OPENVPADATA_H

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <set>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <exception>
#include <stdio.h>
#include <cv.h>
#include <highgui.h>
//#include <boost/asio.hpp> // bogdan, 2011.09.18, building env for win
#include <boost/filesystem.hpp>
#include <boost/config.hpp>
#include <boost/program_options/detail/config_file.hpp>
#include <boost/program_options/parsers.hpp>

using namespace cv;
using namespace std;

namespace openvpa {

    class Data {

        private:
            void default_init();
            vector<clock_t> time_probes;
            unsigned int max_processed_frames;
            unsigned int processed_frames;
            FILE *debug_stream;

        protected:
            int debug_level;
            string debug_file;
            string input_avi_file;
            string output_dir;
            int detection_method;
            string detection_version;
            bool display_time_stats;
            bool display_imgs;
            CvCapture *capture;
            bool capture_loaded;
            string override_output_dir;

        public:
            Data();
            ~Data();
            static const string DEFAULT_DIR;
            static const string IMAGE_DIR_NAME;
            static const string MOVIE_DIR_NAME;
            static const string DEFAULT_CONFIG_FILE;
            static const int ERROR_LEVEL_NONE = 0;
            static const int ERROR_LEVEL_NOTIF = 1;
            static const int ERROR_LEVEL_WARN = 2;
            static const int ERROR_LEVEL_ERROR = 3;
            static const int TIME_STATS_LEVEL_ZERO = 0;
            static const int TIME_STATS_LEVEL_ONE = 1;
            static const int TIME_STATS_LEVEL_TWO = 2;
            static const int TIME_STATS_LEVEL_THREE = 3;
            static const int DEFAULT_MAX_PROCESSED_FRAMES = 1000;
            map<string, string> configuration;
            int reset();
            void debugLevel(int level);
            int debugLevel();
            void debugFile(string filename);
            int debugFileOpenIfNot();
            void raise(int level, string message);
            int lsDir(string dir, vector<string> &files);
            int exportImg(Mat frame, string subpath, string feature, unsigned int index, string msg = "");
            int exportImg(Mat frame, string feature, unsigned int index, string msg = "");
            int exportImg(Mat frame, string feature, string msg = "");
            static void displayImg(Mat frame);
            static bool multiplexImgs(Mat& frame1, Mat& frame2, Mat& mask, Mat& result);
            vector<unsigned int> availableSources();
            void setSource(unsigned int src);
            int clearOutputDir();
            string sourceAviFile();
            void setDetMethod(int method);
            int getDetectionMethod();
            void setDetVersion(int version);
            void setDetVersion(string version);
            string getDetectionVersion();
            int createPath(string path);
            int pathExists(string path_str);
            int fileExists(string file_str);
            int deletePath(string path);
            int splitPathFile(string &file, string &path, string &filename);
            map<string, string> readConf(string file);
            int writeConf(string file, map<string, string> data_map);
            void setDisplayTimeStats(bool value = true);
            bool shouldDisplayTimeStats();
            void setDisplayImgs(bool value = true);
            bool shouldDisplayImgs();
            int addTimeProbe();
            int addTimeProbe(int statsLevel);
            int resetTimeProbes();
            void maxProcessedFrames(unsigned int value);
            unsigned int maxProcessedFrames();
            unsigned int processedFrames();
            string maxProcessedFramesStr();
            int getCapture();
            IplImage* nextFrameIpl();
            Mat nextFrame();
            string outputDir(string subpath, int method, int version);
            string outputDir(string subpath, int method, string version);
            string outputDir();
            string overrideOutputDir();
            void overrideOutputDir(string dir);
    };
}
#endif
