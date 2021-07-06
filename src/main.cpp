#include "Game.h"
#include "DEFINITIONS.h"
#define _WIN32_WINNT 0x0500
#include <windows.h>
#include <ctime>
#include <fstream>
#include <string>
#include <iostream>

extern void LoadConfigs();

bool SHOW_CONSOLE = true;
unsigned int GAME_FRAMERATE = 45;

int main(){
    srand(std::time(0));

    LoadConfigs();

    if(!SHOW_CONSOLE){
        ShowWindow(GetConsoleWindow(), SW_HIDE);
    }

    Game(SCREEN_WIDTH,SCREEN_HEIGHT,"BWNet Simulator",GAME_FRAMERATE);

    LogWrite();
    return EXIT_SUCCESS;
}


void LoadConfigs(){
    std::ifstream file;
    std::string line;

    file.open("config.cfg");

    while(!file.eof() && !file.bad()){
        getline(file,line);

        if(line.find("ShowConsole") != std::string::npos){
            if(line.substr(line.find("=")+1) == "true"){
                SHOW_CONSOLE = true;
            }
            else{
                SHOW_CONSOLE = false;
            }
        }
        else if(line.find("FrameRate") != std::string::npos){
            GAME_FRAMERATE = stoi(line.substr(line.find("=")+1));
        }
    }
}
