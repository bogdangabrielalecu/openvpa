/***************************************************************
 * Name:      TimeStats.cpp
 * Purpose:   Code for Time Measurement and Statistics Class
 * Author:    Bogdan Alecu
 * Created:   2011-09-16
 * Copyright: Bogdan Alecu
 * License:   GNU GPL
 **************************************************************/

#include "TimeStats.h"

namespace openvpa {

    TimeStats::TimeStats() {
        default_init();
    }

    TimeStats::TimeStats(int value) {
        default_init();
        mode = value;
    }

    void TimeStats::default_init() {
        mode = MODE_FULL;
    }

    TimeStats::~TimeStats() {
        //
    }

    clock_t TimeStats::start(string key) {
        if (mode == MODE_NONE) {
            return -2;
        }
        if (m[key].start > 0) {
            return -1;
        }
        m[key].start = clock();
        lastKey(key);
        return m[key].start;
    }

    clock_t TimeStats::restart(string key) {
        if (mode == MODE_NONE) {
            return -2;
        }
        m[key].start = clock();
        lastKey(key);
        return m[key].start;
    }

    void TimeStats::lastKey(string key) {
        last_key = key;
    }

    string TimeStats::lastKey() {
        return last_key;
    }

    double TimeStats::enqueue(string key, double diff) {
        if (mode == MODE_NONE) {
            return -2;
        }
        if (mode > MODE_SIMPLE) {
            m[key].probes.push_back(diff);
        }
        m[key].stats.count ++;
        if (m[key].stats.count == 1) {
            m[key].stats.max = diff;
            m[key].stats.min = diff;
            m[key].stats.avg = diff;
            return diff;
        }
        if (m[key].stats.max < diff) {
            m[key].stats.max = diff;
        }
        if (m[key].stats.min > diff) {
            m[key].stats.min = diff;
        }
        m[key].stats.avg = (double)(
                ((long double) (m[key].stats.avg * (m[key].stats.count - 1) / m[key].stats.count))
                + ((long double) (diff / m[key].stats.count))
            );
        return diff;
    }

    double TimeStats::stop(string key) {
        if (mode == MODE_NONE) {
            return -2;
        }
        if (m[key].start == 0) {
            return -1;
        }
        if (m[key].start < 0) {
            return 0;
        }
        double diff = ((double)(clock() - m[key].start)) / CLOCKS_PER_SEC;
        // double diff = difftime(clock(), m[key].start);
        m[key].start = -1;
        return enqueue(key, diff);
    }

    double TimeStats::stop() {
        return stop(lastKey());
    }

    double TimeStats::diff(string key) {
        if (mode == MODE_NONE) {
            return -2;
        }
        if (m[key].start == 0) {
            return -1;
        }
        if (m[key].probes.empty()) {
            return -2;
        }
        return m[key].probes.back();
    }

    double TimeStats::diff() {
        return diff(lastKey());
    }

    double TimeStats::max(string key) {
        if (mode == MODE_NONE) {
            return 2.22507e-308; // abs min value for double
        }
        if (m[key].start == 0) {
            return 2.22507e-308; // abs min value for double
        }
        return m[key].stats.max;
    }

    double TimeStats::min(string key) {
        if (mode == MODE_NONE) {
            return 1.79769e+308; // abs max value for double
        }
        if (m[key].start == 0) {
            return 1.79769e+308; // abs max value for double
        }
        return m[key].stats.min;
    }

    double TimeStats::avg(string key) {
        if (mode == MODE_NONE) {
            return 2.22507e-308; // abs min value for double
        }
        if (m[key].start == 0) {
            return 2.22507e-308; // abs avg value for double
        }
        return m[key].stats.avg;
    }
}
