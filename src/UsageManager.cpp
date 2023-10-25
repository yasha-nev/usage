#include "UsageManager.hpp"
#include <stdio.h>
#include <unistd.h>
#include <mach/mach.h>
#include <mach/vm_statistics.h>
#include <mach/mach_types.h>
#include <mach/mach_init.h>
#include <mach/mach_host.h>


std::string UsageManager::getMetricName(){
    return m_metricName;
}

CpuUsageManager::CpuUsageManager(){
    m_metricName = "cpu_usage";
    m_waitSec = 1;
    m_cpuUsage = 0;
}

std::string CpuUsageManager::getMetric(){
    return std::to_string(m_cpuUsage);
}

void CpuUsageManager::setWaitSec(unsigned int sec){
    m_waitSec = sec;
}

void CpuUsageManager::cleanup(){
    m_cpuUsage = 0;
}

int CpuUsageManager::getCpuInfo(unsigned long *system, unsigned long *user, unsigned long * nice,
                          unsigned long *idle){
    processor_cpu_load_info_t struct_cpu_data;
    mach_msg_type_number_t  cpu_msg_count = 0;
    kern_return_t err = 0;
    natural_t cpp_num = 0;
    
    // get CPU states
    err = host_processor_info( mach_host_self(), PROCESSOR_CPU_LOAD_INFO, &cpp_num,
                        (processor_info_array_t *)&struct_cpu_data, &cpu_msg_count );
    
    for(int i = 0; i<(int)cpp_num; i++){
            *system += struct_cpu_data[i].cpu_ticks[CPU_STATE_SYSTEM];
            *user += struct_cpu_data[i].cpu_ticks[CPU_STATE_USER];
            *nice += struct_cpu_data[i].cpu_ticks[CPU_STATE_NICE];
            *idle += struct_cpu_data[i].cpu_ticks[CPU_STATE_IDLE];
    }

    return err;
}

void CpuUsageManager::updateMetric(){
    // cpu system state
    unsigned long systemPrevious = 0;
    unsigned long systemNext = 0;

    //cpu user state
    unsigned long userPrevious = 0;
    unsigned long userNext = 0;

    //cpu Nice state
    unsigned long nicePrevious = 0;
    unsigned long niceNext = 0;

    //cpu Idle state
    unsigned long idlePrevious = 0;
    unsigned long idleNext = 0;
    
    // Получаем данные процессора
    int err = getCpuInfo(&systemPrevious, &userPrevious, &nicePrevious, &idlePrevious);
    if(err != KERN_SUCCESS){
            printf("Kernel error: %s\n", mach_error_string(err));
            return;
    }
    
    sleep(m_waitSec);
    
    // Обновляем данные процессора
    err = getCpuInfo(&systemNext, &userNext, &niceNext, &idleNext);
    if(err != KERN_SUCCESS)
    {
            printf("Kernel error: %s\n", mach_error_string(err));
            return;
    }
    
    // Делаем магические вычисления
    float usageTime = (float)(systemNext - systemPrevious) + (userNext - userPrevious) + (niceNext - nicePrevious);
    float totalTime = usageTime + (float)(idleNext - idlePrevious);
    
    // Результат в процентах
    m_cpuUsage = usageTime / totalTime * 100;
}

RamFreeManager::RamFreeManager(){
    m_freeMemory = 0;
    m_metricName = "free_mem";
}

static double bytes2gbytes(double bytes){
    return bytes / 1024 / 1024 / 1024;
}


std::string RamFreeManager::getMetric(){
    return std::to_string(m_freeMemory);
}

void RamFreeManager::cleanup(){
    m_freeMemory = 0;
}

void RamFreeManager::updateMetric(){
    vm_size_t pageSize;
    mach_msg_type_number_t count;
    vm_statistics64_data_t vmStats;
    mach_port_t machPort = mach_host_self();
    long long free_mem;
    
    count = sizeof(vmStats) / sizeof(natural_t);
    // Пытаемся запросить статистику об используемой памяти
    if (KERN_SUCCESS == host_page_size(machPort, &pageSize) &&
        KERN_SUCCESS == host_statistics64(machPort, HOST_VM_INFO,
                                        (host_info64_t)&vmStats, &count))
    {
        // Если удалось
        // Магические вычесления. Результат должен быть в гб
        free_mem = (int64_t)vmStats.free_count * (int64_t)pageSize;
        m_freeMemory = bytes2gbytes(free_mem);
    }
}

RamUsageManager::RamUsageManager(){
    m_usedMemory = 0;
    m_metricName = "usage_mem";
}

std::string RamUsageManager::getMetric(){
    return std::to_string(m_usedMemory);
}

void RamUsageManager::cleanup(){
    m_usedMemory = 0;
}

void RamUsageManager::updateMetric(){
    vm_size_t pageSize;
    mach_msg_type_number_t count;
    vm_statistics64_data_t vmStats;
    mach_port_t machPort = mach_host_self();
    long long used_mem;
    
    count = sizeof(vmStats) / sizeof(natural_t);
    // Пытаемся запросить статистику об используемой памяти
    if (KERN_SUCCESS == host_page_size(machPort, &pageSize) &&
        KERN_SUCCESS == host_statistics64(machPort, HOST_VM_INFO,
                                        (host_info64_t)&vmStats, &count))
    {
        // Если удалось
        // Магические вычесления. Результат должен быть в гб
        used_mem = ((int64_t)vmStats.active_count +
                                 (int64_t)vmStats.inactive_count +
                        (int64_t)vmStats.wire_count) *  (int64_t)pageSize;
        m_usedMemory = bytes2gbytes(used_mem);
        
    }
}
