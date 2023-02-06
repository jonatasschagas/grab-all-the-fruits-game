#pragma once
#ifndef Map_hpp 
#define Map_hpp

#include "core/TileMapSprite.hpp"
#include "core/Vector2.h"
#include "logic/World.hpp"

class Map
{
public:
    
    Map(World* pWorld, TileMapSprite* pTileMapSprite);
    ~Map();

    void updateCameraPosition(const Vector2& rCameraPosition);

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
