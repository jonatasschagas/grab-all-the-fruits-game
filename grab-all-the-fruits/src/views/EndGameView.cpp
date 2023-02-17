#include "EndGameView.hpp"

#include "view/ViewManager.hpp"
#include "data/TileMapData.hpp"
#include "core/TileMapSprite.hpp"
#include "level/LevelManager.hpp"
#include "core/AnimatedSprite.hpp"
#include "imgui/imgui.h"

#include<array> 

EndGameView::EndGameView(PlatformManager* pPlatformManager) : View(pPlatformManager)
{
    initializeMembers();

    m_pDataCacheManager = new DataCacheManager();
}

EndGameView::~EndGameView()
{
    delete m_pDataCacheManager;
    
    initializeMembers();
}

void EndGameView::initialize(ViewManager* pViewManager)
{
    m_pViewManager = pViewManager;

    // setting the view as a sprite that covers the whole screen
    PlatformManager* pPlatformManager = pViewManager->getPlatformManager();
    Vector2 screenSizeInGameUnits = pPlatformManager->getScreenSizeInGameUnits();
    setSize(screenSizeInGameUnits);
    setXY(0, 0);
    setPivotAtCenter(true);

    Vector2 tileSize(5,5);
    
    m_pBackground = new LevelBackground(tileSize, pPlatformManager, "assets/levels/backgrounds/Blue.png", Vector2(32, 32));
    m_pBackground->setXY(0, 0);
    addChild(m_pBackground);

    //TODO: read parameters from levels file...
    TileMapData* m_pMapData1 = new TileMapData("assets/levels/menu.json", "assets/levels", "assets/levels");
    TileMapData* m_pMapData2 = new TileMapData("assets/levels/menu.json", "assets/levels", "assets/levels");
    
    m_pMap1 = new TileMapSprite(tileSize, pPlatformManager);
    m_pMap1->loadMap(m_pMapData1, "meta");
    m_pMap1->setXY(0, 0);
    m_pMap1->setPivotAtCenter(true);
    addChild(m_pMap1);
    m_pMap1->setSize(screenSizeInGameUnits);
    m_pMap1->setDisableCamera(true);

    m_pMap2 = new TileMapSprite(tileSize, pPlatformManager);
    m_pMap2->loadMap(m_pMapData2, "meta");
    m_pMap2->setXY(0, 0);
    m_pMap2->setPivotAtCenter(true);
    addChild(m_pMap2);
    m_pMap2->setSize(screenSizeInGameUnits);
    m_pMap2->setX(screenSizeInGameUnits.x);
    m_pMap2->setDisableCamera(true);

    array<string,4> characters {
        "assets/characters/masked/masked_animation.json", 
        "assets/characters/ninja-frog/ninja-frog_animation.json",
        "assets/characters/pink-man/pink-man_animation.json",
        "assets/characters/virtual-guy/virtual-guy_animation.json",
    };
    
    const float offsetX = 35;
    for (int i = 0; i < characters.size(); i++)
    {
        AnimatedSprite* pCharacter = new AnimatedSprite(pPlatformManager, *m_pDataCacheManager, characters[i]);
        addChild(pCharacter);
        pCharacter->setSize(tileSize);
        pCharacter->setX(offsetX + tileSize.x * (i + 1));
        pCharacter->setY(screenSizeInGameUnits.y - 1.5 * tileSize.y);
        pCharacter->play("run");
    }

    Sprite* pBeatGameImage = new Sprite(pPlatformManager);
    addChild(pBeatGameImage);
    pBeatGameImage->loadTexture("assets/logo/game-complete.png");
    pBeatGameImage->setXY(50, 13);
    pBeatGameImage->setSize(80, 60);
    pBeatGameImage->setTextureCoordinates(0, 0, 1024, 768);

    pPlatformManager->loadMusic("assets/music/POL-treasure-match-short.wav");
    pPlatformManager->playMusic("assets/music/POL-treasure-match-short.wav");
}

void EndGameView::receiveEvent(Event* pEvent)
{

}

void EndGameView::render()
{
    Sprite::render();
}

void EndGameView::update(float delta)
{
    PlatformManager* pPlatformManager = m_pViewManager->getPlatformManager();
    Vector2 screenSizeInGameUnits = pPlatformManager->getScreenSizeInGameUnits();
    
    float currX = m_pMap1->getX();
    //TODO: Make this speed configurable
    float newX = currX - 2 * delta;
    m_pMap1->setX(newX);    
    m_pMap2->setX(newX + screenSizeInGameUnits.x);

    float currentPosMap1 = newX + screenSizeInGameUnits.x;
    if (currentPosMap1 < 0)
    {
        TileMapSprite* pTemp = m_pMap1;
        m_pMap1 = m_pMap2;
        m_pMap2 = pTemp;
    }

    Sprite::update(delta);
}

void EndGameView::readInput(int x, int y, bool pressed)
{

}

void EndGameView::updateEditor()
{
    int screenWidth = m_pViewManager->getPlatformManager()->getScreenWidth();
    int screenHeight = m_pViewManager->getPlatformManager()->getScreenHeight();
    ImGui::SetNextWindowPos(ImVec2(screenWidth * 0.5f, screenHeight * 0.65f), ImGuiCond_::ImGuiCond_Always, ImVec2(0.5f,0.5f));

    ImGui::Begin("Thank you!", nullptr, 
    ImGuiWindowFlags_::ImGuiWindowFlags_NoResize | 
    ImGuiWindowFlags_::ImGuiWindowFlags_NoMove |
    ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar |
    ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar);

    ImGui::SetWindowFontScale(1.3f);
    ImGui::SetWindowSize(ImVec2(500, 100));

    ImGui::Text("Thank you for playing the game! I hope you enjoyed it!");
    ImGui::Text("If you have any feedback, please let me know!");
    ImGui::Text("You can find me on twitter: @jonataschagas3");
    
    if (ImGui::Button("Close"))
    {
        m_pViewManager->switchView("main-menu");
    }

    ImGui::End();
}

