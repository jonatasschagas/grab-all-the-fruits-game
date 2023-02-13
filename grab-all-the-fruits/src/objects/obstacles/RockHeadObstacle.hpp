#pragma once
#ifndef RockHeadObstacle_H
#define RockHeadObstacle_H

#include <string>
#include "objects/PhysicalAnimatedObject.hpp"
#include "physics/PhysicsOnCollideListener.hpp"
#include "data/DataCacheManager.hpp"
#include "event/EventListener.hpp"

class PlatformManager;

using namespace std;

enum RockHeadObstacleState
{
    RockHeadObstacleState_Idle,
    RockHeadObstacleState_Moving,
    RockHeadObstacleState_Falling,
    RockHeadObstacleState_Crushed
};

class RockHeadObstacle : public PhysicalAnimatedObject, public PhysicsOnCollideListener
{
public:
    
    RockHeadObstacle(
        PlatformManager* pPlatformManager, 
        DataCacheManager& rDataCacheManager, 
        PhysicsBody* pPhysicsBody,
        const string& animationFile, 
        const string& name, 
        const string& type, 
        EventListener* pEventListener,
        const Vector2& initialPosition,
        const float idleTime,
        const float crushTime);
        
    ~RockHeadObstacle();

    void onCollideStart(PhysicsBody* pPhysicsBody) override;
    
    void update(const float delta) override;

private:

    EventListener* m_pEventListener;
    PhysicsBody* m_pPhysicsBody;
    Vector2 m_targetPosition;
    Vector2 m_initialPosition;
    Vector2 m_velocity;
    RockHeadObstacleState m_state;
    float m_stateAccumulator;

    float m_idleTime;
    float m_crushTime;
    bool m_goingUp;
    
    void initializeMembers()
    {
        m_pEventListener = nullptr;
        m_pPhysicsBody = nullptr;
        m_state = RockHeadObstacleState_Idle;
        m_targetPosition = Vector2::ZERO;
        m_initialPosition = Vector2::ZERO;
        m_velocity = Vector2::ZERO;
        m_idleTime = 0;
        m_crushTime = 0;
        m_goingUp = false;
    }

};

#endif //RockHeadObstacle_H
