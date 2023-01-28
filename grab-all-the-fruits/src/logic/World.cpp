#include "World.hpp"
#include "physics/box2d/Box2dPhysicsSystem.hpp"
#include "GameConfiguration.h"
#include <string>

World::World(const Vector2& worldSize)
{
    initializeMembers();

    m_pPhysicsSystem = new Box2dPhysicsSystem(worldSize);
    Vector2 gravity(0.0f, GRAVITY);
    m_pPhysicsSystem->initWorld(gravity);
}

World::~World()
{
    for (int i = 0; i < m_dynamicBodies.size(); i++)
    {
        delete m_dynamicBodies[i];
    }

    for (int i = 0; i < m_staticBodies.size(); i++)
    {
        delete m_staticBodies[i];
    }

    delete m_pPhysicsSystem;
    
    initializeMembers();
}

void World::update(float delta)
{
    m_pPhysicsSystem->update(delta);
}

PhysicsBody* World::createDynamicBody(const Vector2& position, const Vector2& size, float weight, float friction, float restituition, float gravityScale)
{
    PhysicsBody* pBody = m_pPhysicsSystem->createDynamicBody(position, size, weight, friction, restituition, gravityScale);
    m_dynamicBodies.push_back(pBody);
    return pBody;
}
PhysicsBody* World::createStaticBody(const Vector2& position, const Vector2& size, float friction, float restituition)
{
    PhysicsBody* pBody = m_pPhysicsSystem->createStaticBody(position, size, friction, restituition);
    m_staticBodies.push_back(pBody);
    return pBody;
}

void World::receiveEvent(Event* pEvent) {
    for (int i = 0; i < m_dynamicBodies.size(); i++)
    {   
        PhysicsBody* pBody = m_dynamicBodies[i];
        GameObject* pGameObject = pBody->getGameObject();

        if (strcmp(pGameObject->getType().c_str(), pEvent->getTarget().c_str()) == 0) {
            if (strcmp(pEvent->getName().c_str(), "right_start") == 0) {
                pBody->applyInstantForce(Vector2(PLAYER_RUNNING_SPEED, 0));
            } else if (strcmp(pEvent->getName().c_str(), "right_stop") == 0) {
                pBody->applyInstantForce(Vector2(0, 0));
            } else if (strcmp(pEvent->getName().c_str(), "left_start") == 0) {
                pBody->applyInstantForce(Vector2(-PLAYER_RUNNING_SPEED, 0));
            } else if (strcmp(pEvent->getName().c_str(), "left_stop") == 0) {
                pBody->applyInstantForce(Vector2(0, 0));
            } else if (strcmp(pEvent->getName().c_str(), "space_start") == 0) {
                const Vector2& currentVel = pBody->getVelocity();
                if (currentVel.y == 0) 
                {
                    pBody->applyForce(Vector2(0, PLAYER_JUMPING_FORCE), PhysicsForceType::PhysicsForceTypeImpulse);
                } 
            }
        }
    }
}
