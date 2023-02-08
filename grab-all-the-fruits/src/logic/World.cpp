#include "World.hpp"
#include "physics/box2d/Box2dPhysicsSystem.hpp"
#include "platform/PlatformManager.h"
#include "GameConfiguration.h"
#include <string>

World::World(PlatformManager* pPlatformManager)
{
    initializeMembers();

    m_pPlatformManager = pPlatformManager;
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

    for (int i = 0; i < m_kinematicBodies.size(); i++)
    {
        delete m_kinematicBodies[i];
    }

    for (int i = 0; i < m_sensors.size(); i++)
    {
        delete m_sensors[i];
    }

    delete m_pPhysicsSystem;
    
    initializeMembers();
}

void World::initWorld(const Vector2& mapSize)
{
    m_pPhysicsSystem = new Box2dPhysicsSystem(mapSize, m_pPlatformManager);
    Vector2 gravity(0.0f, GRAVITY);
    m_pPhysicsSystem->initWorld(gravity);
}  

void World::update(float delta)
{
    m_pPhysicsSystem->update(delta);
}

void World::renderDebug(const Vector2& rOffset)
{
    m_pPhysicsSystem->renderDebug(rOffset);
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

PhysicsBody* World::createKinematicBody(const Vector2& position, const Vector2& size, float friction, float restituition)
{
    PhysicsBody* pBody = m_pPhysicsSystem->createKinematicBody(position, size, friction, restituition);
    m_kinematicBodies.push_back(pBody);
    return pBody;
}

PhysicsBody* World::createSensor(const Vector2& position, const Vector2& size)
{
    PhysicsBody* pBody = m_pPhysicsSystem->createSensor(position, size);
    m_sensors.push_back(pBody);
    return pBody;
}

void World::receiveEvent(Event* pEvent) {}
