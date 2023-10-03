#include "CSVWrite.hpp"

CSVWriter::CSVWriter(){
    m_file.open("file.csv", std::ios_base::out);
    m_tabs = std::vector<std::string>();
}

CSVWriter::~CSVWriter(){
    m_file.close();
}

void CSVWriter::writeTabs(std::vector<std::string> &tabs){
    if (!tabs.size() && m_tabs.size()){
        return;
    }
    
    m_tabs.clear();
    for (auto tab : tabs){
        m_tabs.push_back(tab);
        m_file << tab << ", ";
    }
    m_file << "\n";
}

void CSVWriter::writeParams(std::vector<std::string> &params){
    if (params.size() != m_tabs.size()){
        return;
    }
    
    for (auto param : params){
        m_file << param << ", ";
        
    }
    
    m_file << "\n";
}
