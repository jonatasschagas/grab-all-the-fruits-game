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

struct Trampoline
{
    int tileX;
    int tileY;
    int bounciness;
};

struct Platform
{
    int tileX;
    int tileY;
    int targetTileX;
    int targetTileY;
    float widthInTiles;
    float heightInTiles;
    float speed;
    string type;
};

struct Trap
{
    int tileX;
    int tileY;
    int targetTileX;
    int targetTileY;
    float widthInTiles;
    float heightInTiles;
    string type;
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
    vector<Trap> traps;
    vector<Trampoline> trampolines;
};

class LevelManager : public EventListener
{
public:
    LevelManager(
        const string& levelsFile, 
        Sprite* pStage, 
        World* pWorld, 
        AnimatedObjectsFactory* pAnimatedObjectsFactory, 
        EventListener* pEventListener);
    ~LevelManager();
    
    void loadLevel(const int levelindex);
    void updateEditor();
    void addSpriteToTileMap(Sprite* pSprite);
    void updateCameraPosition(const Vector2& rCameraPosition);

    void receiveEvent(Event* pEvent) override;

    const Vector2& getTileSize() const { return sm_tileSize; }
    const Vector2 getTileMapSize() const;
    Vector2 getTileMapPosition() const;

    const bool hasCompletedLevel() const;

    const Platform* findPlatform(int tileX, int tileY) const;
    const Trap* findTrap(int tileX, int tileY) const;
    const Trampoline* findTrampoline(int tileX, int tileY) const;

private:

    void loadLevelsData(const string& levelsFile);

    Sprite* m_pStage; // main sprite
    TileMapSprite* m_pCurrentLevelTileMap; // tile map sprite
    LevelBackground* m_pCurrentLevelBackground; // background sprite
    Map* m_pMap; // map
    World* m_pWorld; // physics world
    AnimatedObjectsFactory* m_pAnimatedObjectsFactory; // animated objects factory
    EventListener* m_pEventListener;
    int m_currentLevelIndex;
    int m_numFruitsCollected;

    static vector<LevelData> sm_levels;
    static Vector2 sm_tileSize;
    static string sm_levelsFolder;
    static string sm_tilesetsFolder;

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
        m_currentLevelIndex = 0;
        m_numFruitsCollected = 0;
        m_initialized = false;
    }

};

#endif /* LevelManager_hpp */