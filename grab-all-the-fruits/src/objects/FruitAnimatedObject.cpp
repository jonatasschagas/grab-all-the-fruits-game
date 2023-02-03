#include "FruitAnimatedObject.hpp"

#include "physics/PhysicsBody.hpp"
#include "platform/PlatformManager.h"

FruitAnimatedObject::FruitAnimatedObject(
    PlatformManager* pPlatformManager, 
    DataCacheManager& rDataCacheManager, 
    const string& animationFile, 
    const string& name,
    const string& type,
    EventListener* pEventListener) : AnimatedObject(pPlatformManager, rDataCacheManager, animationFile, type), PhysicsOnCollideListener()
{
    initializeMembers();

    m_name = name;
    m_pEventListener = pEventListener;
}

FruitAnimatedObject::~FruitAnimatedObject()
{
    initializeMembers();
}

void FruitAnimatedObject::onCollide(PhysicsBody* pPhysicsBody)
{
    GameObject* pGameObject = pPhysicsBody->getGameObject();
    if (pGameObject != nullptr && pGameObject->getType().compare("player") == 0)
    {
        setVisible(false);
        Event event("fruit-collected");
        event.setInputCoordinates(getGamePosition());
        m_pEventListener->receiveEvent(&event);

        //TODO: Destroy these objects
        //destroy();
    }
}