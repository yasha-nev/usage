#ifndef Collector_hpp
#define Collector_hpp

#include <string>
#include <thread>
#include <chrono>
#include <iostream>
#include "CSVWrite.hpp"
#include "UsageManager.hpp"

#define METRIC_TIME_SLEEP 10s

using namespace std::chrono_literals;

class Collector{
public:
    Collector();
    ~Collector();
    void startCollect();
    void stopCollect();
    double getCurrantCpuUsage();
    long long getCurrantFreeRam();
    long long getCurrantUsageRam();
    void cleanup();

private:
    void cpuThread();
    void ramThread();
    
    UsageManager m_usage;
    CSVWriter m_writer;
    std::mutex m_mtx;
    std::atomic<bool> m_run;
    
    std::thread m_cpuThr;
    std::thread m_ramThr;
};


#endif
