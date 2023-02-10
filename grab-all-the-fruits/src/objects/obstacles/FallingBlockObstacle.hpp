#pragma once
#ifndef FallingBlockObstacle_H
#define FallingBlockObstacle_H

#include <string>
#include "objects/PhysicalAnimatedObject.hpp"
#include "physics/PhysicsOnCollideListener.hpp"
#include "data/DataCacheManager.hpp"
#include "event/EventListener.hpp"

class PlatformManager;

using namespace std;

class FallingBlockObstacle : public PhysicalAnimatedObject, public PhysicsOnCollideListener
{
public:
    
    FallingBlockObstacle(
        PlatformManager* pPlatformManager, 
        DataCacheManager& rDataCacheManager, 
        PhysicsBody* pPhysicsBody,
        const string& animationFile, 
        const string& name, 
        const string& type, 
        EventListener* pEventListener,
        const Vector2& size,
        const Vector2& tileSize,
        const int distanceToGroundInTiles);
    ~FallingBlockObstacle();

    void onCollideStart(PhysicsBody* pPhysicsBody) override;
    void onSensorTriggeredStart(const string& name, PhysicsBody* pOtherBody) override;
    
    void update(const float delta) override;

private:

    EventListener* m_pEventListener;
    PhysicsBody* m_pPhysicsBody;
    bool m_hasFallen;
    bool m_triggered;

    void initializeMembers()
    {
        m_pEventListener = nullptr;
        m_pPhysicsBody = nullptr;
        m_hasFallen = false;
        m_triggered = false;
    }

};

#endif //FallingBlockObstacle_H
