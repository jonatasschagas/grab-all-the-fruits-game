#include "NewGameView.hpp"

void NewGameView::onEnter(void* param)
{
    setCurrentLevel(0);
    initGame();
}
