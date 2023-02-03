#include "DisappearingAnimation.hpp"

#include <functional>

DisappearingAnimation::DisappearingAnimation(
    PlatformManager* pPlatformManager, 
    DataCacheManager& rDataCacheManager,
    const string& animationFile)
    : AnimatedObject(pPlatformManager, rDataCacheManager, animationFile, "disappearing-animation")
{
    initializeMembers();

    setOnAnimationFinishedCallback([this]() -> void { destroy(); });
}

DisappearingAnimation::~DisappearingAnimation()
{
    initializeMembers();
}