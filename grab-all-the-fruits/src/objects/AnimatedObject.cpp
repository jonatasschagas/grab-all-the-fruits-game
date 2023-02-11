#include "AnimatedObject.hpp"

AnimatedObject::AnimatedObject(
    PlatformManager* pPlatformManager, 
    DataCacheManager& rDataCacheManager, 
    const string& animationFile, 
    const string& type,
    const string& name) : 
    AnimatedSprite(
        pPlatformManager, 
        rDataCacheManager, 
        animationFile
    ), 
    m_type(type), 
    m_name(name)
{
}

AnimatedObject::~AnimatedObject()
{
    initializeMembers();
}

void AnimatedObject::update(float deltaTime)
{
    AnimatedSprite::update(deltaTime);
}
