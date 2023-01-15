#include "SDLGame.hpp"
#include "views/GrabAllTheFruitsGame.hpp"

int main(int argc, char **argv)
{
    int exitCode = 0;

    SDLGameConfigs sdlGameConfigs;    
    sdlGameConfigs.screenWidth = 800;
    sdlGameConfigs.screenHeight = 640;
    sdlGameConfigs.windowTitle = "Grab All The Fruits!";
    
    // black background screen
    sdlGameConfigs.backgroundColor.r = 0;
    sdlGameConfigs.backgroundColor.g = 0;
    sdlGameConfigs.backgroundColor.b = 0;
    sdlGameConfigs.backgroundColor.a = 1;
    
    GrabAllTheFruitsGame* pGame = new GrabAllTheFruitsGame();

    printf("\n\n Starting the game\n\n");
    SDLGame* pSDLGame = new SDLGame(sdlGameConfigs, pGame);
    
    exitCode = pSDLGame->run();
    
    delete pSDLGame;
    
    return exitCode;
}
