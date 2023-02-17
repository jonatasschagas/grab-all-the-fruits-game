#include "GameView.hpp"

#include "logic/World.hpp"
#include "objects/Player.hpp"
#include "objects/AnimatedObjectsFactory.hpp"
#include "view/ViewManager.hpp"
#include "event/Event.hpp"
#include "platform/PlatformManager.h"
#include "core/Vector2.h"
#include "core/Sprite.hpp"
#include "logic/GameConfiguration.h"

#ifdef __APPLE__
    #include <TargetConditionals.h>
#endif

GameView::GameView(PlatformManager* pPlatformManager) : View(pPlatformManager)
{
    initializeMembers();
}

GameView::~GameView()
{
    delete m_pLevelManager;
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
   
    m_pHUD = new HUD(m_pViewManager->getPlatformManager()->getScreenWidth(), m_pViewManager->getPlatformManager()->getScreenHeight(), m_pViewManager, pPlatformManager);
}


void GameView::receiveEvent(Event* pEvent)
{
    m_pWorld->receiveEvent(pEvent);

    if (pEvent->getName().compare("create-player") == 0)
    {
        m_playerStartPosition = pEvent->getInputCoordinates();
        createPlayer(m_playerStartPosition);
    }
    else if (pEvent->getName().compare("fruit-collected") == 0)
    {
        playSoundEffect("collect");
        AnimatedObject* pDisappearingAnimation = createDisappearingAnimation(pEvent->getInputCoordinates(), m_pLevelManager->getTileSize());
         pDisappearingAnimation->setOnAnimationFinishedCallback("idle", [this, pDisappearingAnimation]() {
            pDisappearingAnimation->destroy();
        });
    }
    else if (pEvent->getName().compare("player-died") == 0 && !m_died)
    {
        playSoundEffect("death");
        AnimatedObject* pDisappearingAnimation = createDisappearingAnimation(pEvent->getInputCoordinates(), m_pLevelManager->getTileSize());
        m_pPlayer->destroy();
        pDisappearingAnimation->setOnAnimationFinishedCallback("idle", [this, pDisappearingAnimation]() {
            createPlayer(m_playerStartPosition);
            pDisappearingAnimation->destroy();
        });
        m_died = true;
    }   
    else if (pEvent->getName().compare("waypoint-reached") == 0)
    {
        string waypointName = pEvent->getTarget();
        if (waypointName.compare("end-level") == 0)
        {
            if (m_pLevelManager->hasCompletedLevel())
            {
                Event showEndLevelScreen("show-end-level-screen");
                m_pViewManager->receiveEvent(&showEndLevelScreen);
            }
            else
            {
                Event showEndLevelScreen("show-end-level-screen-failed");
                m_pViewManager->receiveEvent(&showEndLevelScreen);
            }
        }
    }
    else if (pEvent->getName().compare("debug_toggle") == 0)
    {
        m_debug = !m_debug;
    }
    else if (pEvent->getName().compare("next-level") == 0)
    {
        m_currentLevel++;
        initGame();
        m_skipNextEditorUpdate = true;
        return;
    }   
    else if (pEvent->getName().compare("load-level") == 0)
    {
        m_currentLevel = pEvent->getData();
        initGame();
        m_skipNextEditorUpdate = true;
        return;
    }   
    else if (pEvent->getName().compare("restart-level") == 0)
    {
        initGame();
        m_skipNextEditorUpdate = true;
    }
    else if (pEvent->getName().compare("quit") == 0)
    {
        m_pViewManager->switchView("main-menu");
    }
    
    if (!m_died && m_pPlayer != nullptr)
    {
        m_pPlayer->receiveEvent(pEvent);
    }

    if (m_pHUD != nullptr)
    {
        m_pHUD->receiveEvent(pEvent);
    }

    if (m_pLevelManager != nullptr)
    {
        m_pLevelManager->receiveEvent(pEvent);
    }
}

void GameView::initGame()
{
    removeAllChildren();

    delete m_pLevelManager;
    m_pLevelManager = nullptr;
    delete m_pWorld;
    m_pWorld = nullptr;
    delete m_pAnimatedObjectsFactory;
    m_pAnimatedObjectsFactory = nullptr;
    m_pPlayer = nullptr;
    m_debug = false;

    m_pHUD->reset();

    PlatformManager* pPlatformManager = m_pViewManager->getPlatformManager();
    
    m_pWorld = new World(pPlatformManager);
    
    m_pAnimatedObjectsFactory = new AnimatedObjectsFactory(OBJECTS_PATH, pPlatformManager, *m_pDataCacheManager, m_pWorld, m_pViewManager);

    m_pLevelManager = new LevelManager(LEVELS_FILE, static_cast<Sprite*>(this), m_pWorld, m_pAnimatedObjectsFactory, m_pViewManager);

    m_pAnimatedObjectsFactory->setLevelManager(m_pLevelManager);

    m_pLevelManager->loadLevel(m_currentLevel);

    m_initialized = true;
}

void GameView::render()
{
    Sprite::render();
    
    if (m_debug)
    {
        m_pWorld->renderDebug(m_pLevelManager->getTileMapPosition());
    }
}

void GameView::update(float delta)
{   
    Sprite::update(delta);
    
    m_pWorld->update(delta);

    if (!m_died && m_pPlayer != nullptr)
    {
        m_pPlayer->update(delta);
        Vector2 playerPosition = m_pPlayer->getGamePosition();
        m_pLevelManager->updateCameraPosition(playerPosition);
    }

    m_pHUD->setDebugMode(m_debug);
    m_pHUD->update(delta);
    m_pLevelManager->update(delta);
}

void GameView::readInput(int x, int y, bool pressed)
{
}

void GameView::updateEditor()
{
    if (m_skipNextEditorUpdate)
    {
        m_skipNextEditorUpdate = false;
        return;
    }

    if (m_debug && m_pPlayer)
    {
        m_pPlayer->updateEditor();
        m_pLevelManager->updateEditor();
    }

    m_pHUD->renderIMGUI();
}

void GameView::onClick(const string& rButtonName)
{
}

AnimatedObject* GameView::createDisappearingAnimation(const Vector2& position, const Vector2& size)
{
    AnimatedObject* pAnimatedObj = m_pAnimatedObjectsFactory->createDisappearingAnimation(position, size);
    pAnimatedObj->play("idle");
    m_pLevelManager->addSpriteToTileMap(static_cast<Sprite*>(pAnimatedObj));
    return pAnimatedObj;
}

void GameView::createPlayer(const Vector2& position)
{
    m_pPlayer = m_pAnimatedObjectsFactory->createPlayer(m_pLevelManager->getCurrentCharacter() ,position, m_pLevelManager->getTileSize());
    m_pLevelManager->addSpriteToTileMap(static_cast<Sprite*>(m_pPlayer));
    m_died = false;   
}

void GameView::setCurrentLevel(int level)
{
    m_currentLevel = level;
}
