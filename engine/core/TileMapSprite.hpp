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
    
    TileMapSprite(const Vector2& tileSizeWorldUnits, PlatformManager* pPlatformManager);
    ~TileMapSprite();
    
    void update(float delta) override;
    void render() override;
    
    void loadMap(TileMapData* pMapData);

    const float getWorldLimitX() const;
    
    int getTileX(float x) const;
    int getTileY(float y) const;
    int getTileMapWidth() const;
    int getTileMapHeight() const;
    
    int getScreenWidthInGameUnits() const;
    int getScreenHeightInGameUnits() const;
    
    float getXOffSet() const;
    void setXOffSet(float xOffSet);
    
    float getYOffSet() const;
    void setYOffSet(float yOffSet);

    int getTileData(int tileX, int tileY, const string& tileLayerName) const;
    int getTileData(float x, float y, const string& tileLayerName) const;
    
    bool isVisibleInParent(Sprite* pChild) const override;
    
    const Vector2& getTileSizeInGameUnits() const;
    const Vector2& getMapSizeInGameUnits() const;

private:
    
    void unloadMap();
    void loadMapLayers();
    void createTile(int x, int y, Sprite* pSpriteLayer, TileMapLayer* pLayer);
    
    PlatformManager* m_pPlatformManager;
    TileMapData* m_pCurrentMapData;
    Vector2 m_tileSizeInWorldUnits;
    Vector2 m_mapSizeInGameUnits;
    float m_xOffSet;
    float m_yOffSet;
    
    void initializeMembers()
    {
        m_pPlatformManager = nullptr;
        m_pCurrentMapData = nullptr;
        m_tileSizeInWorldUnits.x = 0;
        m_tileSizeInWorldUnits.y = 0;
        m_xOffSet = 0;
        m_yOffSet = 0;
        m_mapSizeInGameUnits.x = 0;
        m_mapSizeInGameUnits.y = 0;
    }
    
};

#endif /* TileMapSprite_hpp */
