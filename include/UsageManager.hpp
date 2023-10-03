#ifndef UsageManager_hpp
#define UsageManager_hpp

class UsageManager{
    
public:
    UsageManager();
    double getCpuUsage();
    long long getUsedMemory();
    long long getFreeMemory();
    void updateCpuInfo();
    void updateRamInfo();
    void setWaitSec(unsigned int  sec);
    
private:
    double m_cpuUsage;
    long long m_usedMemory;
    long long m_freeMemory;
    int m_waitSec;
    
    int getCpuInfo(unsigned long *system, unsigned long *user, unsigned long * nice,
                   unsigned long *idle);
};



#endif
