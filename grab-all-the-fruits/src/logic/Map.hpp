#pragma once
#ifndef Map_hpp 
#define Map_hpp

#include "core/TileMapSprite.hpp"
#include "core/BaseDataStructures.h"

class Map
{
public:
    
    Map(TileMapSprite* pTileMapSprite);
    ~Map();

    const bool isWall(const Vector2& position, const GameSize& size, bool facingRight) const;
    Vector2 getGroundCoordUnderneath(const Vector2& position) const;
    const bool isGround(const Vector2& position) const;

    const float getTileXFloor(const float& x) const;
    const float getTileXCeil(const float& x) const;

private:

    const bool isTileWall(const int& tileX, const int& tileY) const;
    const bool isTileGround(const int& tileX, const int& tileY) const;
    
    TileMapSprite* m_pTileMapSprite;
    
    void initializeMembers()
    {
        m_pTileMapSprite = nullptr;
    }
    
};

#endif /* Map_hpp */
