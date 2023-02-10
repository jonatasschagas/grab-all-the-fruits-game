#pragma once
#ifndef World_hpp 
#define World_hpp

#include "core/Vector2.h"
#include "event/EventListener.hpp"
#include <vector>
#include <string>
#include "physics/PhysicsBody.hpp"
#include "physics/PhysicsSystem.hpp"

using namespace std;

class PlatformManager;

class World : public EventListener
{
public:
    
    World(PlatformManager* pPlatformManager);
    ~World();
    
    void initWorld(const Vector2& mapSize);

    void update(float delta);
    void renderDebug(const Vector2& rOffset);
    void receiveEvent(Event* pEvent) override;
    
    PhysicsBody* createDynamicBody(const Vector2& position, const Vector2& size, float weight, float friction, float restituition, float gravityScale, PhysicsShape physicsShape=PhysicsShape::PhysicsShape_Circle);
    PhysicsBody* createStaticBody(const Vector2& position, const Vector2& size, float friction, float restituition, PhysicsShape physicsShape=PhysicsShape::PhysicsShape_Box);
    PhysicsBody* createKinematicBody(const Vector2& position, const Vector2& size, float friction, float restituition, PhysicsShape physicsShape=PhysicsShape::PhysicsShape_Box);
    PhysicsBody* createSensor(const Vector2& position, const Vector2& size);

private:

    vector<PhysicsBody*> m_dynamicBodies;
    vector<PhysicsBody*> m_staticBodies;
    vector<PhysicsBody*> m_kinematicBodies;
    vector<PhysicsBody*> m_sensors;
    PhysicsSystem* m_pPhysicsSystem;
    PlatformManager* m_pPlatformManager;

    void initializeMembers()
    {
        m_pPhysicsSystem = nullptr;
        m_pPlatformManager = nullptr;
        m_dynamicBodies.clear();
        m_staticBodies.clear();
        m_kinematicBodies.clear();
        m_sensors.clear();
    }
    
};

#endif /* World_hpp */
