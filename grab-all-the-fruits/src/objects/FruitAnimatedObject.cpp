#include "FruitAnimatedObject.hpp"

#include "physics/PhysicsBody.hpp"
#include "platform/PlatformManager.h"

FruitAnimatedObject::FruitAnimatedObject(
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

FruitAnimatedObject::~FruitAnimatedObject()
{
    initializeMembers();
}

void FruitAnimatedObject::onCollideStart(PhysicsBody* pPhysicsBody)
{
    GameObject* pGameObject = pPhysicsBody->getGameObject();
    if (pGameObject != nullptr && pGameObject->getType().compare("player") == 0 && !m_isCollected)
    {
        m_isCollected = true;
        Event event("fruit-collected");
        event.setInputCoordinates(getGamePosition());
        m_pEventListener->receiveEvent(&event);
        destroy();
    }
}
