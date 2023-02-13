#pragma once
#ifndef TogglePlatform_H
#define TogglePlatform_H

#include <string>
#include "objects/PhysicalAnimatedObject.hpp"
#include "physics/PhysicsOnCollideListener.hpp"
#include "data/DataCacheManager.hpp"
#include "event/EventListener.hpp"

class PlatformManager;

using namespace std;

const float MOVING_PLATFORM_SPEED = 150.f;

class TogglePlatform : public PhysicalAnimatedObject, public PhysicsOnCollideListener
{
public:
    
    TogglePlatform(
        PlatformManager* pPlatformManager, 
        DataCacheManager& rDataCacheManager, 
        PhysicsBody* pPhysicsBody,
        const string& animationFile, 
        const string& name, 
        const string& type, 
        EventListener* pEventListener,
        const Vector2& initialPosition,
        const Vector2& finalPosition,
        const float movingSpeed);
    ~TogglePlatform();

    void update(const float delta) override;

    void onCollideStart(PhysicsBody* pPhysicsBody) override;
    void onCollideEnd(PhysicsBody* pPhysicsBody) override;

private:

    EventListener* m_pEventListener;
    Vector2 m_initialPosition;
    Vector2 m_finalPosition;
    PhysicsBody* m_pPhysicsBody;
    float m_movingSpeed;
    bool m_isMoving;

    void initializeMembers()
    {
        m_pEventListener = nullptr;
        m_pPhysicsBody = nullptr;
        m_initialPosition.x = 0;
        m_initialPosition.y = 0;
        m_finalPosition.x = 0;
        m_finalPosition.y = 0;
        m_isMoving = true;
        m_movingSpeed = 0;
    }

};

#endif //TogglePlatform_H
