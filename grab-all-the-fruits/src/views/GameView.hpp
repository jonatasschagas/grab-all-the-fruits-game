#pragma once
#ifndef GameView_hpp
#define GameView_hpp

#include <stdio.h>

#include "data/DataCacheManager.hpp"
#include "core/Sprite.hpp"
#include "ui/MainMenu.hpp"
#include "ui/ButtonClickListener.hpp"
#include "core/TileMapSprite.hpp"
#include "view/View.h"
#include <vector>

class Player;
class World;
class Map;
class ViewManager;
class PlatformManager;
class AnimatedObjectsFactory;

class GameView : public View, public ButtonClickListener
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

    void onClick(const string& rButtonName) override;
    
private:
    
    void initGame();
    
    ViewManager* m_pViewManager;
    DataCacheManager* m_pDataCacheManager;
    TileMapSprite* m_pTileMapSprite;
    World* m_pWorld;
    Player* m_pPlayer;
    Vector2 m_tileSizeInGameUnits;
    Map* m_pMap;
    MainMenu* m_pMainMenu;
    AnimatedObjectsFactory* m_pAnimatedObjectsFactory;

    bool m_initialized;
    bool m_started;
    
    void initializeMembers()
    {
        m_pViewManager = nullptr;
        m_pDataCacheManager = nullptr;
        m_initialized = false;
        m_pWorld = nullptr;
        m_pTileMapSprite = nullptr;
        m_pPlayer = nullptr;
        m_pMap = nullptr;
        m_pMainMenu = nullptr;
        m_pAnimatedObjectsFactory = nullptr;
        m_started = false;
        m_tileSizeInGameUnits = Vector2(0, 0);
    }
    
};
#endif /* GameView */
