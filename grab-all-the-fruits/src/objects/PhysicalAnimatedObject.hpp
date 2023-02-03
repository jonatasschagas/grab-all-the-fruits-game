#pragma once
#ifndef PhysicalAnimatedObject_H
#define PhysicalAnimatedObject_H

#include <string>
#include "AnimatedObject.hpp"
#include "physics/PhysicsBody.hpp"

using namespace std;

class PhysicalAnimatedObject : public AnimatedObject
{
public:
    
    PhysicalAnimatedObject(PlatformManager* pPlatformManager, DataCacheManager& rDataCacheManager, PhysicsBody* pPhysicsBody, const string& animationFile, const string& type);
    ~PhysicalAnimatedObject();

private:

    PhysicsBody* m_pPhysicsBody;

    void initializeMembers()
    {
        m_pPhysicsBody = nullptr;
    }

};

#endif //PhysicalAnimatedObject_H
