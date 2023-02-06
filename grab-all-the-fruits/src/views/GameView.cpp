#include "GameView.hpp"

#include "logic/World.hpp"
#include "level/Map.hpp"
#include "level/LevelBackground.hpp"
#include "objects/Player.hpp"
#include "objects/AnimatedObjectsFactory.hpp"
#include "view/ViewManager.hpp"
#include "event/Event.hpp"
#include "platform/PlatformManager.h"
#include "core/TileMapSprite.hpp"
#include "core/Vector2.h"
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
    // remove the tilemap first
    removeChildForced(m_pTileMapSprite);
    
    delete m_pMap;
    delete m_pMainMenu;
    delete m_pWorld;
    
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
        createDisappearingAnimation(pEvent->getInputCoordinates(), m_tileSizeInGameUnits);
    }
    else if (pEvent->getName().compare("player-died") == 0 && !m_died)
    {
        createDisappearingAnimation(pEvent->getInputCoordinates(), m_tileSizeInGameUnits);
        m_pPlayer->destroy();
        m_died = true;
    }   
    else if (pEvent->getName().compare("debug_toggle") == 0)
    {
        m_debug = !m_debug;
    }   
    else if (!m_died)
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

    m_pWorld = new World(mapSizeInGameUnits, pPlatformManager);
    
    //TODO: read assets path from the config file
    m_pAnimatedObjectsFactory = new AnimatedObjectsFactory("assets/objects", pPlatformManager, *m_pDataCacheManager, m_pWorld, m_pViewManager);

    // creating the background
    // TODO: read background path from the config file, along with the texture size and file path
    LevelBackground* pLevelBackground = new LevelBackground(Vector2(10, 10), pPlatformManager, "assets/levels/backgrounds/Brown.png", Vector2(64, 64));
    pLevelBackground->setXY(0, 0);
    addChild(pLevelBackground);
    pLevelBackground->fillParent();

    //TODO: tile size should be read from the config file
    m_pTileMapSprite = new TileMapSprite(m_tileSizeInGameUnits, pPlatformManager, m_pAnimatedObjectsFactory);
    m_pTileMapSprite->loadMap(pTileMapData, "meta");
    m_pTileMapSprite->setXY(0, 0);
    m_pTileMapSprite->setPivotAtCenter(true);
    addChild(m_pTileMapSprite);
    m_pTileMapSprite->fillParent();
    
    m_pMap = new Map(m_pWorld, m_pTileMapSprite);
}

void GameView::render()
{
    if (!m_started)
    {
        return;
    }   

    Sprite::render();
    
    if (m_debug)
    {
        m_pWorld->renderDebug(m_pTileMapSprite->getGamePosition());
    }
}

void GameView::update(float delta)
{   
    Sprite::update(delta);
    
    m_pWorld->update(delta);

    if (!m_died)
    {
        m_pPlayer->update(delta);
        Vector2 playerPosition = m_pPlayer->getGamePosition();
        m_pMap->updateCameraPosition(playerPosition);
    }
}

void GameView::readInput(int x, int y, bool pressed)
{
}

void GameView::updateEditor()
{
    if (m_started)
    {
        if (m_debug)
        {
            m_pPlayer->updateEditor();
        }
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

void GameView::createDisappearingAnimation(const Vector2& position, const Vector2& size)
{
    AnimatedObject* pAnimatedObj = m_pAnimatedObjectsFactory->createDisappearingAnimation(position, size);
    pAnimatedObj->play("idle");
    m_pTileMapSprite->addChild(pAnimatedObj);
}