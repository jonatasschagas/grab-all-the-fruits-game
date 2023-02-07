#include "HUD.hpp"

#include "utils/StringUtils.h"
#include "objects/Player.hpp"

#include "imgui/imgui.h"

HUD::HUD(int screenWidth, int screenHeight, EventListener* pEventListener)
{
    initializeMembers();

    m_screenWidth = screenWidth;
    m_screenHeight = screenHeight;
    m_pEventListener = pEventListener;
}

HUD::~HUD()
{
    initializeMembers();
}

void HUD::update(float deltaTime)
{
    if (m_isLevelMessageTimerActive && m_levelMessageTimerMax != -1)
    {
        m_levelMessageTimer += deltaTime;
        if (m_levelMessageTimer > m_levelMessageTimerMax)
        {
            m_isLevelMessageTimerActive = false;
            m_levelMessageTimer = 0.0f;
            m_levelMessageTextCurrentAlpha = 0;
        }
        else
        {
            m_levelMessageTextCurrentAlpha = 255 - (m_levelMessageTimer / m_levelMessageTimerMax) * 255;
        }
    }
}

void HUD::renderIMGUI()
{
    renderFruitCounter();
    renderLevelMessage();
}

void HUD::receiveEvent(Event* pEvent)
{
    if (pEvent->getName().compare("update-fruit-collected-hud") == 0)
    {
        m_numFruits = pEvent->getData();
    }
    
    if (pEvent->getName().compare("level-start") == 0)
    {
        m_currentLevelMessage = pEvent->getTarget();
        m_isLevelMessageTimerActive = true;
        //TODO: parameterize this in a config file
        m_levelMessageTimerMax = 50;
    }
    else if (pEvent->getName().compare("show-end-level-screen") == 0)
    {
        m_currentLevelMessage = "Level complete!";
        m_isLevelMessageTimerActive = true;
        m_levelMessageTextCurrentAlpha = 255;
        m_levelCompleted = true;
        m_levelMessageTimerMax = -1;
    }
    else if (pEvent->getName().compare("show-end-level-screen-failed") == 0)
    {
        m_currentLevelMessage = "Collect all the fruits first!";
        m_isLevelMessageTimerActive = true;
        m_levelMessageTimerMax = 50;
    }
}

void HUD::renderLevelMessage()
{
    if (m_isLevelMessageTimerActive)
    {
        ImGui::SetNextWindowPos(ImVec2(m_screenWidth * 0.5f, m_screenHeight * 0.5f), ImGuiCond_::ImGuiCond_Always, ImVec2(0.5f,0.5f));
        ImGui::Begin("Level message", nullptr, 
        ImGuiWindowFlags_::ImGuiWindowFlags_AlwaysAutoResize | 
        ImGuiWindowFlags_::ImGuiWindowFlags_NoMove | 
        ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar | 
        ImGuiWindowFlags_::ImGuiWindowFlags_NoBackground);

        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, m_levelMessageTextCurrentAlpha));
        ImGui::SetWindowFontScale(2.5f);
        ImGui::TextUnformatted(m_currentLevelMessage.c_str());
        ImGui::PopStyleColor();

        if (m_levelCompleted)
        {
            ImGui::SetWindowFontScale(1.5f);
            if (ImGui::Button("Next level", ImVec2(200, 50)))
            {
                Event nextLevelEvent("next-level");
                m_pEventListener->receiveEvent(&nextLevelEvent);
            }
        }

        ImGui::End();
    }
}

void HUD::renderFruitCounter()
{
    // put window on the upper right corner
    ImGui::SetNextWindowPos(ImVec2(m_screenWidth * 0.85f, m_screenHeight * 0.1f), ImGuiCond_::ImGuiCond_Always, ImVec2(0.5f,0.5f));

    ImGui::Begin("Status Bar", nullptr, 
    ImGuiWindowFlags_::ImGuiWindowFlags_AlwaysAutoResize | 
    ImGuiWindowFlags_::ImGuiWindowFlags_NoMove | 
    ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar | 
    ImGuiWindowFlags_::ImGuiWindowFlags_NoBackground);

    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32_WHITE);
    ImGui::SetWindowFontScale(1.5f);
    ImGui::Text("Fruits: %d", m_numFruits);
    ImGui::PopStyleColor();
    ImGui::End();
}

void HUD::reset()
{
    m_numFruits = 0;
    m_levelMessageTimer = 0.0f;
    m_levelMessageTimerMax = 3.0f;
    m_levelMessageTextCurrentAlpha = 255;
    m_isLevelMessageTimerActive = false;
    m_levelCompleted = false;
    m_currentLevelMessage = "";
}
