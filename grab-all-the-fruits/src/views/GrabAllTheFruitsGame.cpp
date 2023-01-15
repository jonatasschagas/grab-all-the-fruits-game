#include "GrabAllTheFruitsGame.hpp"

#include "view/ViewManager.hpp"
#include "views/GameView.hpp"

GrabAllTheFruitsGame::GrabAllTheFruitsGame()
{
    initializeMembers();
}

GrabAllTheFruitsGame::~GrabAllTheFruitsGame()
{
    delete m_pViewManager;
    delete m_pDataCacheManager;

    initializeMembers();
}

void GrabAllTheFruitsGame::initialize(PlatformManager* pManager)
{
    m_pPlatformManager = pManager;
    m_pDataCacheManager = new DataCacheManager();
    m_pViewManager = new ViewManager();
    
    m_pViewManager->initialize(pManager, m_pDataCacheManager);

    GameView* pGameView = new GameView(m_pPlatformManager);
    m_pViewManager->addView("main", pGameView);
    m_pViewManager->switchView("main");
}

void GrabAllTheFruitsGame::receiveEvent(Event* pEvent)
{
    m_pViewManager->receiveEvent(pEvent);
}

void GrabAllTheFruitsGame::update(const float deltaTime)
{
    m_pViewManager->update(deltaTime);
}

void GrabAllTheFruitsGame::updateEditor(const float deltaTime)
{
    m_pViewManager->updateEditor();
}

void GrabAllTheFruitsGame::render()
{
    m_pViewManager->render();
}