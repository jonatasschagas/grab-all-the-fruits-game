#pragma once
#ifndef LevelManager_hpp
#define LevelManager_hpp

#include <string>
#include <vector>
#include "core/Sprite.hpp"
#include "core/TileMapSprite.hpp"
#include "core/Vector2.h"
#include "LevelBackground.hpp"
#include "Map.hpp"
#include "logic/World.hpp"
#include "objects/AnimatedObjectsFactory.hpp"

using namespace std;

struct LevelData
{
    string title;
    string configFile;
    string background;
    Vector2 backgroundTileSize;
    Vector2 backgroundTextureSize;
};

class LevelManager
{
public:
    LevelManager(const string& levelsFile, Sprite* pStage, World* pWorld, AnimatedObjectsFactory* pAnimatedObjectsFactory);
    ~LevelManager();
    
    void loadLevel(const int levelindex);
    void updateEditor();
    void addSpriteToTileMap(Sprite* pSprite);
    void updateCameraPosition(const Vector2& rCameraPosition);

    const Vector2& getTileSize() const { return m_tileSize; }
    const Vector2 getTileMapSize() const;
    Vector2 getTileMapPosition() const;

private:

    void loadLevelsData(const string& levelsFile);

    Sprite* m_pStage; // main sprite
    TileMapSprite* m_pCurrentLevelTileMap; // tile map sprite
    LevelBackground* m_pCurrentLevelBackground; // background sprite
    Map* m_pMap; // map
    World* m_pWorld; // physics world
    AnimatedObjectsFactory* m_pAnimatedObjectsFactory; // animated objects factory
    vector<LevelData> m_levels;

    Vector2 m_tileSize;
    string m_levelsFolder;
    string m_tilesetsFolder;

    void initializeMembers()
    {
        m_pStage = nullptr;
        m_pCurrentLevelTileMap = nullptr;
        m_pCurrentLevelBackground = nullptr;
        m_pWorld = nullptr;
        m_pMap = nullptr;
        m_pAnimatedObjectsFactory = nullptr;
        m_levelsFolder = "";
        m_tilesetsFolder = "";
    }

};

#endif /* LevelManager_hpp */