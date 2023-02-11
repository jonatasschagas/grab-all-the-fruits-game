#include "DisappearingAnimation.hpp"

#include <functional>

DisappearingAnimation::DisappearingAnimation(
    PlatformManager* pPlatformManager, 
    DataCacheManager& rDataCacheManager,
    const string& animationFile)
    : AnimatedObject(
        pPlatformManager, 
        rDataCacheManager, 
        animationFile, 
        "animation", 
        "disappearing-animation"
    )
{
    initializeMembers();

    setOnAnimationFinishedCallback("idle", [this]() -> void { destroy(); });
}

DisappearingAnimation::~DisappearingAnimation()
{
    initializeMembers();
}