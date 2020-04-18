#include "Game.h"
#include "DEFINITIONS.h"
#define _WIN32_WINNT 0x0500
#include <windows.h>

int main(){
    if(SHOW_CONSOLE == 0){
        ShowWindow(GetConsoleWindow(), SW_HIDE);
    }

    Game(SCREEN_WIDTH,SCREEN_HEIGHT,"BWNet Simulator");

    LogWrite();
    return EXIT_SUCCESS;
}
