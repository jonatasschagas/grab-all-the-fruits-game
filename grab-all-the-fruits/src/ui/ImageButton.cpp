#include "ImageButton.hpp"

#include "imgui/imgui.h"

ImageButton::ImageButton(
    const string& buttonName, 
    const string& textureName, 
    const Vector2& position, 
    const Vector2& size,
    ButtonClickListener* pButtonClickListener,
    PlatformManager* pPlatformManager
)
{
    initializeMembers();

    m_buttonName = buttonName;
    m_textureName = textureName;
    m_position = position;
    m_size = size;
    m_pButtonClickListener = pButtonClickListener;
    m_pPlatformManager = pPlatformManager;

    m_imageTextureID = (intptr_t) m_pPlatformManager->loadTexture(m_textureName);
}

ImageButton::~ImageButton()
{
    initializeMembers();
}

void ImageButton::update()
{
    if (ImGui::ImageButton(m_buttonName.c_str(), (ImTextureID) m_imageTextureID, ImVec2(m_size.x, m_size.y), ImVec2(0, 0), ImVec2(1, 1), m_bgColor, m_tint))
    {
        m_pButtonClickListener->onClick(m_buttonName);
    }
}

void ImageButton::showDebug()
{
    ImGui::Begin(m_buttonName.c_str());
    ImGui::Text("Texture name: %s", m_textureName.c_str());
    ImGui::InputFloat("X", &m_position.x);
    ImGui::InputFloat("Y", &m_position.y);
    ImGui::InputFloat("W", &m_size.x);
    ImGui::InputFloat("H", &m_size.y);
    ImGui::ColorPicker4("Background color", (float *) &m_bgColor);
    ImGui::ColorPicker4("Tint color", (float *) &m_tint);
    ImGui::End();
}
