#include "MainMenu.hpp"

#if IMGUI_ENABLED
#include "imgui/imgui.h"
#endif

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
    #if IMGUI_ENABLED
    // center the window
    ImGui::SetNextWindowPos(ImVec2(m_screenWidth * 0.5f, m_screenHeight * 0.5f), ImGuiCond_::ImGuiCond_Always, ImVec2(0.5f,0.5f));

    if (m_showCredits)
    {
        ImGui::Begin("Credits", nullptr, 
        ImGuiWindowFlags_::ImGuiWindowFlags_NoResize | 
        ImGuiWindowFlags_::ImGuiWindowFlags_NoMove | 
        ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar | 
        ImGuiWindowFlags_::ImGuiWindowFlags_NoBackground);
    
        ImGui::Text("Credits");
        ImGui::Text("Game by: @jonatasschagas");
        //ImGui::Text("Music by: @josephgattuso");
        ImGui::Text("Art by: https://pixelfrog-assets.itch.io/");
        ImGui::Text("Programming by: @jonatasschagas");
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
        ImGui::Text(" - Nacho Lasheras -> https://nacholasheras.com/ (Friend and mentor)");
        
        if (ImGui::Button("Back")) 
        {
            m_showCredits = false;
        }

        ImGui::End();
    }
    else
    {
        ImGui::Begin("Grab All The Fruits!", nullptr, 
        ImGuiWindowFlags_::ImGuiWindowFlags_NoResize | 
        ImGuiWindowFlags_::ImGuiWindowFlags_NoMove | 
        ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar | 
        ImGuiWindowFlags_::ImGuiWindowFlags_NoBackground);
    
        if (ImGui::Button("New Game")) 
        {
            m_pButtonClickListener->onClick("new_game");
        }
        else if (ImGui::Button("Continue")) 
        {
            m_pButtonClickListener->onClick("continue");
        }
        else if (ImGui::Button("Credits")) 
        {
            m_showCredits = true;
        }        
        
        ImGui::End();
    }

#endif
}

void MainMenu::setButtonClickListener(ButtonClickListener* pButtonClickListener)
{
    m_pButtonClickListener = pButtonClickListener;
}
