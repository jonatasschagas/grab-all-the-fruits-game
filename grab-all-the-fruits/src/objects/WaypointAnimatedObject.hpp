#pragma once
#ifndef WaypointAnimatedObject_H
#define WaypointAnimatedObject_H

#include <string>
#include "AnimatedObject.hpp"
#include "physics/PhysicsOnCollideListener.hpp"
#include "data/DataCacheManager.hpp"
#include "event/EventListener.hpp"

class PlatformManager;

using namespace std;

class WaypointAnimatedObject : public AnimatedObject, public PhysicsOnCollideListener
{
public:
    
    WaypointAnimatedObject(PlatformManager* pPlatformManager, DataCacheManager& rDataCacheManager, EventListener* pEventListener, const string& animationFile, const string& name, const string& type);
    ~WaypointAnimatedObject();

    void onCollideStart(PhysicsBody* pPhysicsBody) override;

private:

    EventListener* m_pEventListener;

    void initializeMembers()
    {
        m_pEventListener = nullptr;
    }

};

#endif //WaypointAnimatedObject_H
