#include "WaypointAnimatedObject.hpp"

#include "physics/PhysicsBody.hpp"
#include "platform/PlatformManager.h"

WaypointAnimatedObject::WaypointAnimatedObject(
    PlatformManager* pPlatformManager, 
    DataCacheManager& rDataCacheManager, 
    const string& animationFile, 
    const string& name) : AnimatedObject(pPlatformManager, rDataCacheManager, animationFile, "waypoint"), PhysicsOnCollideListener()
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