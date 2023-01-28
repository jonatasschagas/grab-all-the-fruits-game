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
                    Vector2(tileX * m_pTileMapSprite->getTileSizeInWorldUnits().w, tileY * m_pTileMapSprite->getTileSizeInWorldUnits().h),
                    Vector2(m_pTileMapSprite->getTileSizeInWorldUnits().w, m_pTileMapSprite->getTileSizeInWorldUnits().h),
                    0.f, // friction
                    0.f // restitution
                );
            }
        }
    }
}

const bool Map::isWall(const Vector2& position) const
{
    int tileX = m_pTileMapSprite->getTileX(position.x);
    int tileY = m_pTileMapSprite->getTileY(position.y);
    return isTileGround(tileX, tileY);
}
    
Vector2 Map::getGroundCoordUnderneath(const Vector2& position) const
{
    int tileX = m_pTileMapSprite->getTileX(position.x);
    int startTileY = m_pTileMapSprite->getTileY(position.y);

    for (int tileY = startTileY; tileY <= m_pTileMapSprite->getTileMapHeight(); tileY++)
    {
        if (isTileGround(tileX, tileY))
        {
            return Vector2(position.x, (tileY - 1) * m_pTileMapSprite->getTileSizeInWorldUnits().h);
        }
    }
    // return a coordinate below the visible screen so that the object is not visible
    return Vector2(position.x, (m_pTileMapSprite->getTileMapHeight() + 10) * m_pTileMapSprite->getTileSizeInWorldUnits().h);
}

const bool Map::isGround(const Vector2& position) const
{
    int tileX = m_pTileMapSprite->getTileX(position.x);
    int tileY = m_pTileMapSprite->getTileY(position.y);
    return isTileGround(tileX, tileY);
}

const float Map::getTileX(const float& x) const
{
    return floor(x/m_pTileMapSprite->getTileSizeInWorldUnits().w) * m_pTileMapSprite->getTileSizeInWorldUnits().w;
}

const bool Map::isTileGround(const int& tileX, const int& tileY) const
{
    int tileData = m_pTileMapSprite->getTileData(tileX, tileY, "ground");
    return tileData != 0;
}
 
const Vector2 Map::getWorldLimits() const
{
    return Vector2(m_pTileMapSprite->getTileMapWidth() * m_pTileMapSprite->getTileSizeInWorldUnits().w,
                    m_pTileMapSprite->getTileMapHeight() * m_pTileMapSprite->getTileSizeInWorldUnits().h);
}