#include "../../sdl/SDLGame.hpp"

int main(int argc, char **argv)
{
    int exitCode = 0;

    SDLGameConfigs sdlGameConfigs;    
    sdlGameConfigs.screenWidth = 568;
    sdlGameConfigs.screenHeight = 800;
    sdlGameConfigs.windowTitle = "Sample Game";
    
    // black background screen
    sdlGameConfigs.backgroundColor.r = 0;
    sdlGameConfigs.backgroundColor.g = 0;
    sdlGameConfigs.backgroundColor.b = 0;
    sdlGameConfigs.backgroundColor.a = 1;
    
    printf("\n\n Starting the game\n\n");
    SDLGame* pSDLGame = new SDLGame(sdlGameConfigs);
    
    exitCode = pSDLGame->run("main");
    
    delete pSDLGame;
    
    return exitCode;
}
