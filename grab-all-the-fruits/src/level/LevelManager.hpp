#pragma once
#ifndef LevelManager_hpp
#define LevelManager_hpp

#include <string>
#include <vector>
#include "core/Sprite.hpp"
#include "core/TileMapSprite.hpp"
#include "core/Vector2.h"
#include "event/EventListener.hpp"
#include "LevelBackground.hpp"
#include "Map.hpp"
#include "logic/World.hpp"
#include "objects/AnimatedObjectsFactory.hpp"

using namespace std;

struct Platform
{
    int tileX;
    int tileY;
    int targetTileX;
    int targetTileY;
    int widthInTiles;
    int heightInTiles;
};

struct LevelData
{
    string title;
    string configFile;
    string background;
    Vector2 backgroundTileSize;
    Vector2 backgroundTextureSize;
    int numFruits;
    vector<Platform> platforms;
};

class LevelManager
{
public:
    LevelManager(const string& levelsFile, Sprite* pStage, World* pWorld, AnimatedObjectsFactory* pAnimatedObjectsFactory, EventListener* pEventListener);
    ~LevelManager();
    
    void loadLevel(const int levelindex);
    void updateEditor();
    void addSpriteToTileMap(Sprite* pSprite);
    void updateCameraPosition(const Vector2& rCameraPosition);

    const Vector2& getTileSize() const { return m_tileSize; }
    const Vector2 getTileMapSize() const;
    Vector2 getTileMapPosition() const;

    const bool hasCompletedLevel(const int numFruitsCollected) const;

    const Platform* findPlatform(int tileX, int tileY) const;

private:

    void loadLevelsData(const string& levelsFile);

    Sprite* m_pStage; // main sprite
    TileMapSprite* m_pCurrentLevelTileMap; // tile map sprite
    LevelBackground* m_pCurrentLevelBackground; // background sprite
    Map* m_pMap; // map
    World* m_pWorld; // physics world
    AnimatedObjectsFactory* m_pAnimatedObjectsFactory; // animated objects factory
    vector<LevelData> m_levels;
    EventListener* m_pEventListener;
    int m_currentLevelIndex;

    Vector2 m_tileSize;
    string m_levelsFolder;
    string m_tilesetsFolder;

    bool m_initialized;

    void initializeMembers()
    {
        m_pStage = nullptr;
        m_pCurrentLevelTileMap = nullptr;
        m_pCurrentLevelBackground = nullptr;
        m_pWorld = nullptr;
        m_pMap = nullptr;
        m_pAnimatedObjectsFactory = nullptr;
        m_pEventListener = nullptr;
        m_levelsFolder = "";
        m_tilesetsFolder = "";
        m_currentLevelIndex = 0;
        m_levels.clear();
        m_initialized = false;

        m_tileSize.x = 0;
        m_tileSize.y = 0;
    }

};

#endif /* LevelManager_hpp */