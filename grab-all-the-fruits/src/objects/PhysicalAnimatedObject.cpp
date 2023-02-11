#include "PhysicalAnimatedObject.hpp"

PhysicalAnimatedObject::PhysicalAnimatedObject(
    PlatformManager* pPlatformManager, 
    DataCacheManager& rDataCacheManager, 
    PhysicsBody* pPhysicsBody, 
    const string& animationFile, 
    const string& type,
    const string& name) : 
    AnimatedObject(
        pPlatformManager, 
        rDataCacheManager, 
        animationFile, 
        type, 
        name)
{
    initializeMembers();
    
    m_pPhysicsBody = pPhysicsBody;
}

PhysicalAnimatedObject::~PhysicalAnimatedObject()
{
    if (m_pPhysicsBody != nullptr)
    {
        m_pPhysicsBody->destroy();
    }
    
    initializeMembers();
}