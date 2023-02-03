#pragma once
#ifndef WaypointAnimatedObject_H
#define WaypointAnimatedObject_H

#include <string>
#include "AnimatedObject.hpp"
#include "physics/PhysicsOnCollideListener.hpp"
#include "data/DataCacheManager.hpp"

class PlatformManager;

using namespace std;

class WaypointAnimatedObject : public AnimatedObject, public PhysicsOnCollideListener
{
public:
    
    WaypointAnimatedObject(PlatformManager* pPlatformManager, DataCacheManager& rDataCacheManager, const string& animationFile, const string& name, const string& type);
    ~WaypointAnimatedObject();

    const string& getName() const { return m_name; }

    void onCollide(PhysicsBody* pPhysicsBody) override;

private:

    string m_name;

    void initializeMembers()
    {
        m_name = "";
    }

};

#endif //WaypointAnimatedObject_H
