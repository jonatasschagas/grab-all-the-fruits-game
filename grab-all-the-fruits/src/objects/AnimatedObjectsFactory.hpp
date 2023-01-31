#pragma once
#ifndef AnimatedObjectsFactory_H
#define AnimatedObjectsFactory_H

#include <string>
#include "core/TileMapMetaTileFactory.hpp"
#include "data/DataCacheManager.hpp"

using namespace std;

class PlatformManager;
class World;
class AnimatedObject;

class AnimatedObjectsFactory : public TileMapMetaTileFactory
{
public:

    AnimatedObjectsFactory(const string& animatedObjectsPath, PlatformManager* pPlatformManager, DataCacheManager& rDataCacheManager, World* pWorld);
    ~AnimatedObjectsFactory();
    
    Sprite* createMetaTile(const TileConfig* pTileConfig, Vector2 position, Vector2 size) override;

private:

    AnimatedObject* createWaypoint(const string& animationFile, const string& objectName, Vector2 position, Vector2 size);

    string m_animatedObjectsPath;
    PlatformManager* m_pPlatformManager;
    World* m_pWorld;
    DataCacheManager& m_rDataCacheManager;

    void initializeMembers()
    {
        m_animatedObjectsPath = "";
        m_pPlatformManager = nullptr;
        m_pWorld = nullptr;
    }

};

#endif //AnimatedObjectsFactory_H
