#pragma once
#ifndef Player_hpp 
#define Player_hpp

#include <stdio.h>
#include "event/EventListener.hpp"
#include "core/AnimatedSprite.hpp"
#include "GameObject.hpp"
#include "physics/PhysicsOnCollideListener.hpp"

class PlatformManager;
class PhysicsBody;
class World;

class Player : public EventListener, public AnimatedSprite, public PhysicsOnCollideListener, public GameObject
{
public:
    
    ~Player();
    
    void receiveEvent(Event* pEvent) override;
    void update(float delta) override;

    static Player* create(PlatformManager* pPlatformManager, World* pWorld, DataCacheManager& rDataCacheManager);

    void onCollide(PhysicsBody* pOtherBody) override;
    const string& getType() const override { return m_type; }

    const Vector2 getGamePosition();

private:
    
    Player(PlatformManager* pPlatformManager, const PhysicsBody* pBody, DataCacheManager& rDataCacheManager);
    
    PlatformManager* m_pPlatformManager;
    const PhysicsBody* m_pBody;
    string m_type;

    void initializeMembers()
    {
        m_pBody = nullptr;
        m_pPlatformManager = nullptr;
        m_type = "player";
    }
    
};

#endif /* Player_hpp */
