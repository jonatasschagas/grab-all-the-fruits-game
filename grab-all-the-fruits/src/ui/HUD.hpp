#pragma once
#ifndef HUD_H
#define HUD_H

#include "event/EventListener.hpp"
#include <string>

using namespace std;

class HUD : public EventListener
{   
public:

    HUD(const int screenWidth, const int screenHeight, EventListener* pEventListener);
    ~HUD();

    void update(float deltaTime);    
    void renderIMGUI();

    void receiveEvent(Event* pEvent) override;

    void reset();

private:

    void renderFruitCounter();
    void renderLevelMessage();

    int m_numFruits;
    bool m_levelCompleted;
    int m_screenWidth;
    int m_screenHeight;

    float m_levelMessageTimer;
    float m_levelMessageTimerMax;
    int m_levelMessageTextCurrentAlpha;
    bool m_isLevelMessageTimerActive;
    string m_currentLevelMessage;

    EventListener* m_pEventListener;

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
        m_currentLevelMessage = "";

        m_pEventListener = nullptr;
    }

};

#endif //HUD_H