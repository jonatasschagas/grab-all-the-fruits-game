#pragma once
#ifndef StationaryObstacle_H
#define StationaryObstacle_H

#include <string>
#include "objects/PhysicalAnimatedObject.hpp"
#include "physics/PhysicsOnCollideListener.hpp"
#include "data/DataCacheManager.hpp"
#include "event/EventListener.hpp"

class PlatformManager;

using namespace std;

class StationaryObstacle : public PhysicalAnimatedObject, public PhysicsOnCollideListener
{
public:
    
    StationaryObstacle(
        PlatformManager* pPlatformManager, 
        DataCacheManager& rDataCacheManager, 
        PhysicsBody* pPhysicsBody,
        const string& animationFile, 
        const string& name, 
        const string& type, 
        EventListener* pEventListener);
    ~StationaryObstacle();

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

#endif //StationaryObstacle_H
