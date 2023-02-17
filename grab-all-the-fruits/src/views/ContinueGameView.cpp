#include "ContinueGameView.hpp"

void ContinueGameView::onEnter(void* param)
{
    //TODO: implementing loading the previous level from local storage
    int currentLevel = 1;
    setCurrentLevel(currentLevel);
    initGame();
}
