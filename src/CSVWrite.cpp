#include "CSVWrite.hpp"

CSVWriter::CSVWriter(){
    m_file.open("file.csv", std::ios_base::out);
    m_tabs = std::vector<std::string>();
}

CSVWriter::~CSVWriter(){
    m_file.close();
}

void CSVWriter::writeTabs(std::vector<std::string> &tabs){
    m_tabs.clear();
    for (auto tab : tabs){
        m_tabs.push_back(tab);
        std::string sup = std::string(' ', 25 - tab.size());
        m_file << tab << sup << ", ";
    }
    m_file << "\n";
}

void CSVWriter::writeParams(std::vector<std::string> &params){
    for (auto param : params){
        std::string sup = std::string(' ', 25 - param.size());
        m_file << param << sup << ", ";
    }

    m_file << "\n";
}
