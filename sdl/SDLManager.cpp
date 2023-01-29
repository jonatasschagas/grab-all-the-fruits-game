#include "SDL_image.h"
#include <math.h>
#include "SDLManager.hpp"
#include "SDL_mixer.h"
#include "glm/glm.hpp"
#include <glm/gtx/transform.hpp>

SDLManager* SDLManager::sm_pInstance = 0;

void SDLManager::init(SDL_Renderer* pRenderer, int screenWidth, int screenHeight)
{
    m_pRenderer = pRenderer;
    
    m_screenWidth = screenWidth;
    m_screenHeight = screenHeight;
    
    float aspectRatio = screenWidth / (screenHeight * 1.f);
    
    m_screenWidthInGameUnits = 100;
    // the number of vertical units depends on the aspect ratio from the device
    m_screenHeightInGameUnits = m_screenWidthInGameUnits / aspectRatio;
    
    m_scaleFactorX = (screenWidth * 1.0f) /m_screenWidthInGameUnits;
    m_scaleFactorY = (screenHeight * 1.0f) /m_screenHeightInGameUnits;

}

void SDLManager::renderText(const string& labelName, float worldX, float worldY, float scaleX, float scaleY)
{
    map<string,int>::iterator it = m_textureMap.find(labelName);
    if(it == m_textureMap.end())
    {
        //element not found;
        return;
    }
    
    const int index = m_textureMap.at(labelName);
    LTexture* pTexture = m_textures.at(index);
    
    float width = pTexture->getWidth();
    float height = pTexture->getHeight();
    
    SDL_Rect textureClip;
    textureClip.x = 0;
    textureClip.y = 0;
    textureClip.w = width;
    textureClip.h = height;
    
    // scaling to the size of the world
    worldX = worldX * m_scaleFactorX;
    worldY = ((m_screenHeightInGameUnits - worldY) * m_scaleFactorY) - height;
    
    // render the sprite
    pTexture->render(m_pRenderer, worldX, worldY, &textureClip, width, height, false);
}

void SDLManager::renderTexture(const DrawCall& drawCall)
{
    
    if (drawCall.debug)
    {
        RGBA debugQuadColor;
        if (drawCall.settings.tileMap)
        {
            // green
            debugQuadColor.r = 0;
            debugQuadColor.g = 255;
            debugQuadColor.b = 0;
            debugQuadColor.a = 255;
        }
        else
        {
            // red
            debugQuadColor.r = 255;
            debugQuadColor.g = 0;
            debugQuadColor.b = 0;
            debugQuadColor.a = 255;
        }

        renderDebugQuad(drawCall.spriteProperties.x,
                        drawCall.spriteProperties.y,
                        drawCall.spriteProperties.w,
                        drawCall.spriteProperties.h,
                        debugQuadColor,
                        drawCall.settings.topToDown,
                        drawCall.settings.scale);
        return;
    }
    
    if (drawCall.colored)
    {
        RGBA quadColor;
        quadColor.r = drawCall.color.r;
        quadColor.g = drawCall.color.g;
        quadColor.b = drawCall.color.b;
        quadColor.a = drawCall.color.a;

        renderFillQuad(drawCall.spriteProperties.x,
                       drawCall.spriteProperties.y,
                       drawCall.spriteProperties.w,
                       drawCall.spriteProperties.h,
                       quadColor,
                       drawCall.settings.topToDown,
                       drawCall.settings.scale);
        return;
    }
    
    string path = drawCall.textureSettings.name;
    
    SDL_Rect textureClip;
    textureClip.x = drawCall.textureSettings.x;
    textureClip.y = drawCall.textureSettings.y;
    textureClip.w = drawCall.textureSettings.w;
    textureClip.h = drawCall.textureSettings.h;
    
    const int index = m_textureMap.at(path);
    LTexture* pTexture = m_textures.at(index);
    
    float width = drawCall.spriteProperties.w;
    float height = drawCall.spriteProperties.h;
    float worldX = drawCall.spriteProperties.x;
    float worldY = drawCall.spriteProperties.y;
    
    worldY += m_currentOffsetY;
    
    if (drawCall.settings.scale)
    {
        width = width * m_scaleFactorX;
        height = height * m_scaleFactorY;
    }
    
    // scaling to the size of the world
    worldX = worldX * m_scaleFactorX;
    worldY = fmax((worldY * m_scaleFactorY) - height,0);
    
    // applying alpha
    int alpha = drawCall.textureSettings.alpha * 255;
    pTexture->setAlpha(alpha);
    
    // render the sprite
    pTexture->render(m_pRenderer,
                     worldX,
                     worldY,
                     &textureClip,
                     width,
                     height,
                     0,
                     drawCall.settings.flipHorizontal);
    
}

void SDLManager::renderDebugQuad(float worldX, float worldY, float width, float height, RGBA rgba, bool topToDown, bool scale)
{
    if (m_debugMode)
    {
		worldY += m_currentOffsetY;

		if (scale)
		{
			width = width * m_scaleFactorX;
			height = height * m_scaleFactorY;
		}

		// scaling to the size of the world
		worldX = worldX * m_scaleFactorX;

		if (topToDown)
		{
			worldY = fmax((worldY * m_scaleFactorY) - height,0);
		}
		else
		{
			// 1. (m_screenHeightInGameUnits - worldY) -> coordinates start from the top (0 is top and m_screenHeightInGameUnits is bottom.
			// in order to have a system where the bottom starts with zero we do this -> (m_screenHeightInGameUnits - worldY)
			// 2. We substract the height here in order to put the pivot from the sprite to the bottom left corner (default is top left corner)
			worldY = ((m_screenHeightInGameUnits - worldY) * m_scaleFactorY) - height;
		}
        
        SDL_FRect outlineRect = { worldX, worldY, width, height };
        SDL_SetRenderDrawColor( m_pRenderer, rgba.r, rgba.g, rgba.b, rgba.a);
        SDL_RenderDrawRectF( m_pRenderer, &outlineRect );
    }
}

void SDLManager::renderFillQuad(float worldX, float worldY, float width, float height, RGBA rgba, bool topToDown, bool scale)
{
    worldY += m_currentOffsetY;

    if (scale)
    {
        width = width * m_scaleFactorX;
        height = height * m_scaleFactorY;
    }

    // scaling to the size of the world
    worldX = worldX * m_scaleFactorX;

    if (topToDown)
    {
        worldY = fmax((worldY * m_scaleFactorY) - height,0);
    }
    else
    {
        // 1. (m_screenHeightInGameUnits - worldY) -> coordinates start from the top (0 is top and m_screenHeightInGameUnits is bottom.
        // in order to have a system where the bottom starts with zero we do this -> (m_screenHeightInGameUnits - worldY)
        // 2. We substract the height here in order to put the pivot from the sprite to the bottom left corner (default is top left corner)
        worldY = ((m_screenHeightInGameUnits - worldY) * m_scaleFactorY) - height;
    }

    SDL_FRect outlineRect = { worldX, worldY, width, height };
    SDL_SetRenderDrawColor( m_pRenderer, rgba.r, rgba.g, rgba.b, rgba.a);
    SDL_RenderFillRectF( m_pRenderer, &outlineRect );
}

void SDLManager::loadTexture(const string& path)
{
    
    if (!m_pRenderer)
    {
        printf( "SDLManager has not been initialized yet! SDL_Renderer is null. !\n" );
        return;
    }
    
    map<string,int>::iterator itTexMap = m_textureMap.find(path);
    if(itTexMap != m_textureMap.end())
    {
        // texture already loaded
        return;
    }
    
    LTexture* pNewTexture = new LTexture();

    //Loading success flag
    bool success = true;
    
    //Load sprite sheet texture
    if(!pNewTexture->loadFromFile(m_pRenderer, path))
    {
        printf( "Failed to load texture!\n" );
        success = false;
    }
    else
    {
        m_textures.push_back(pNewTexture);
        m_texturesNames.push_back(path);
        const int index = (int)m_textures.size() - 1;
        m_textureMap[path] = index;
    }
}

SDLManager::~SDLManager()
{
    for (int i = 0; i < m_textures.size(); i++)
    {
        auto pSDLTex = m_textures.at(i);
        pSDLTex->free();
        pSDLTex = nullptr;
    }
    
    for( auto it = m_songs.begin(); it != m_songs.end(); ++it )
    {
        string path = it->first;
        auto pMUS = it->second;
        Mix_FreeMusic(pMUS);
        pMUS = nullptr;
    }
    
    for( auto it = m_soundEffects.begin(); it != m_soundEffects.end(); ++it )
    {
        string path = it->first;
        auto pSEffc = it->second;
        Mix_FreeChunk(pSEffc);
        pSEffc = nullptr;
    }
    
    initializeMembers();
}

const int SDLManager::getWorldLocationXFromScreenCoordinates(int x) const
{
    return x/m_scaleFactorX;
}

const int SDLManager::getWorldLocationYFromScreenCoordinates(int y) const
{
    return m_screenHeightInGameUnits - y/m_scaleFactorY;
}

bool SDLManager::isDebugMode() const
{
    return m_debugMode;
}

void SDLManager::setDebugMode(bool debugMode)
{
    m_debugMode = debugMode;
}

const Vector2 SDLManager::getScreenSizeInGameUnits() const
{
    return Vector2(m_screenWidthInGameUnits, m_screenHeightInGameUnits);
}

void SDLManager::playSoundEffect(const string& path)
{
	if (!m_sounds)
	{
		return;
	}
    
    map<string,Mix_Chunk*>::iterator it = m_soundEffects.find(path);
    if(it == m_soundEffects.end())
    {
        //not found
        return;
    }
    
    Mix_Chunk* pSoundEffect = m_soundEffects.at(path);
    Mix_PlayChannel( -1, pSoundEffect, 0 );
}

void SDLManager::playMusic(const string& path)
{
    if (!m_sounds)
    {
        return;
    }
    
    map<string,Mix_Music*>::iterator it = m_songs.find(path);
    if(it == m_songs.end())
    {
        //element not found;
        return;
    }
    
    Mix_Music* pMusic = m_songs.at(path);
    Mix_PlayMusic(pMusic, -1);
    Mix_VolumeMusic(25);
}

bool SDLManager::loadSoundEffect(const string& path)
{
    map<string,Mix_Chunk*>::iterator it = m_soundEffects.find(path);
    if(it != m_soundEffects.end())
    {
        //already loaded;
        return true;
    }
    
    bool success = true;
    Mix_Chunk* pSound = Mix_LoadWAV(path.c_str());
    if( pSound == NULL )
    {
        printf( "Failed to load sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }
    else
    {
        m_soundEffects[path] = pSound;
    }
    
    return success;
}

bool SDLManager::loadMusic(const string& path)
{
    bool success = true;
    Mix_Music* pMusic = Mix_LoadMUS(path.c_str());
    if( pMusic == NULL )
    {
        printf( "Failed to load music! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }
    else
    {
        m_songs[path] = pMusic;
    }
    
    return success;
}

unsigned int SDLManager::getTicks() const
{
	return SDL_GetTicks();
}

void SDLManager::stopSounds()
{
	Mix_PauseMusic();
	m_sounds = false;
}

bool SDLManager::shouldScale() const
{
    return true;
}

void SDLManager::setOffsetY(float y)
{
    m_currentOffsetY = y;
}

int SDLManager::getScreenWidth() const
{
    return m_screenWidth;
}

int SDLManager::getScreenHeight() const
{
    return m_screenHeight;
}