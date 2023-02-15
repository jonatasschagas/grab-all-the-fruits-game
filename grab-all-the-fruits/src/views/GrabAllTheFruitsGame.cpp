#include "GrabAllTheFruitsGame.hpp"

#include "view/ViewManager.hpp"
#include "views/MainMenuView.hpp"
#include "views/GameView.hpp"
#include "imgui/imgui.h"

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

void GrabAllTheFruitsGame::initImGui()
{
    ImGuiIO& io = ImGui::GetIO();
    //credits to: https://www.1001freefonts.com/hand-of-gamedevdan.font
    io.Fonts->AddFontFromFileTTF("assets/fonts/Hand_Of_GameDevDan.ttf", 18);
    io.Fonts->AddFontFromFileTTF("assets/fonts/Hand_Of_GameDevDan.ttf", 36);
    io.Fonts->Build();
}

void GrabAllTheFruitsGame::initialize(PlatformManager* pManager)
{
    m_pPlatformManager = pManager;
    m_pDataCacheManager = new DataCacheManager();
    m_pViewManager = new ViewManager();
    
    m_pViewManager->initialize(pManager, m_pDataCacheManager);

    GameView* pGameView = new GameView(m_pPlatformManager);
    m_pViewManager->addView("game", pGameView);
    
    MainMenuView* pMainMenuView = new MainMenuView(m_pPlatformManager);
    m_pViewManager->addView("main-menu", pMainMenuView);
    
    m_pViewManager->switchView("main-menu");
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