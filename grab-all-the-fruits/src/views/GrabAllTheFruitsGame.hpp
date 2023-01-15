#pragma once
#ifndef GrabAllTheFruitsGame_hpp
#define GrabAllTheFruitsGame_hpp

#include <stdio.h>

#include "view/Game.hpp"
#include <vector>

class ViewManager;
class DataCacheManager;

class GrabAllTheFruitsGame : public Game
{
public:
    
    GrabAllTheFruitsGame();
    ~GrabAllTheFruitsGame();
    
    void initialize(PlatformManager* pManager) override;
    void receiveEvent(Event* pEvent) override;
    void update(const float deltaTime) override;
    void updateEditor(const float deltaTime) override;
    void render() override;
    
private:
    
    PlatformManager* m_pPlatformManager;
    ViewManager* m_pViewManager;
    DataCacheManager* m_pDataCacheManager;
    
    void initializeMembers()
    {
        m_pPlatformManager = nullptr;
        m_pViewManager = nullptr;
        m_pDataCacheManager = nullptr;
    }
    
};
#endif /* GrabAllTheFruitsGame */
