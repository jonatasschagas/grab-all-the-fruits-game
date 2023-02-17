#pragma once
#ifndef ImageButton_hpp
#define ImageButton_hpp

#include <string>
#include "core/Vector2.h"
#include "ButtonClickListener.hpp"
#include "platform/PlatformManager.h"
#include "imgui/imgui.h"

using namespace std;

class ImageButton
{
public:

    ImageButton(
        const string& buttonName, 
        const string& textureName, 
        const Vector2& position, 
        const Vector2& size,
        ButtonClickListener* pButtonClickListener,
        PlatformManager* pPlatformManager);
    ~ImageButton();

    void update();
    void showDebug();

private:

    string m_buttonName;
    string m_textureName;
    Vector2 m_position;
    Vector2 m_size;
    ButtonClickListener* m_pButtonClickListener;
    PlatformManager* m_pPlatformManager;
    intptr_t m_imageTextureID;
    ImVec4 m_bgColor;
    ImVec4 m_tint;

    void initializeMembers()
    {
        m_buttonName = "";
        m_textureName = "";
        m_position = Vector2(0.0f, 0.0f);
        m_size = Vector2(0.0f, 0.0f);
        m_pButtonClickListener = nullptr;
        m_pPlatformManager = nullptr;
        m_bgColor = ImGui::GetStyleColorVec4(ImGuiCol_Button);
        m_tint = ImVec4(1, 1, 1, 1);
    }

};

#endif /* ImageButton_hpp */