#include "MainMenu.hpp"

#include "imgui/imgui.h"

MainMenu::MainMenu(const int screenWidth, const int screenHeight)
{
    initializeMembers();

    m_screenWidth = screenWidth;
    m_screenHeight = screenHeight;
}

MainMenu::~MainMenu()
{  
    initializeMembers();
}

void MainMenu::update()
{
    if (m_showCredits)
    {
        // window in the center
        ImGui::SetNextWindowPos(ImVec2(m_screenWidth * 0.5f, m_screenHeight * 0.5f), ImGuiCond_::ImGuiCond_Always, ImVec2(0.5f,0.5f));

        ImGui::Begin("Credits", nullptr, 
        ImGuiWindowFlags_::ImGuiWindowFlags_NoResize | 
        ImGuiWindowFlags_::ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar);
    
        ImGui::Text("CREDITS:");
        ImGui::Text("Game by: Jonatas Chagas @jonatasschagas");
        //ImGui::Text("Music by: @josephgattuso");
        ImGui::Text("Art by: https://pixelfrog-assets.itch.io/");
        ImGui::Text("Programming by: Jonatas Chagas @jonatasschagas");
        //ImGui::Text("Sound effects by: @josephgattuso");
        ImGui::Text("Built with:");
        ImGui::Text(" - SDL2");
        ImGui::Text(" - IMGUI");
        ImGui::Text(" - Box2d");
        ImGui::Text(" - Rapidjson");
        ImGui::Text("Special thanks to:");
        ImGui::Text(" - LazyFoo -> https://lazyfoo.net (SDL2 tutorials)");
        ImGui::Text(" - Erin Catto -> Box2d");
        ImGui::Text(" - Pixelfrog -> Art");
        ImGui::Text(" - IForce -> https://www.iforce2d.net/ (Box2d tutorials)");
        ImGui::Text(" - Tiled - flexible map editor -> https://www.mapeditor.org/");
        ImGui::Text(" - Nacho Lasheras -> https://nacholasheras.com/ (Friend and mentor)");
        ImGui::Text(" - Antti Mattila -> (Friend and mentor)");
        ImGui::Text(" - Patrick Corander -> (The first person to teach me how games work)");
        
        ImGui::SetScrollY(ImGui::GetScrollMaxY());
        ImGui::SetScrollY(m_creditsScrollY);
        
        if (ImGui::GetScrollY() < ImGui::GetScrollMaxY())
        {
            m_creditsScrollY += 0.1f;
        }
        
        if (ImGui::Button("Back")) 
        {
            m_showCredits = false;
            m_creditsScrollY = 0;
        }

        ImGui::End();
    }
    else
    {
        
        // window on the right side
        ImGui::SetNextWindowPos(ImVec2(m_screenWidth * 0.9f, m_screenHeight * 0.5f), ImGuiCond_::ImGuiCond_Always, ImVec2(0.5f,0.5f));

        ImGui::Begin("Grab All The Fruits!", nullptr, 
        ImGuiWindowFlags_::ImGuiWindowFlags_NoResize | 
        ImGuiWindowFlags_::ImGuiWindowFlags_NoMove | 
        ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar | 
        ImGuiWindowFlags_::ImGuiWindowFlags_NoBackground);

        ImGui::SetWindowSize(ImVec2(200, 200));
        ImGui::SetWindowFontScale(1.5f);

        if (ImGui::Button("New Game", ImVec2(100, 50))) 
        {
            m_pButtonClickListener->onClick("new_game");
        }
        else if (ImGui::Button("Continue", ImVec2(100, 50))) 
        {
            m_pButtonClickListener->onClick("continue");
        }
        else if (ImGui::Button("Credits", ImVec2(100, 50))) 
        {
            m_showCredits = true;
        }        
        
        ImGui::End();
    }
}

void MainMenu::setButtonClickListener(ButtonClickListener* pButtonClickListener)
{
    m_pButtonClickListener = pButtonClickListener;
}
