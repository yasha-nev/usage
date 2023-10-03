#ifndef CSVWrite_hpp
#define CSVWrite_hpp

#include <vector>
#include <string>
#include <fstream>

class CSVWriter{
    
public:
    CSVWriter();
    ~CSVWriter();
    void writeTabs(std::vector<std::string> &tab);
    void writeParams(std::vector<std::string> &param);
    void read();
    
private:
    std::vector <std::string> m_tabs;
    std::fstream m_file;
};



#endif
