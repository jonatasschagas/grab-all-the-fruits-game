#include "GameView.hpp"

#include "characters/Masked.hpp"
#include "view/ViewManager.hpp"
#include "event/Event.hpp"
#include "platform/PlatformManager.h"

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

}

void GameView::initGame()
{
    PlatformManager* pPlatformManager = m_pViewManager->getPlatformManager();
    // creating the player   
    m_pPlayer = new Masked(pPlatformManager, *m_pDataCacheManager);
    // setting the view as a sprite that covers the whole screen
    m_pPlayer->setXY(10, 10);
    m_pPlayer->setSize(12, 12);
    addChild(m_pPlayer);
}

void GameView::render()
{
    Sprite::render();
}

void GameView::update(float delta)
{   
    Sprite::update(delta);
}

void GameView::readInput(int x, int y, bool pressed)
{
}

void GameView::updateEditor()
{
#if TARGET_OS_OSX
#endif
}
