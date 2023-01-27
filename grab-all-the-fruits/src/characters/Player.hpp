#pragma once
#ifndef Player_hpp 
#define Player_hpp

#include <stdio.h>
#include "event/EventListener.hpp"
#include "core/AnimatedSprite.hpp"

class PlatformManager;
class Body;
class World;

class Player : public EventListener, public AnimatedSprite
{
public:
    
    ~Player();
    
    void receiveEvent(Event* pEvent) override;
    void update(float delta) override;

    static Player* create(PlatformManager* pPlatformManager, World* pWorld, DataCacheManager& rDataCacheManager);

private:
    
    Player(PlatformManager* pPlatformManager, const Body* pBody, DataCacheManager& rDataCacheManager);
    
    const Body* m_pBody;
    
    void initializeMembers()
    {
        m_pBody = nullptr;
    }
    
};

#endif /* Player_hpp */
