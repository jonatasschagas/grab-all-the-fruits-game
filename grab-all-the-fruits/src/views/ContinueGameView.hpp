#pragma once
#ifndef ContinueGameView_hpp
#define ContinueGameView_hpp

#include "GameView.hpp"
#include "platform/PlatformManager.h"

class ContinueGameView : public GameView
{
public:

    ContinueGameView(PlatformManager* pPlatformManager) : GameView(pPlatformManager) {};

    void onEnter(void* param) override;
};

#endif /* ContinueGameView_hpp */
