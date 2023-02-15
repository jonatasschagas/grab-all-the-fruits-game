#pragma once
#ifndef MainMenuView_hpp
#define MainMenuView_hpp

#include <stdio.h>

#include "data/DataCacheManager.hpp"
#include "core/Vector2.h"
#include "ui/MainMenu.hpp"
#include "ui/ButtonClickListener.hpp"
#include "view/View.h"
#include <vector>

class ViewManager;
class PlatformManager;
class TileMapSprite;
class TileMapData;
class AnimatedSprite;
class LevelBackground;

class MainMenuView : public View, public ButtonClickListener
{
public:
    
    MainMenuView(PlatformManager* pPlatformManager);
    ~MainMenuView();
    
    void initialize(ViewManager* pViewManager) override;
    void receiveEvent(Event* pEvent) override;
    void render() override;
    void update(float delta) override;
    
    void readInput(int x, int y, bool pressed) override;
    
    void updateEditor() override;

    void onClick(const string& rButtonName) override;

private:
    
    ViewManager* m_pViewManager;
    DataCacheManager* m_pDataCacheManager;
    MainMenu* m_pMainMenu;
    TileMapSprite* m_pMap1;
    TileMapSprite* m_pMap2;
    TileMapData* m_pMapData1;
    TileMapData* m_pMapData2;
    LevelBackground* m_pBackground;
    
    void initializeMembers()
    {
        m_pViewManager = nullptr;
        m_pDataCacheManager = nullptr;
        m_pMainMenu = nullptr;
        m_pMap1 = nullptr;
        m_pMap2 = nullptr;
        m_pMapData1 = nullptr;
        m_pMapData2 = nullptr;
        m_pBackground = nullptr;
    }
    
};
#endif /* MainMenuView */
