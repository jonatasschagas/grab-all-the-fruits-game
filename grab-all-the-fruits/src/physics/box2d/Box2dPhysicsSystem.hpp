#pragma once
#ifndef Box2dPhysicsSystem_hpp 
#define Box2dPhysicsSystem_hpp

#include "physics/PhysicsSystem.hpp"
#include "platform/PlatformManager.h"
#include <box2d/box2d.h>

class Box2dDebugRenderer;

const int BOX2D_VELOCITY_ITERATIONS = 6;
const int BOX2D_POSITION_ITERATIONS = 2;
const float BOX2D_TIME_STEP = 1.0f / 60.0f;

class Box2dPhysicsSystem : public PhysicsSystem, public b2ContactListener
{
public:

    Box2dPhysicsSystem(const Vector2& rWorldSize, PlatformManager* pPlatformManager);
    ~Box2dPhysicsSystem();

    void initWorld(const Vector2& rGravity) override;
    void update(float delta) override;
    void renderDebug(const Vector2& rOffset) override;
    
    PhysicsBody* createDynamicBody(
        const Vector2& rGamePosition, 
        const Vector2& rGameSize,
        float weight,
        float friction,
        float restituition,
        float gravityScale) override;

    PhysicsBody* createStaticBody(
        const Vector2& rGamePosition, 
        const Vector2& rGameSize,
        float friction,
        float restituition) override;

    PhysicsBody* createSensor(
        const Vector2& rGamePosition, 
        const Vector2& rGameSize) override;

    void BeginContact(b2Contact* contact) override;
    void EndContact(b2Contact* contact) override;
    
private:

    b2Vec2 toPhysicsWorld(const Vector2& rGamePosition) const;

    b2World* m_pBox2DWorld;
    Box2dDebugRenderer * m_pDebugRenderer;
    Vector2 m_worldSize;

    void initializeMembers()
    {
        m_pBox2DWorld = nullptr;
        m_pDebugRenderer = nullptr;
    }

};

#endif /* Box2dPhysicsSystem_hpp */
