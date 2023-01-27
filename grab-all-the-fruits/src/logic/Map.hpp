#pragma once
#ifndef Map_hpp 
#define Map_hpp

#include "core/TileMapSprite.hpp"
#include "core/BaseDataStructures.h"
#include "World.hpp"

class Map
{
public:
    
    Map(World* pWorld, TileMapSprite* pTileMapSprite);
    ~Map();

    const bool isWall(const Vector2& position) const;
    Vector2 getGroundCoordUnderneath(const Vector2& position) const;
    const bool isGround(const Vector2& position) const;
    const float getTileX(const float& x) const;

    const Vector2 getWorldLimits() const;

private:

    void createMapInPhysicsWorld();

    const bool isTileGround(const int& tileX, const int& tileY) const;
    
    TileMapSprite* m_pTileMapSprite;
    World* m_pWorld;
    
    void initializeMembers()
    {
        m_pTileMapSprite = nullptr;
        m_pWorld = nullptr;
    }
    
};

#endif /* Map_hpp */
