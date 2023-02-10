#pragma once
#ifndef FireObstacle_H
#define FireObstacle_H

#include <string>
#include "objects/PhysicalAnimatedObject.hpp"
#include "physics/PhysicsOnCollideListener.hpp"
#include "data/DataCacheManager.hpp"
#include "event/EventListener.hpp"

class PlatformManager;

using namespace std;

class FireObstacle : public PhysicalAnimatedObject, public PhysicsOnCollideListener
{
public:
    
    FireObstacle(
        PlatformManager* pPlatformManager, 
        DataCacheManager& rDataCacheManager, 
        PhysicsBody* pPhysicsBody,
        const string& animationFile, 
        const string& name, 
        const string& type, 
        EventListener* pEventListener, 
        const float fireInterval,
        const Vector2& size);
    ~FireObstacle();

    const string& getName() const { return m_name; }

    void update(const float delta) override;

    void onSensorTriggeredStart(const string& name) override;

private:

    string m_name;
    EventListener* m_pEventListener;
    PhysicsBody* m_pPhysicsBody;
    float m_fireInterval;
    float m_fireTimer;
    bool m_fireOn;

    void initializeMembers()
    {
        m_name = "";
        m_pEventListener = nullptr;
        m_pPhysicsBody = nullptr;
        m_fireInterval = 0;
        m_fireTimer = 0;
        m_fireOn = false;
    }

};

#endif //FireObstacle_H
