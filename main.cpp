#include "Collector.hpp"
#include <iostream>

int main(){
    Collector collect;
    std::string str = "";
    
    while (str != "quit"){
        std::cin >> str;
        
        if (str == "start"){
            std::cout << "start collect" << "\n";
            collect.startCollect();
        }
        else if (str == "stop"){
            std::cout << "stop collect" << "\n";
            collect.stopCollect();
        }
        
        else if (str == "clean"){
            std::cout << "cleanup" << "\n";
            collect.cleanup();
        }
        else if (str == "get"){
            std::cout << "cpu (%): " << collect.getCurrantCpuUsage() << "\n"
            << "free ram memory (bit): " << collect.getCurrantFreeRam() << "\n"
            << "usage ram memory (bit): " << collect.getCurrantUsageRam() << "\n";
        }
        
        else if (str == "quit"){
            break;
        }
        else{
            continue;
        }
    }
    
    return 0;
}
