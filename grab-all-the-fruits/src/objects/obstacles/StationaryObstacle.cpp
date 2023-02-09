#include "StationaryObstacle.hpp"

#include "physics/PhysicsBody.hpp"
#include "platform/PlatformManager.h"

StationaryObstacle::StationaryObstacle(
    PlatformManager* pPlatformManager, 
    DataCacheManager& rDataCacheManager, 
    PhysicsBody* pPhysicsBody,
    const string& animationFile, 
    const string& name,
    const string& type,
    EventListener* pEventListener) : PhysicalAnimatedObject(pPlatformManager, rDataCacheManager, pPhysicsBody, animationFile, type), PhysicsOnCollideListener()
{
    initializeMembers();

    m_name = name;
    m_pEventListener = pEventListener;
}

StationaryObstacle::~StationaryObstacle()
{
    initializeMembers();
}

void StationaryObstacle::onCollide(PhysicsBody* pPhysicsBody)
{
    GameObject* pGameObject = pPhysicsBody->getGameObject();
    if (pGameObject != nullptr && pGameObject->getType().compare("player") == 0)
    {
        Event event("player-died");
        event.setInputCoordinates(getGamePosition());
        m_pEventListener->receiveEvent(&event);
    }
}

void StationaryObstacle::onSensorTriggeredStart(const string& name) {}

void StationaryObstacle::onSensorTriggeredEnd(const string& name) {}
