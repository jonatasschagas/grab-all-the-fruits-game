#pragma once
#ifndef Player_hpp 
#define Player_hpp

#include <stdio.h>
#include "event/EventListener.hpp"
#include "PhysicalAnimatedObject.hpp"
#include "core/Vector2.h"
#include "physics/PhysicsOnCollideListener.hpp"
#include "event/EventListener.hpp"

class PlatformManager;
class PhysicsBody;
class World;

class Player : public EventListener, public PhysicalAnimatedObject, public PhysicsOnCollideListener
{
public:
    
    Player(PlatformManager* pPlatformManager, PhysicsBody* pBody, DataCacheManager& rDataCacheManager, EventListener* pEventListener);
    
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

    bool isGrounded() const;

    PlatformManager* m_pPlatformManager;
    PhysicsBody* m_pBody;
    EventListener* m_pEventListener;
    string m_type;

    int m_numGroundContacts;
    int m_numFrontWallContacts;
    int m_numBackWallContacts;
    
    bool m_isDoubleJumping;
    
    void initializeMembers()
    {
        m_pBody = nullptr;
        m_pPlatformManager = nullptr;
        m_pEventListener = nullptr;
        m_type = "player";

        m_numGroundContacts = 0;
        m_numFrontWallContacts = 0;
        m_numBackWallContacts = 0;
        m_isDoubleJumping = false;
    }
    
};

#endif /* Player_hpp */
