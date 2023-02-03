#pragma once
#ifndef DisappearingAnimation_H
#define DisappearingAnimation_H

#include <string>
#include "AnimatedObject.hpp"
#include "data/DataCacheManager.hpp"

class PlatformManager;

using namespace std;

class DisappearingAnimation : public AnimatedObject
{
public:
    
    DisappearingAnimation(
        PlatformManager* pPlatformManager, 
        DataCacheManager& rDataCacheManager,
        const string& animationFile);
    ~DisappearingAnimation();

private:

    void initializeMembers()
    {
    }

};

#endif //DisappearingAnimation_H
