#include "WaypointAnimatedObject.hpp"

#include "physics/PhysicsBody.hpp"
#include "platform/PlatformManager.h"
#include "event/Event.hpp"

WaypointAnimatedObject::WaypointAnimatedObject(
    PlatformManager* pPlatformManager, 
    DataCacheManager& rDataCacheManager, 
    EventListener* pEventListener,
    const string& animationFile, 
    const string& name,
    const string& type) : 
    AnimatedObject(
        pPlatformManager, 
        rDataCacheManager, 
        animationFile, 
        type,
        name), 
    PhysicsOnCollideListener()
{
    initializeMembers();

    m_name = name;
    m_pEventListener = pEventListener;
}

WaypointAnimatedObject::~WaypointAnimatedObject()
{
    initializeMembers();
}

void WaypointAnimatedObject::onCollideStart(PhysicsBody* pPhysicsBody)
{
    GameObject* pGameObject = pPhysicsBody->getGameObject();
    if (pGameObject != nullptr && pGameObject->getType().compare("player") == 0)
    {
        Event event("waypoint-reached");
        event.setTarget(m_name);
        event.setInputCoordinates(getGamePosition());
        m_pEventListener->receiveEvent(&event);
    }
}
