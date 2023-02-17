#pragma once
#ifndef NewGameView_hpp
#define NewGameView_hpp

#include "GameView.hpp"
#include "platform/PlatformManager.h"

class NewGameView : public GameView
{
public:

    NewGameView(PlatformManager* pPlatformManager) : GameView(pPlatformManager) {};

    void onEnter(void* param) override;

};

#endif /* NewGameView_hpp */