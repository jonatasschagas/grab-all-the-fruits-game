#pragma once
#ifndef MapManager_hpp
#define MapManager_hpp

#include <tuple>
#include <vector>
#include <stdio.h>

#include "platform/PlatformManager.h"
#include "core/BaseDataStructures.h"
#include "core/Sprite.hpp"

using namespace std;

class LevelData;
class TileMapLayer;

class MetaTileItem
{
public:
    string type = "";
    string name = "";
    float m_x = 0;
    float m_y = 0;
};

class MapManager : public Sprite
{
public:
  
    MapManager(const int tileSizeInWorldUnits,
               const int maxScreenShakeOffsetY,
               const float screenShakeDuration,
               const vector<int>& slidableTileTypes,
               PlatformManager* pPlatformManager);
    ~MapManager();
    
    void update(float deltaTime) override;
    void updateCamera(float centerCameraX, float centerCameraY);
   
    void loadMap(LevelData* pLevelData);

    const float getWorldLimitX() const;
    
    int getTileX(float x) const;
    int getTileXCeil(float x) const;
    int getTileXFloor(float x) const;
    
    int getTileY(float y) const;
    int getTileMapWidth() const;
    int getTileMapHeight() const;
    
    int getWorldUnitsY() const;
    
    float getNextWall(float x, float y, bool facingRight) const;
    float getGroundTileY(float x, float y) const;
    float getDistanceToGroundTile(float x, float y) const;
    bool isGround(float x, float y) const;
    bool isTileGround(int tileX, int tileY) const;

    bool isWall(float x, float y, float w, float h, bool movingRight) const;
    bool isTileWall(int tileX, int tileY) const;
    
    bool isSlidableWall(float x, float y, float width, float height, bool facingRight) const;
    bool isVisible(float x, float y) const;
    bool hasReachedEndOfLevel(Vector2 point) const;

    float getXOffSet() const;
    float getYOffSet() const;
    
    float getLevelStartX() const;
    float getLevelStartY() const;
    float getLevelExitX() const;
    float getLevelExitY() const;
    
    const Vector2 collideWithWall(float x, float y, float width, float height) const;
    
    static bool collide(float x1, float y1, float width1, float height1, float x2, float y2, float width2, float height2);
    
    const vector<MetaTileItem*>& getMetaTileItems() const;
    
    bool isVisibleInParent(Sprite* pChild) const override;
    
    void shakeCamera(float intensity);
    
private:

    void unloadMap();
    
    void loadMetaLayerConfiguration();
    void loadMapLayers();
    void loadMapMiscElements();

    void createTile(int x, int y, Sprite* pSpriteLayer, TileMapLayer* pLayer);

    void updateMapMiscElements(float deltaTime);
    void updateCameraShake(float deltaTime);

    int invertTileY(const int originalTileY) const;
    int getTileData(int tileX, int tileY) const;
    
    PlatformManager* m_pPlatformManager;
    LevelData* m_pCurrentLevelData;
    TileMapLayer* m_pMetaLayer;
    TileMapLayer* m_pGroundLayer;

    GameSize m_worldSize;
    GameSize m_tileMapSize;
    int m_tileSizeInWorldUnits;
    int m_maxScreenShakeOffsetY;
    float m_screenShakeDuration;
    vector<int> m_slidableTileTypes;
    
    // camera
    float m_xOffSetParallax1;
    float m_xOffSet;
    float m_yOffSet;
    int m_cameraStartTileX;
    int m_cameraStartTileY;
    float m_cameraCenterX;
    float m_cameraCenterY;
    
    bool m_cameraShaking;
    float m_cameraShakeIntensity;
    float m_cameraShakeAngle;
    float m_cameraShakeOffsetX;
    float m_cameraShakeOffsetY;
    float m_cameraShakeAccumulator;
    int m_ticks;
    
    Vector2 m_levelStart;
    Vector2 m_levelExit;

    vector<MetaTileItem*> m_metaTileItems;
    
    void initializeMembers()
    {
        m_pPlatformManager = nullptr;
        m_pCurrentLevelData = nullptr;
        
        m_pMetaLayer = nullptr;
        m_pGroundLayer = nullptr;
        
        m_maxScreenShakeOffsetY = 0;
        m_tileSizeInWorldUnits = 0;
        m_screenShakeDuration = 0;
        m_slidableTileTypes.clear();
        
        m_xOffSetParallax1 = 0;
        m_xOffSet = 0;
        m_yOffSet = 0;
        m_cameraStartTileX = 0;
        m_cameraStartTileY = 0;
        m_cameraCenterX = 0;
        m_cameraCenterY = 0;
        
        m_cameraShaking = false;
        m_cameraShakeIntensity = 0;
        m_cameraShakeAngle = 0;
        m_cameraShakeOffsetX = 0;
        m_cameraShakeOffsetY = 0;
        m_cameraShakeAccumulator = 0;
        m_ticks = 0;
        
        m_metaTileItems.clear();
    }

};
#endif /* MapManager_hpp */
