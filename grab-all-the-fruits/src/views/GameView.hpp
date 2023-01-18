#pragma once
#ifndef GameView_hpp
#define GameView_hpp

#include <stdio.h>

#include "data/DataCacheManager.hpp"
#include "core/Sprite.hpp"
#include "view/View.h"
#include <vector>

class Player;
class World;
class ViewManager;
class PlatformManager;

class GameView : public View
{
public:
    
    GameView(PlatformManager* pPlatformManager);
    ~GameView();
    
    void initialize(ViewManager* pViewManager) override;
    void receiveEvent(Event* pEvent) override;
    void render() override;
    void update(float delta) override;
    
    void readInput(int x, int y, bool pressed) override;
    
    void updateEditor() override;
    
private:
    
    void initGame();
    
    ViewManager* m_pViewManager;
    DataCacheManager* m_pDataCacheManager;
    World* m_pWorld;
    Player* m_pPlayer;

    bool m_initialized;
    
    void initializeMembers()
    {
        m_pViewManager = nullptr;
        m_pDataCacheManager = nullptr;
        m_initialized = false;
        m_pWorld = nullptr;
        m_pPlayer = nullptr;
    }
    
};
#endif /* GameView */
