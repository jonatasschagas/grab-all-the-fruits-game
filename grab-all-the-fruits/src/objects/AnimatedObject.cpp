#include "AnimatedObject.hpp"

AnimatedObject::AnimatedObject(
    PlatformManager* pPlatformManager, 
    DataCacheManager& rDataCacheManager, 
    const string& animationFile, 
    const string& type) : AnimatedSprite(pPlatformManager, rDataCacheManager, animationFile), m_type(type)
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
