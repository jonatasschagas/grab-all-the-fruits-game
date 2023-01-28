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
    GameSize worldSize = pPlatformManager->getWorldSizeUnits();
    setSize(worldSize.w, worldSize.h);
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
    TileMapSprite* pTileMapSprite = new TileMapSprite(GameSize(5, 5), pPlatformManager);
    pTileMapSprite->loadMap(pTileMapData);
    pTileMapSprite->setXY(0, 0);
    addChild(pTileMapSprite);
    GameSize worldSize = pPlatformManager->getWorldSizeUnits();
    pTileMapSprite->setSize(worldSize.w, worldSize.h);
    
    m_pWorld = new World(Vector2(
        pTileMapSprite->getTileMapWidth() * pTileMapSprite->getTileSizeInWorldUnits().w, 
        pTileMapSprite->getTileMapHeight() * pTileMapSprite->getTileSizeInWorldUnits().h));
    m_pMap = new Map(m_pWorld, pTileMapSprite);
    
    // creating the player   
    //TODO: player size should be read from the config file
    m_pPlayer = Player::create(pPlatformManager, m_pWorld, *m_pDataCacheManager);
    addChild(m_pPlayer);
}

void GameView::render()
{
    Sprite::render();
}

void GameView::update(float delta)
{   
    Sprite::update(delta);
    
    m_pWorld->update(delta);
}

void GameView::readInput(int x, int y, bool pressed)
{
}

void GameView::updateEditor()
{
#if TARGET_OS_OSX
#endif
}
