#pragma once
#ifndef SawObstacle_H
#define SawObstacle_H

#include <string>
#include "objects/PhysicalAnimatedObject.hpp"
#include "physics/PhysicsOnCollideListener.hpp"
#include "data/DataCacheManager.hpp"
#include "event/EventListener.hpp"
#include "core/Vector2.h"

using namespace std;

class PlatformManager;

const float SAW_SPEED = 150.f;

class SawObstacle : public PhysicalAnimatedObject, public PhysicsOnCollideListener
{
public:
    
    SawObstacle(
        PlatformManager* pPlatformManager, 
        DataCacheManager& rDataCacheManager, 
        PhysicsBody* pPhysicsBody,
        const string& animationFile, 
        const string& name, 
        const string& type, 
        EventListener* pEventListener,
        const Vector2& initialPosition,
        const Vector2& finalPosition);
    ~SawObstacle();

    void update(const float delta) override;

    void onCollideStart(PhysicsBody* pPhysicsBody) override;

private:

    EventListener* m_pEventListener;
    Vector2 m_initialPosition;
    Vector2 m_finalPosition;
    PhysicsBody* m_pPhysicsBody;

    void initializeMembers()
    {
        m_pEventListener = nullptr;
        m_initialPosition = Vector2::ZERO;
        m_finalPosition = Vector2::ZERO;
        m_pPhysicsBody = nullptr;
    }

};

#endif //SawObstacle_H
