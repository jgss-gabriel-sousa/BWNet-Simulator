#include "Game.h"
#include "DEFINITIONS.h"
#define _WIN32_WINNT 0x0500
#include <windows.h>
#include <ctime>

int main(){
    srand(std::time(0));
    if(SHOW_CONSOLE == 0){
        ShowWindow(GetConsoleWindow(), SW_HIDE);
    }

    Game(SCREEN_WIDTH,SCREEN_HEIGHT,"BWNet Simulator");

    LogWrite();
    return EXIT_SUCCESS;
}
