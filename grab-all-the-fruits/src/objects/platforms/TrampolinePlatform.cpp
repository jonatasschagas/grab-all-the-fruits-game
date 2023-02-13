#include "TrampolinePlatform.hpp"

#include "physics/PhysicsBody.hpp"
#include "platform/PlatformManager.h"

TrampolinePlatform::TrampolinePlatform(
    PlatformManager* pPlatformManager, 
    DataCacheManager& rDataCacheManager, 
    PhysicsBody* pPhysicsBody,
    const string& animationFile, 
    const string& name,
    const string& type,
    EventListener* pEventListener) : 
    PhysicalAnimatedObject(
        pPlatformManager, 
        rDataCacheManager, 
        pPhysicsBody, 
        animationFile, 
        type,
        name), PhysicsOnCollideListener()
{
    initializeMembers();

    m_pEventListener = pEventListener;
}

TrampolinePlatform::~TrampolinePlatform()
{
    initializeMembers();
}

void TrampolinePlatform::onCollideStart(PhysicsBody* pPhysicsBody)
{
    GameObject* pGameObject = pPhysicsBody->getGameObject();
    if (pGameObject != nullptr && pGameObject->getType().compare("player") == 0)
    {
        play("bounce");
        setOnAnimationFinishedCallback("bounce", [this]() {
            play("idle");
        });
    }
}
