#ifndef Collector_hpp
#define Collector_hpp

#include <string>
#include <thread>
#include <chrono>
#include <iostream>
#include "CSVWrite.hpp"
#include "UsageManager.hpp"

#define METRIC_TIME_SLEEP 60s

using namespace std::chrono_literals;

class Collector{
public:
    Collector();
    ~Collector();
    void startCollect();
    void stopCollect();
    std::string getCurrentMetric(std::string name);
    void cleanup();

private:
    void usageThread(UsageManager *usage);
    CSVWriter m_writer;
    std::mutex m_mtx;
    std::atomic<bool> m_run;
    std::vector<UsageManager *> m_usages;
    std::vector<std::thread *> m_thrs;
};


#endif
