#include "GameView.hpp"

#include "logic/World.hpp"
#include "logic/Map.hpp"
#include "objects/Player.hpp"
#include "view/ViewManager.hpp"
#include "event/Event.hpp"
#include "platform/PlatformManager.h"
#include "core/TileMapSprite.hpp"
#include "data/TileMapData.hpp"

#ifdef __APPLE__
    #include <TargetConditionals.h>
#endif

#if IMGUI_ENABLED
#include "imgui/imgui.h"
#endif

GameView::GameView(PlatformManager* pPlatformManager) : View(pPlatformManager)
{
    initializeMembers();
}

GameView::~GameView()
{
    delete m_pWorld;
    delete m_pMap;

    initializeMembers();
}

void GameView::initialize(ViewManager* pViewManager)
{
    m_pViewManager = pViewManager;
    m_pDataCacheManager = pViewManager->getDataCacheManager();

    // setting the view as a sprite that covers the whole screen
    PlatformManager* pPlatformManager = pViewManager->getPlatformManager();
    Vector2 screenSize = pPlatformManager->getScreenSizeInGameUnits();
    setSize(screenSize);
    setXY(0, 0);
    setPivotAtCenter(true);
   
    initGame();
    
    m_initialized = true;
}


void GameView::receiveEvent(Event* pEvent)
{
    m_pWorld->receiveEvent(pEvent);
    m_pPlayer->receiveEvent(pEvent);
}

void GameView::initGame()
{
    PlatformManager* pPlatformManager = m_pViewManager->getPlatformManager();
    
    /*LOADING MAP*/
    //TODO: load map from file
    TileMapData* pTileMapData = new TileMapData("assets/levels/level1.json", "assets/levels", "assets/levels");
    
    //TODO: tile size should be read from the config file
    TileMapSprite* pTileMapSprite = new TileMapSprite(Vector2(5, 5), pPlatformManager);
    pTileMapSprite->loadMap(pTileMapData);
    pTileMapSprite->setXY(0, 0);
    addChild(pTileMapSprite);
    Vector2 screenSize = pPlatformManager->getScreenSizeInGameUnits();
    pTileMapSprite->setSize(screenSize);
    
    m_pWorld = new World(pTileMapSprite->getMapSizeInGameUnits());
    m_pMap = new Map(m_pWorld, pTileMapSprite);
    
    // creating the player   
    //TODO: player size should be read from the config file
    m_pPlayer = Player::create(pPlatformManager, m_pWorld, *m_pDataCacheManager);
    // player has to be a child of the map, so it can be moved with the camera
    pTileMapSprite->addChild(m_pPlayer);
}

void GameView::render()
{
    if (!m_started)
    {
        return;
    }   

    Sprite::render();
}

void GameView::update(float delta)
{   
    Sprite::update(delta);
    
    m_pWorld->update(delta);
    m_pMap->updateCameraPosition(m_pPlayer->getGamePosition());
}

void GameView::readInput(int x, int y, bool pressed)
{
}

void GameView::updateEditor()
{
    if (m_started)
    {
        return;
    }
    
#if IMGUI_ENABLED
    //TODO: move this to a separate class
    // center the window
    PlatformManager* pPlatformManager = getPlatformManager();
    ImGui::SetNextWindowPos(ImVec2(pPlatformManager->getScreenWidth() * 0.5f, pPlatformManager->getScreenHeight() * 0.5f), ImGuiCond_::ImGuiCond_Always, ImVec2(0.5f,0.5f));

    ImGui::Begin("Grab All The Fruits!", nullptr, 
        ImGuiWindowFlags_::ImGuiWindowFlags_NoResize | 
        ImGuiWindowFlags_::ImGuiWindowFlags_NoMove | 
        ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar | 
        ImGuiWindowFlags_::ImGuiWindowFlags_NoBackground);
    
    if (ImGui::Button("New Game")) 
    {
        m_started = true;
    }
    else if (ImGui::Button("Continue")) 
    {
        //TODO: load game
        m_started = true;
    }
    ImGui::End();

#endif
}
