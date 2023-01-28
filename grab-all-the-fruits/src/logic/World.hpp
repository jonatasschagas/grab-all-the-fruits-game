#pragma once
#ifndef World_hpp 
#define World_hpp

struct Body;

#include "core/BaseDataStructures.h"
#include "event/EventListener.hpp"
#include <vector>
#include <string>
#include "physics/PhysicsBody.hpp"
#include "physics/PhysicsSystem.hpp"

using namespace std;


class World : public EventListener
{
public:
    
    World(const Vector2& worldSize);
    ~World();
    
    void update(float delta);
    void receiveEvent(Event* pEvent) override;
    
    PhysicsBody* createDynamicBody(const Vector2& position, const Vector2& size, float weight, float friction, float restituition, float gravityScale);
    PhysicsBody* createStaticBody(const Vector2& position, const Vector2& size, float friction, float restituition);

private:

    vector<PhysicsBody*> m_dynamicBodies;
    vector<PhysicsBody*> m_staticBodies;
    PhysicsSystem* m_pPhysicsSystem;

    void initializeMembers()
    {
        m_pPhysicsSystem = nullptr;
        m_dynamicBodies.clear();
        m_staticBodies.clear();
    }
    
};

#endif /* World_hpp */
