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
    void usageThread(std::shared_ptr<UsageManager>);
    CSVWriter m_writer;
    std::mutex m_mtx;
    std::atomic<bool> m_run;
    std::vector<std::shared_ptr<UsageManager>> m_usages;
    std::vector<std::shared_ptr<std::thread>>m_thrs;
};


#endif
