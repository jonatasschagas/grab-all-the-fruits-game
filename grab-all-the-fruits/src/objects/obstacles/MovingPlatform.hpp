#pragma once
#ifndef MovingPlatform_H
#define MovingPlatform_H

#include <string>
#include "objects/PhysicalAnimatedObject.hpp"
#include "physics/PhysicsOnCollideListener.hpp"
#include "data/DataCacheManager.hpp"
#include "event/EventListener.hpp"

class PlatformManager;

using namespace std;

const float MOVING_PLATFORM_SPEED = 150.f;

class MovingPlatform : public PhysicalAnimatedObject, public PhysicsOnCollideListener
{
public:
    
    MovingPlatform(
        PlatformManager* pPlatformManager, 
        DataCacheManager& rDataCacheManager, 
        PhysicsBody* pPhysicsBody,
        const string& animationFile, 
        const string& name, 
        const string& type, 
        EventListener* pEventListener,
        const Vector2& initialPosition,
        const Vector2& finalPosition);
    ~MovingPlatform();

    const string& getName() const { return m_name; }

    void update(const float delta) override;

    void onCollide(PhysicsBody* pPhysicsBody) override;
    void onSensorTriggeredStart(const string& name) override;
    void onSensorTriggeredEnd(const string& name) override;

private:

    string m_name;
    EventListener* m_pEventListener;
    Vector2 m_initialPosition;
    Vector2 m_finalPosition;
    PhysicsBody* m_pPhysicsBody;

    void initializeMembers()
    {
        m_name = "";
        m_pEventListener = nullptr;
        m_pPhysicsBody = nullptr;
        m_initialPosition.x = 0;
        m_initialPosition.y = 0;
        m_finalPosition.x = 0;
        m_finalPosition.y = 0;
    }

};

#endif //MovingPlatform_H
