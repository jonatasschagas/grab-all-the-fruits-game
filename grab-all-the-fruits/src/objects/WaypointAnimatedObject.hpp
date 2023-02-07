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

    const string& getName() const { return m_name; }

    void onCollide(PhysicsBody* pPhysicsBody) override;
    void onSensorTriggeredStart(const string& name) override;
    void onSensorTriggeredEnd(const string& name) override;

private:

    string m_name;
    EventListener* m_pEventListener;

    void initializeMembers()
    {
        m_name = "";
        m_pEventListener = nullptr;
    }

};

#endif //WaypointAnimatedObject_H
