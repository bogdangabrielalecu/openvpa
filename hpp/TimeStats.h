/***************************************************************
 * Name:      TimeStats.h
 * Purpose:   Code for Time Measurement and Statistics Class Header
 * Author:    Bogdan Alecu
 * Created:   2011-09-16
 * Copyright: Bogdan Alecu
 * License:   GNU GPL
 **************************************************************/

#ifndef OPENVPATIMESTATS_H
#define OPENVPATIMESTATS_H

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <exception>
#include <stdio.h>
#include <ctime>
#include <time.h>

using namespace std;

namespace openvpa {

    typedef vector<double> Probes;

    struct StatsTable {
        double max, min, avg;
        long int count;
    };

    struct TimeStatsTable {
        clock_t start;
        StatsTable stats;
        Probes probes;
    };

    typedef map<string, TimeStatsTable> TimeStatsTableMap;

    typedef map<string, StatsTable> StatsTableMap;

    class TimeStats {

        private:
            TimeStatsTableMap m;
            int mode;
            string last_key;
            void default_init();
            void lastKey(string key);
            string lastKey();
            double enqueue(string key, double diff);

        public:
            static const int MODE_NONE = 1;
            static const int MODE_SIMPLE = 2;
            static const int MODE_FULL = 3;

            TimeStats();
            TimeStats(int value);
            ~TimeStats();

            clock_t start(string key);
            clock_t restart(string key);
            double stop(string key);
            double stop();
            double diff(string key);
            double diff();
            double max(string key);
            double min(string key);
            double avg(string key);
            TimeStatsTableMap map() {return m;};
    };
}
#endif
