#include "GrabAllTheFruitsGame.hpp"

#include "view/ViewManager.hpp"
#include "views/MainMenuView.hpp"
#include "views/NewGameView.hpp"
#include "views/ContinueGameView.hpp"
#include "imgui/imgui.h"
#include "ui/IconDefs.h"

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
    io.Fonts->AddFontDefault();
    
    static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
    ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
    io.Fonts->AddFontFromFileTTF( "assets/fonts/font.ttf", 16.0f, &icons_config, icons_ranges);
    
    io.Fonts->Build();
}

void GrabAllTheFruitsGame::initialize(PlatformManager* pManager)
{
    m_pPlatformManager = pManager;
    m_pDataCacheManager = new DataCacheManager();
    m_pViewManager = new ViewManager();
    
    loadResources();

    m_pViewManager->initialize(pManager, m_pDataCacheManager);

    NewGameView* pNewGameView = new NewGameView(m_pPlatformManager);
    m_pViewManager->addView("new-game", pNewGameView);
    
    ContinueGameView* pContinueGameView = new ContinueGameView(m_pPlatformManager);
    m_pViewManager->addView("continue-game", pContinueGameView);

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

void GrabAllTheFruitsGame::loadResources()
{
    m_pPlatformManager->loadSoundEffect("jump", "assets/sounds/jump.mp3");
    m_pPlatformManager->loadSoundEffect("trampoline", "assets/sounds/trampoline.mp3");
    m_pPlatformManager->loadSoundEffect("thud", "assets/sounds/thud.wav");
    m_pPlatformManager->loadSoundEffect("collect", "assets/sounds/collect.mp3");
    m_pPlatformManager->loadSoundEffect("death", "assets/sounds/death.wav");
}