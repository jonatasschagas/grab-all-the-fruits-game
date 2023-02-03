#include "PhysicalAnimatedObject.hpp"

PhysicalAnimatedObject::PhysicalAnimatedObject(
    PlatformManager* pPlatformManager, 
    DataCacheManager& rDataCacheManager, 
    PhysicsBody* pPhysicsBody, 
    const string& animationFile, 
    const string& type) : AnimatedObject(pPlatformManager, rDataCacheManager, animationFile, type)
{
    initializeMembers();
    
    m_pPhysicsBody = pPhysicsBody;
}

PhysicalAnimatedObject::~PhysicalAnimatedObject()
{
    m_pPhysicsBody->destroy();
    
    initializeMembers();
}