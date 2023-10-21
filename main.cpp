#include "Collector.hpp"
#include <iostream>

int main(){
    Collector collect;
    std::string str = "";
    
    collect.startCollect();
    
    while (str != "quit"){
        std::cin >> str;
    }
    
    collect.stopCollect();
    
    return 0;
}
