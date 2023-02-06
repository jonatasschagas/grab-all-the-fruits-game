#pragma once
#ifndef Player_hpp 
#define Player_hpp

#include <stdio.h>
#include "event/EventListener.hpp"
#include "PhysicalAnimatedObject.hpp"
#include "core/Vector2.h"
#include "physics/PhysicsOnCollideListener.hpp"

class PlatformManager;
class PhysicsBody;
class World;

class Player : public EventListener, public PhysicalAnimatedObject, public PhysicsOnCollideListener
{
public:
    
    Player(PlatformManager* pPlatformManager, PhysicsBody* pBody, DataCacheManager& rDataCacheManager);
    
    ~Player();
    
    void receiveEvent(Event* pEvent) override;
    void update(float delta) override;

    void onCollide(PhysicsBody* pOtherBody) override;
    
    const string& getType() const override { return m_type; }

    const Vector2 getGamePosition();

    void onSensorTriggeredStart(const string& name) override;
    void onSensorTriggeredEnd(const string& name) override;    

    void updateEditor();

private:
    
    PlatformManager* m_pPlatformManager;
    PhysicsBody* m_pBody;
    string m_type;

    bool m_isGrounded;
    bool m_isDoubleJumping;
    bool m_isTouchingFrontWall;
    bool m_isTouchingBackWall;

    void initializeMembers()
    {
        m_pBody = nullptr;
        m_pPlatformManager = nullptr;
        m_type = "player";

        m_isGrounded = false;
        m_isDoubleJumping = false;
        m_isTouchingFrontWall = false;
        m_isTouchingBackWall = false;
    }
    
};

#endif /* Player_hpp */
