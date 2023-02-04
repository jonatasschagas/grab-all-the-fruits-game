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
    
    World(const Vector2& worldSize, PlatformManager* pPlatformManager);
    ~World();
    
    void update(float delta);
    void renderDebug(const Vector2& rOffset);
    void receiveEvent(Event* pEvent) override;
    
    PhysicsBody* createDynamicBody(const Vector2& position, const Vector2& size, float weight, float friction, float restituition, float gravityScale);
    PhysicsBody* createStaticBody(const Vector2& position, const Vector2& size, float friction, float restituition);
    PhysicsBody* createSensor(const Vector2& position, const Vector2& size);

private:

    vector<PhysicsBody*> m_dynamicBodies;
    vector<PhysicsBody*> m_staticBodies;
    vector<PhysicsBody*> m_sensors;
    PhysicsSystem* m_pPhysicsSystem;

    void initializeMembers()
    {
        m_pPhysicsSystem = nullptr;
        m_dynamicBodies.clear();
        m_staticBodies.clear();
        m_sensors.clear();
    }
    
};

#endif /* World_hpp */
