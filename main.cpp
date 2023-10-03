#include "CSVWrite.hpp"
#include "UsageManager.hpp"
#include <thread>
#include <chrono>
#include <iostream>
#include <signal.h>

#define METRIC_TIME_SLEEP 60s

using namespace std::chrono_literals;

void cpu_thread(UsageManager &usage, CSVWriter &writer);
void ram_thread(UsageManager &usage, CSVWriter &writer);

std::mutex mtx;
std::atomic<bool> run;

int main(){
    std::string stop = "";
    std::vector <std::string> tabs = {"param", "var"};
    CSVWriter writer = CSVWriter();
    UsageManager usage = UsageManager();
    
    writer.writeTabs(tabs);
    
    // Остановку цикла отдаем атомик переменной
    run.store(true, std::memory_order_relaxed);
    
    // Тут и создание и старт
    std::thread cpu_thr(cpu_thread, std::ref(usage), std::ref(writer));
    std::thread ram_thr(ram_thread, std::ref(usage), std::ref(writer));
    
    while (stop != "stop"){
        std::cin >> stop;
    }
    
    // останавливаем циклы
    run.store(false, std::memory_order_relaxed);
    
    //Ловим потоки
    cpu_thr.join();
    ram_thr.join();
    
    return 0;
}

void cpu_thread(UsageManager &usage, CSVWriter &writer){
    std::vector <std::string> param = {"cpu", ""};
    auto past = std::chrono::system_clock::now();

    
    while (run.load(std::memory_order_relaxed)){
        std::this_thread::sleep_for(60ms);
        auto now = std::chrono::system_clock::now();
        
        // Время пришло или нет?
        if (now < past + METRIC_TIME_SLEEP){
            continue;
        }
        // проверяем изменился ли статус пока ждали
        if (!run.load(std::memory_order_relaxed)){
            return;
        }
        
        usage.updateCpuInfo();
        param[1] = std::to_string(usage.getCpuUsage());
        
        
        //Блокируем запись в файл для других потоков
        mtx.lock();
        writer.writeParams(param);
        mtx.unlock();
        
        past = std::chrono::system_clock::now();
    }
}

void ram_thread(UsageManager &usage, CSVWriter &writer){
    std::vector <std::string> param = {"", ""};
    auto past = std::chrono::system_clock::now();
    
    while (run.load(std::memory_order_relaxed)){
        std::this_thread::sleep_for(60ms);
        auto now = std::chrono::system_clock::now();
        
        // Время пришло или нет?
        if (now < past + METRIC_TIME_SLEEP){
            continue;
        }
        // проверяем изменился ли статус пока ждали
        if (!run.load(std::memory_order_relaxed)){
            return;
        }
        
        usage.updateRamInfo();
        param[0] = "used_memory";
        param[1] = std::to_string(usage.getUsedMemory());
        
        //Блокируем запись в файл для других потоков
        mtx.lock();
        writer.writeParams(param);
        mtx.unlock();
        
        param[0] = "free_memory";
        param[1] = std::to_string(usage.getFreeMemory());
        
        //Блокируем запись в файл для других потоков
        mtx.lock();
        writer.writeParams(param);
        mtx.unlock();
        
        past = std::chrono::system_clock::now();
    }
}
