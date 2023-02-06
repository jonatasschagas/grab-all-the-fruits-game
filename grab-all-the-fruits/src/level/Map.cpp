#include "Map.hpp"

#include <math.h>
#include "utils/MathUtils.h"
#include "utils/StringUtils.h"

 Map::Map(World* pWorld, TileMapSprite* pTileMapSprite)
 {
    initializeMembers();
    m_pTileMapSprite = pTileMapSprite;
    m_pWorld = pWorld;

    createMapInPhysicsWorld();
 }

Map::~Map()
{
    initializeMembers();
}

void Map::createMapInPhysicsWorld()
{
    for (int tileX = 0; tileX <= m_pTileMapSprite->getTileMapWidth(); tileX++)
    {
        for (int tileY = 0; tileY <= m_pTileMapSprite->getTileMapHeight(); tileY++)
        {
            if (isTileGround(tileX, tileY))
            {
                m_pWorld->createStaticBody(
                    Vector2(tileX * m_pTileMapSprite->getTileSizeInGameUnits().x, tileY * m_pTileMapSprite->getTileSizeInGameUnits().y),
                    Vector2(m_pTileMapSprite->getTileSizeInGameUnits().x, m_pTileMapSprite->getTileSizeInGameUnits().y),
                    0.f, // friction
                    0.f // restitution
                );
            }
        }
    }
}

const bool Map::isTileGround(const int& tileX, const int& tileY) const
{
    int tileData = m_pTileMapSprite->getTileData(tileX, tileY, "ground");
    return tileData != 0;
}
 
 void Map::updateCameraPosition(const Vector2& rCameraPosition)
 {

    const int screenSizeX = m_pTileMapSprite->getScreenWidthInGameUnits();
    const int screenSizeY = m_pTileMapSprite->getScreenHeightInGameUnits();
    const int worldUnitsX = m_pTileMapSprite->getMapSizeInGameUnits().x;
    const int worldUnitsY = m_pTileMapSprite->getMapSizeInGameUnits().y;
    const int halfScreenHorizontal = screenSizeX/2;
    const int halfScreenVertical = screenSizeY/2;
    float levelMaxX = worldUnitsX - halfScreenHorizontal;
    
    if (rCameraPosition.x < halfScreenHorizontal)
    {
        m_pTileMapSprite->setXOffSet(0);
    }
    else if (rCameraPosition.x > levelMaxX)
    {
        m_pTileMapSprite->setXOffSet(levelMaxX - halfScreenHorizontal);
    }
    else
    {
        m_pTileMapSprite->setXOffSet(rCameraPosition.x - halfScreenHorizontal);
    }
    
    if (rCameraPosition.y > halfScreenVertical)
    {
        m_pTileMapSprite->setYOffSet(0);
    }
    else
    {
        m_pTileMapSprite->setYOffSet(rCameraPosition.y - halfScreenVertical);
    }
    
 }
