#include "WaypointAnimatedObject.hpp"

#include "physics/PhysicsBody.hpp"
#include "platform/PlatformManager.h"

WaypointAnimatedObject::WaypointAnimatedObject(
    PlatformManager* pPlatformManager, 
    DataCacheManager& rDataCacheManager, 
    const string& animationFile, 
    const string& name,
    const string& type) : AnimatedObject(pPlatformManager, rDataCacheManager, animationFile, type), PhysicsOnCollideListener()
{
    initializeMembers();

    m_name = name;
}

WaypointAnimatedObject::~WaypointAnimatedObject()
{
    initializeMembers();
}

void WaypointAnimatedObject::onCollide(PhysicsBody* pPhysicsBody)
{
    GameObject* pGameObject = pPhysicsBody->getGameObject();
    if (pGameObject != nullptr && pGameObject->getType().compare("player") == 0)
    {
        stopAnimation();
    }
}

void WaypointAnimatedObject::onSensorTriggeredStart(const string& name) {}

void WaypointAnimatedObject::onSensorTriggeredEnd(const string& name) {}
