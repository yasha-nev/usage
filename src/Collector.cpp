#include "Collector.hpp"

Collector::Collector(){
    m_usages.push_back(std::make_shared<CpuUsageManager>());
    m_usages.push_back(std::make_shared<RamFreeManager>());
    m_usages.push_back(std::make_shared<RamUsageManager>());
    
    std::vector <std::string> tabs = {"time", "param", "var"};
    m_writer.writeTabs(tabs);
    
    m_run.store(false, std::memory_order_relaxed);
}

Collector::~Collector(){
    stopCollect();
    m_usages.clear();
}

void Collector::startCollect(){
    if (m_run.load(std::memory_order_relaxed)){
        return;
    }
    
    //Отдаем управление потоком атомику
    m_run.store(true, std::memory_order_relaxed);
    
    for (auto usage : m_usages){
        auto ptr = std::make_shared<std::thread>(&Collector::usageThread, this, usage);
        m_thrs.push_back(ptr);
    }
}

void Collector::stopCollect(){
    if (!m_run.load(std::memory_order_relaxed)){
        return;
    }
    
    //Говорим потокам остановиться
    m_run.store(false, std::memory_order_relaxed);
    
    //Ловим
    for (auto thr : m_thrs){
        thr->join();
    }
    m_thrs.clear(); 
}

void Collector::usageThread(std::shared_ptr<UsageManager> usage){
    std::vector <std::string> param = {"", "", ""};
    auto past = std::chrono::system_clock::now();
    
    while (m_run.load(std::memory_order_relaxed)){
        std::this_thread::sleep_for(60ms);
        auto now = std::chrono::system_clock::now();
        
        // Время пришло или нет?
        if (now < past + METRIC_TIME_SLEEP){
            continue;
        }
        // проверяем изменился ли статус пока ждали
        if (!m_run.load(std::memory_order_relaxed)){
            return;
        }
        
        usage->updateMetric();
        std::time_t t = std::chrono::system_clock::to_time_t(now);
        std::string time = std::ctime(&t);
        time.resize(time.size()-1);
        param[0] = time;
        param[1] = usage->getMetricName();
        param[2] = usage->getMetric();
        
        //Блокируем запись в файл для других потоков
        const std::lock_guard<std::mutex> lock(m_mtx);
        m_writer.writeParams(param);
        
        past = std::chrono::system_clock::now();
    }
}

void Collector::cleanup(){
    for (auto usage : m_usages){
        usage->cleanup();
    }
}

std::string Collector::getCurrentMetric(std::string name){
    for (auto usage : m_usages){
        if (usage->getMetricName() == name){
            return usage->getMetric();
        }
    }
    return "";
}
