#pragma once
#ifndef TileMapMetaTileFactory_H
#define TileMapMetaTileFactory_H

#include "Sprite.hpp"
#include "data/TileMapData.hpp"

class TileMapMetaTileFactory
{
public:

    virtual ~TileMapMetaTileFactory() {}
    
    virtual Sprite* createMetaTile(const TileConfig* pTileConfig, Vector2 position, Vector2 size) = 0;

};

#endif //TileMapMetaTileFactory_H
