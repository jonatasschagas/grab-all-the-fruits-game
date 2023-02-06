#pragma once
#ifndef LevelBackground_h
#define LevelBackground_h

#include "core/Sprite.hpp"
#include "core/Vector2.h"
#include "platform/PlatformManager.h"
#include <string>

using namespace std;

class LevelBackground : public Sprite
{
public:

    LevelBackground(const Vector2& tileSizeInGameUnits, PlatformManager* pPlatformManager, const string& textureFileName, const Vector2& textureSize);
    ~LevelBackground();

    void update(float delta) override;
    void render() override;
    
private:

    PlatformManager* m_pPlatformManager;
    string m_textureFileName;
    Sprite* m_pBg1;
    Sprite* m_pBg2;
    
    void initializeMembers()
    {
        m_pPlatformManager = nullptr;
        m_pBg1 = nullptr;
        m_pBg2 = nullptr;
        m_textureFileName = "";
    }

};

#endif //LevelBackground_h