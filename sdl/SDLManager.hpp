#pragma once
#ifndef SDLManager_hpp
#define SDLManager_hpp

#include <stdio.h>
#include <vector>
#include <map>
#include <string>
#include "SDL.h"
#include "LTexture.hpp"
#include "SDL_mixer.h"
#include "../engine/platform/PlatformManager.h"

using namespace std;

struct RGBA
{
    int r,g,b,a;
};

class SDLManager : public PlatformManager
{
public:
    
    void init(SDL_Renderer* pRenderer, int screenWidth, int screenHeight);
    
    void renderText(const string& labelName, float worldX, float worldY, float scaleX=1, float scaleY=1);
    void renderTexture(const DrawCall& drawCall);
    void setOffsetY(float y);
    
    void renderDebugQuad(float worldX, float worldY, float width, float height, RGBA rgba, bool topToDown=false, bool scale=true);
    
    void renderFillQuad(float worldX, float worldY, float width, float height, RGBA rgba, bool topToDown=false, bool scale=true);
    
    void playSoundEffect(const string& path);
    void playMusic(const string& path);
	void stopSounds();
    
    bool loadMusic(const string& path);
    bool loadSoundEffect(const string& path);
    void loadTexture(const string& path);
    bool loadText(const string& labelName, string text, int red, int green, int blue, int size);
    
    const int getWorldLocationXFromScreenCoordinates(int x) const;
    const int getWorldLocationYFromScreenCoordinates(int y) const;
    
    const GameSize getWorldSizeUnits() const;
    bool shouldScale() const;
    
    static SDLManager* getInstance()
    {
        if(!sm_pInstance)
        {
            sm_pInstance = new SDLManager();
        }
        return sm_pInstance;
    }
    
    SDLManager(SDLManager const&) = delete;
    void operator=(SDLManager const&) = delete;
    
    void setDebugMode(bool debugMode);
    bool isDebugMode() const;
    
	unsigned int getTicks() const;
	
    ~SDLManager();
    
private:
    
    static SDLManager* sm_pInstance;
    
    SDLManager() {
        initializeMembers();
    }
    
    SDL_Renderer* m_pRenderer;
    vector<LTexture*> m_textures;
    vector<string> m_texturesNames;
    map<string,int> m_textureMap;
    map<string,Mix_Chunk*> m_soundEffects;
    map<string,Mix_Music*> m_songs;
    int m_screenWidth;
    int m_screenHeight;
    float m_scaleFactorX;
    float m_scaleFactorY;
    float m_currentOffsetY;
    int m_worldSizeWidthUnits;
    int m_worldSizeHeightUnits;
    bool m_debugMode;
	bool m_sounds;
    
    void initializeMembers()
    {
        m_pRenderer = NULL;
        m_textures.clear();
        m_texturesNames.clear();
        m_textureMap.clear();
        m_soundEffects.clear();
        m_songs.clear();
        m_scaleFactorX = 0.f;
        m_scaleFactorY = 0.f;
        m_worldSizeWidthUnits = 0;
        m_worldSizeHeightUnits = 0;
        m_currentOffsetY = 0;
        m_debugMode = false;
		m_sounds = false;
    }
    
};

#endif /* SDLManager_hpp */
