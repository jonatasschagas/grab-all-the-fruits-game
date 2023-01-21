#include "Map.hpp"

#include <math.h>
#include "utils/MathUtils.h"

 Map::Map(TileMapSprite* pTileMapSprite)
 {
    initializeMembers();
    m_pTileMapSprite = pTileMapSprite;
 }

Map::~Map()
{
    initializeMembers();
}

const bool Map::isWall(const Vector2& position, const GameSize& size, bool facingRight) const
{
    const GameSize& tileSize = m_pTileMapSprite->getTileSizeInWorldUnits();

    int tileXBefore = (int) floor(position.x/tileSize.w);
    int tileY = (int) floor(position.y/tileSize.h);
    bool tileBeforeWall = isTileWall(tileXBefore, tileY);

    if (facingRight)
    {
        int tileXAfter = tileXBefore + 1;
        bool tileAfterWall = isTileWall(tileXAfter, tileY);

        return tileAfterWall && checkCollision(position, size, Vector2(tileXAfter * tileSize.w, tileY * tileSize.h), tileSize);
    }

    return tileBeforeWall && checkCollision(position, size, Vector2(tileXBefore * tileSize.w, tileY * tileSize.h), tileSize);
}
    
Vector2 Map::getGroundCoordUnderneath(const Vector2& position) const
{
    int tileX = m_pTileMapSprite->getTileX(position.x);

    int startTileY = m_pTileMapSprite->getTileY(position.y);
    //int invertedTileY = invertTileY(startTileY);

    for (int tileY = startTileY; tileY >= 0; tileY--)
    {
        if (isTileGround(tileX, tileY))
        {
            return Vector2(position.x, (tileY + 1) * m_pTileMapSprite->getTileSizeInWorldUnits().h);
        }
    }
    return Vector2(position.x, 0);
}

const bool Map::isGround(const Vector2& position) const
{
    int tileX = m_pTileMapSprite->getTileX(position.x);
    int tileY = m_pTileMapSprite->getTileY(position.y);
    return isTileGround(tileX, tileY);
}

const float Map::getTileXFloor(const float& x) const
{
    return floor(x/m_pTileMapSprite->getTileSizeInWorldUnits().w) * m_pTileMapSprite->getTileSizeInWorldUnits().w;
}

const float Map::getTileXCeil(const float& x) const
{
    return ceil(x/m_pTileMapSprite->getTileSizeInWorldUnits().w) * m_pTileMapSprite->getTileSizeInWorldUnits().w;
}

const bool Map::isTileWall(const int& tileX, const int& tileY) const
{
    int tileData = m_pTileMapSprite->getTileData(tileX, tileY, "ground");
    return tileData != 0;
}

const bool Map::isTileGround(const int& tileX, const int& tileY) const
{
    int tileData = m_pTileMapSprite->getTileData(tileX, tileY, "ground");
    return tileData != 0;
}