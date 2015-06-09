/***************************************************************
 * Name:      Method.cpp
 * Purpose:   Code for Method Base Class
 * Author:    Bogdan Alecu
 * Created:   2011-09-15
 * Copyright: Bogdan Alecu
 * License:   GNU GPL
 **************************************************************/

#include "Method.h"

namespace openvpa {

    Method::Method() {
        default_init();
    }

    Method::Method(int src) {
        default_init();
        data().setSource(src);
    }

    void Method::default_init() {
        //todo
    }

    Method::~Method() {
        //todo
    }

    void Method::data(Data &data) {
        data_obj = data;
    }

    Data& Method::data() {
        return data_obj;
    }

    void Method::detector(Detector &detector) {
        detector_obj = detector;
    }

    Detector& Method::detector() {
        return detector_obj;
    }

    int Method::execute() {
        prepare();
        IplImage* frame = NULL;
        log();

        while (NULL != (frame = data().nextFrameIpl())) {
            if (!process(frame)) {
                data().raise(Data::ERROR_LEVEL_NOTIF, "Method::execute() : requested exit by inharited_class::process()");
                break;
            }
            if (detector().sourceFrameSize().width <= 0) {
                detector().sourceFrameSize(frame->width, frame->height);
            }
        }
        cleanup();
        return 1;
    }

    int Method::reset() {
        data().reset();
        detector().reset();
        return 1;
    }

    int Method::log(int logLevel) {
        if (logLevel <= 0) {
            return logLevel;
        }
        data().createPath(data().outputDir());
        ofstream fs;
        stringstream ss;
        ss << data().outputDir() << "/export_details.txt";
        fs.open(ss.str().c_str());
        fs << "MethodName" << logMethodName() << endl;
        if (logLevel <= 1) {
            return logLevel;
        }
        fs << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" << endl;
        fs << "MethodDesc" << logMethodDesc() << endl;
        if (logLevel <= 2) {
            return logLevel;
        }
        fs << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" << endl;
        fs << "MethodParams" << "" << endl;
        logMethodParams(fs);
        return logLevel;
    }

    int Method::logMethodGenericParams(ofstream &fs) {
        fs << "Export path" << data().outputDir() << endl;
        fs << "Method id" << data().getDetectionMethod() << endl;
        fs << "Version" << data().getDetectionVersion() << endl;
        fs << "Frame size" << detector().genericFrameSize().width << "x" << detector().genericFrameSize().height << endl;
        fs << "Processed frames" << data().maxProcessedFramesStr() << endl;
        return 1;
    }
}
