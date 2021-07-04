#include "Log.h"
#include "DEFINITIONS.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

std::vector<std::string> vectorLog;

void LogPush(std::string text){
    std::cout<<text<<std::endl;
    vectorLog.push_back(text);
}

void LogWrite(){
    if(GENERATE_LOG){
        std::ofstream file;
        std::string line;

        file.open("log.txt",std::fstream::out);
        for(int i = 0; i < vectorLog.size(); ++i){
            file<<vectorLog[i]<<std::endl;
        }
        file.close();
    }
}
