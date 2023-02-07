#pragma once
#ifndef GameView_hpp
#define GameView_hpp

#include <stdio.h>

#include "data/DataCacheManager.hpp"
#include "core/Sprite.hpp"
#include "core/Vector2.h"
#include "ui/MainMenu.hpp"
#include "ui/HUD.hpp"
#include "ui/ButtonClickListener.hpp"
#include "view/View.h"
#include "level/LevelManager.hpp"
#include <vector>

class Player;
class World;
class ViewManager;
class PlatformManager;
class AnimatedObjectsFactory;
class AnimatedObject;

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
    void createPlayer(const Vector2& position);

    AnimatedObject* createDisappearingAnimation(const Vector2& position, const Vector2& size);
    
    ViewManager* m_pViewManager;
    DataCacheManager* m_pDataCacheManager;
    World* m_pWorld;
    Player* m_pPlayer;
    Vector2 m_playerStartPosition;
    MainMenu* m_pMainMenu;
    HUD* m_pHUD;
    LevelManager* m_pLevelManager;
    AnimatedObjectsFactory* m_pAnimatedObjectsFactory;
    int m_currentLevel;

    bool m_initialized;
    bool m_started;
    bool m_died;
    bool m_debug;
    
    void initializeMembers()
    {
        m_pViewManager = nullptr;
        m_pDataCacheManager = nullptr;
        m_initialized = false;
        m_debug = false;
        m_pWorld = nullptr;
        m_pPlayer = nullptr;
        m_pMainMenu = nullptr;
        m_pAnimatedObjectsFactory = nullptr;
        m_pLevelManager = nullptr;
        m_started = false;
        m_died = false;
        m_playerStartPosition.x = 0;
        m_playerStartPosition.y = 0;
        m_pHUD = nullptr;
        m_currentLevel = 0;
    }
    
};
#endif /* GameView */
