#pragma once
#ifndef AnimatedObjectsFactory_H
#define AnimatedObjectsFactory_H

#include <string>
#include "core/TileMapMetaTileFactory.hpp"
#include "data/DataCacheManager.hpp"

using namespace std;

class PlatformManager;
class World;
class EventListener;
class AnimatedObject;
class Player;

class AnimatedObjectsFactory : public TileMapMetaTileFactory
{
public:

    AnimatedObjectsFactory(const string& animatedObjectsPath, PlatformManager* pPlatformManager, DataCacheManager& rDataCacheManager, World* pWorld, EventListener* pEventListener);
    ~AnimatedObjectsFactory();
    
    Sprite* createMetaTile(const TileConfig* pTileConfig, const Vector2& position, const Vector2& size) override;
    
    Player* createPlayer(const Vector2& position, const Vector2& size);

    AnimatedObject* createDisappearingAnimation(const Vector2& position, const Vector2& size);
    
private:

    AnimatedObject* createWaypoint(const string& objectName, const string& objectType, Vector2 position, Vector2 size);
    AnimatedObject* createCollectable(const string& objectName, const string& objectType, Vector2 position, Vector2 size);
    AnimatedObject* createObstacle(const string& objectName, const string& objectType, Vector2 position, Vector2 size);
    
    string m_animatedObjectsPath;
    PlatformManager* m_pPlatformManager;
    EventListener* m_pEventListener;
    World* m_pWorld;
    DataCacheManager& m_rDataCacheManager;

    void initializeMembers()
    {
        m_animatedObjectsPath = "";
        m_pPlatformManager = nullptr;
        m_pEventListener = nullptr;
        m_pWorld = nullptr;
    }

};

#endif //AnimatedObjectsFactory_H
