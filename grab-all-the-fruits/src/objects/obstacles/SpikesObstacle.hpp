#pragma once
#ifndef SpikesObstacle_H
#define SpikesObstacle_H

#include <string>
#include "objects/PhysicalAnimatedObject.hpp"
#include "physics/PhysicsOnCollideListener.hpp"
#include "data/DataCacheManager.hpp"
#include "event/EventListener.hpp"

class PlatformManager;

using namespace std;

class SpikesObstacle : public PhysicalAnimatedObject, public PhysicsOnCollideListener
{
public:
    
    SpikesObstacle(
        PlatformManager* pPlatformManager, 
        DataCacheManager& rDataCacheManager, 
        PhysicsBody* pPhysicsBody,
        const string& animationFile, 
        const string& name, 
        const string& type, 
        EventListener* pEventListener);
    ~SpikesObstacle();

    const string& getName() const { return m_name; }

    void onCollide(PhysicsBody* pPhysicsBody) override;

private:

    string m_name;
    EventListener* m_pEventListener;

    void initializeMembers()
    {
        m_name = "";
        m_pEventListener = nullptr;
    }

};

#endif //SpikesObstacle_H
