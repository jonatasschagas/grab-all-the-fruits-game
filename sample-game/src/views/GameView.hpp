#pragma once
#ifndef GameView_hpp
#define GameView_hpp

#include <stdio.h>

#include "event/EventListener.hpp"
#include "data/DataCacheManager.hpp"
#include "core/Sprite.hpp"
#include "view/View.h"
#include <vector>

class Masked;
class ViewManager;

class GameView : public EventListener, public View
{
public:
    
    GameView();
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
    Masked* m_pPlayer;

    bool m_initialized;
    
    void initializeMembers()
    {
        m_pViewManager = nullptr;
        m_pDataCacheManager = nullptr;
        m_initialized = false;
        m_pPlayer = nullptr;
    }
    
};
#endif /* GameView */
