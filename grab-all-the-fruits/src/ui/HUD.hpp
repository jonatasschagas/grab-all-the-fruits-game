#pragma once
#ifndef HUD_H
#define HUD_H

#include "event/EventListener.hpp"
#include "ButtonClickListener.hpp"
#include "platform/PlatformManager.h"
#include <string>

using namespace std;

class HUD : public EventListener, public ButtonClickListener
{   
public:

    HUD(const int screenWidth, const int screenHeight, EventListener* pEventListener, PlatformManager* pPlatformManager);
    ~HUD();

    void update(float deltaTime);    
    void renderIMGUI();

    void setDebugMode(bool debugMode);

    void receiveEvent(Event* pEvent) override;

    void reset();

    void onClick(const string& buttonName) override;

private:

    void renderFruitCounter();
    void renderLevelMessage();
    void renderPauseMenu();

    int m_numFruits;
    bool m_levelCompleted;
    int m_screenWidth;
    int m_screenHeight;

    float m_levelMessageTimer;
    float m_levelMessageTimerMax;
    int m_levelMessageTextCurrentAlpha;
    bool m_isLevelMessageTimerActive;
    string m_currentLevelMessage;

    bool m_pauseMenuOpen;

    EventListener* m_pEventListener;
    PlatformManager* m_pPlatformManager;
    intptr_t m_pauseIconTextureID;
    bool m_debugMode;

    void initializeMembers()
    {
        m_numFruits = 0;
        m_screenWidth = 0;
        m_screenHeight = 0;

        m_levelMessageTimer = 0.0f;
        m_levelMessageTimerMax = 3.0f;
        m_levelMessageTextCurrentAlpha = 255;
        m_isLevelMessageTimerActive = false;
        m_levelCompleted = false;
        m_pauseMenuOpen = false;
        m_currentLevelMessage = "";

        m_pEventListener = nullptr;
        m_pPlatformManager = nullptr;
        m_pauseIconTextureID = 0;

        m_debugMode = false;
    }

};

#endif //HUD_H