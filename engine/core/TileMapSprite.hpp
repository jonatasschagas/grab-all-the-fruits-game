#pragma once
#ifndef TileMapSprite_hpp
#define TileMapSprite_hpp

#include <stdio.h>
#include <string>
#include "../core/Sprite.hpp"
#include "core/BaseDataStructures.h"

using namespace std;

class TileMapData;
class TileMapLayer;

class TileMapSprite : public Sprite
{
public:
    
    TileMapSprite(const GameSize& tileSizeWorldUnits, PlatformManager* pPlatformManager);
    ~TileMapSprite();
    
    void update(float delta) override;
    void render() override;
    
    void loadMap(TileMapData* pMapData);

    const float getWorldLimitX() const;
    
    int getTileX(float x) const;
    int getTileXCeil(float x) const;
    int getTileXFloor(float x) const;
    
    int getTileY(float y) const;
    int getTileMapWidth() const;
    int getTileMapHeight() const;
    
    int getWorldUnitsX() const;
    int getWorldUnitsY() const;
    
    float getXOffSet() const;
    void setXOffSet(float xOffSet);
    
    float getYOffSet() const;
    void setYOffSet(float yOffSet);

    int getTileData(int tileX, int tileY, const string& tileLayerName) const;
    int getTileData(float x, float y, const string& tileLayerName) const;
    
    bool isVisibleInParent(Sprite* pChild) const override;
    
    const GameSize& getTileSizeInWorldUnits() const;

private:
    
    void unloadMap();
    void loadMapLayers();
    void createTile(int x, int y, Sprite* pSpriteLayer, TileMapLayer* pLayer);
    int invertTileY(const int originalTileY) const;
    
    PlatformManager* m_pPlatformManager;
    TileMapData* m_pCurrentMapData;
    GameSize m_tileSizeInWorldUnits;
    float m_xOffSet;
    float m_yOffSet;
    
    void initializeMembers()
    {
        m_pPlatformManager = nullptr;
        m_pCurrentMapData = nullptr;
        m_tileSizeInWorldUnits.w = 0;
        m_tileSizeInWorldUnits.h = 0;
        m_xOffSet = 0;
        m_yOffSet = 0;
    }
    
};

#endif /* TileMapSprite_hpp */
