#include "GameView.hpp"

#include "logic/World.hpp"
#include "logic/Map.hpp"
#include "objects/Player.hpp"
#include "objects/AnimatedObjectsFactory.hpp"
#include "view/ViewManager.hpp"
#include "event/Event.hpp"
#include "platform/PlatformManager.h"
#include "core/TileMapSprite.hpp"
#include "data/TileMapData.hpp"

#ifdef __APPLE__
    #include <TargetConditionals.h>
#endif

GameView::GameView(PlatformManager* pPlatformManager) : View(pPlatformManager)
{
    initializeMembers();
}

GameView::~GameView()
{
    delete m_pWorld;
    delete m_pMap;
    delete m_pMainMenu;

    initializeMembers();
}

void GameView::initialize(ViewManager* pViewManager)
{
    m_pViewManager = pViewManager;
    m_pDataCacheManager = pViewManager->getDataCacheManager();
    
    // setting the view as a sprite that covers the whole screen
    PlatformManager* pPlatformManager = pViewManager->getPlatformManager();
    Vector2 screenSizeInGameUnits = pPlatformManager->getScreenSizeInGameUnits();
    setSize(screenSizeInGameUnits);
    setXY(0, 0);
    setPivotAtCenter(true);
   
    m_pMainMenu = new MainMenu(pPlatformManager->getScreenWidth(), pPlatformManager->getScreenHeight());
    m_pMainMenu->setButtonClickListener(this);

    initGame();
    
    m_initialized = true;
}


void GameView::receiveEvent(Event* pEvent)
{
    m_pWorld->receiveEvent(pEvent);
    if (pEvent->getName().compare("create-player") == 0)
    {
        m_pPlayer = m_pAnimatedObjectsFactory->createPlayer(pEvent->getInputCoordinates(), m_tileSizeInGameUnits);
        m_pTileMapSprite->addChild(m_pPlayer);
    }
    else if (pEvent->getName().compare("fruit-collected") == 0)
    {
        AnimatedObject* pAnimatedObj = m_pAnimatedObjectsFactory->createGenericObject("collected", pEvent->getInputCoordinates(), m_tileSizeInGameUnits);
        pAnimatedObj->play("idle");
        m_pTileMapSprite->addChild(pAnimatedObj);
    }   
    else
    {
        m_pPlayer->receiveEvent(pEvent);
    }
}

void GameView::initGame()
{
    PlatformManager* pPlatformManager = m_pViewManager->getPlatformManager();
    
    /*LOADING MAP*/
    //TODO: load map from file
    TileMapData* pTileMapData = new TileMapData("assets/levels/level1.json", "assets/levels", "assets/levels");
    
    //TODO: tile size should be read from the config file 
    m_tileSizeInGameUnits = Vector2(5, 5);
    Vector2 mapSizeInGameUnits(pTileMapData->getWidth() * m_tileSizeInGameUnits.x, pTileMapData->getHeight() * m_tileSizeInGameUnits.y);

    m_pWorld = new World(mapSizeInGameUnits);
    
    //TODO: read assets path from the config file
    m_pAnimatedObjectsFactory = new AnimatedObjectsFactory("assets/objects", pPlatformManager, *m_pDataCacheManager, m_pWorld, m_pViewManager);

    //TODO: tile size should be read from the config file
    m_pTileMapSprite = new TileMapSprite(Vector2(5, 5), pPlatformManager, m_pAnimatedObjectsFactory);
    m_pTileMapSprite->loadMap(pTileMapData, "meta");
    m_pTileMapSprite->setXY(0, 0);
    addChild(m_pTileMapSprite);
    
    Vector2 screenSize = pPlatformManager->getScreenSizeInGameUnits();
    m_pTileMapSprite->setSize(screenSize);
    
    m_pMap = new Map(m_pWorld, m_pTileMapSprite);
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
    m_pPlayer->update(delta);

    Vector2 playerPosition = m_pPlayer->getGamePosition();
    m_pMap->updateCameraPosition(playerPosition);
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
    
    m_pMainMenu->update();
}

void GameView::onClick(const string& rButtonName)
{
    if (strcmp(rButtonName.c_str(), "new_game") != 0)
    {
        m_started = true;
    }
    else if (strcmp(rButtonName.c_str(), "continue") != 0)
    {
        m_started = true;
    }
}
