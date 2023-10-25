#ifndef UsageManager_hpp
#define UsageManager_hpp

#include <string>

class UsageManager{
    
public:
    UsageManager(){}
    virtual ~UsageManager(){}
    virtual void updateMetric() = 0;
    virtual void cleanup() = 0;
    virtual std::string getMetric() = 0;
    virtual std::string getMetricName();

protected:
    std::string m_metricName;
};

class CpuUsageManager : public UsageManager {
public:
    CpuUsageManager();
    virtual void updateMetric() override;
    virtual void cleanup() override;
    virtual std::string getMetric() override;
    void setWaitSec(unsigned int sec);

private:
    double m_cpuUsage;
    int m_waitSec;
    int getCpuInfo(unsigned long *system, unsigned long *user, unsigned long * nice,
                   unsigned long *idle);
};

class RamUsageManager : public UsageManager {
public:
    RamUsageManager();
    virtual void updateMetric() override;
    virtual void cleanup() override;
    virtual std::string getMetric() override;
private:
    double m_usedMemory;
};

class RamFreeManager : public UsageManager {
public:
    RamFreeManager();
    virtual void updateMetric() override;
    virtual void cleanup() override;
    virtual std::string getMetric() override;
private:
    double m_freeMemory;
};


#endif
