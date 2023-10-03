#include "Collector.hpp"

Collector::Collector(){
    m_usage = UsageManager();
    
    std::vector <std::string> tabs = {"time", "param", "var"};
    m_writer.writeTabs(tabs);
    
    m_run.store(false, std::memory_order_relaxed);
}

Collector::~Collector(){
    stopCollect();
}

void Collector::startCollect(){
    if (m_run.load(std::memory_order_relaxed)){
        return;
    }
    //Отдаем управление потоком атомику
    m_run.store(true, std::memory_order_relaxed);
    
    m_cpuThr = std::thread(&Collector::cpuThread, this);
    m_ramThr = std::thread(&Collector::ramThread, this);
}

void Collector::stopCollect(){
    if (!m_run.load(std::memory_order_relaxed)){
        return;
    }
    
    //Говорим потокам остановится
    m_run.store(false, std::memory_order_relaxed);
    
    //Ловим
    m_cpuThr.join();
    m_ramThr.join();
}

void Collector::cleanup(){
    //Сбросить что?
    return;
}

double Collector::getCurrantCpuUsage(){
    return m_usage.getCpuUsage();
}

long long Collector::getCurrantFreeRam(){
    return m_usage.getFreeMemory();
}

long long Collector::getCurrantUsageRam(){
    return m_usage.getFreeMemory();
}


void Collector::cpuThread(){
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
        
        m_usage.updateCpuInfo();
        std::time_t t = std::chrono::system_clock::to_time_t(now);
        std::string time = std::ctime(&t);
        time.resize(time.size()-1);
        param[0] = time;
        param[1] = "cpu_usage";
        param[2] = std::to_string(m_usage.getCpuUsage());
        
        
        //Блокируем запись в файл для других потоков
        m_mtx.lock();
        m_writer.writeParams(param);
        m_mtx.unlock();
        
        past = std::chrono::system_clock::now();
    }
}

void Collector::ramThread(){
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
        
        m_usage.updateRamInfo();
        
        std::time_t t = std::chrono::system_clock::to_time_t(now);
        std::string time = std::ctime(&t);
        time.resize(time.size()-1);
        param[0] = time;
        param[1] = "used_mem";
        param[2] = std::to_string(m_usage.getUsedMemory());
        
        //Блокируем запись в файл для других потоков
        m_mtx.lock();
        m_writer.writeParams(param);
        m_mtx.unlock();
        
        param[0] = time;
        param[1] = "free_mem";
        param[2] = std::to_string(m_usage.getFreeMemory());
        
        //Блокируем запись в файл для других потоков
        m_mtx.lock();
        m_writer.writeParams(param);
        m_mtx.unlock();
        
        past = std::chrono::system_clock::now();
    }
}
