#pragma once
#ifndef Player_hpp 
#define Player_hpp

#include <stdio.h>
#include "event/EventListener.hpp"
#include "core/AnimatedSprite.hpp"

class PlatformManager;
class Body;
class Body;

class Player : public EventListener, public AnimatedSprite
{
public:
    
    Player(PlatformManager* pPlatformManager, const Body* pBody, DataCacheManager& rDataCacheManager);
    ~Player();
    
    void receiveEvent(Event* pEvent) override;
    void update(float delta) override;

private:
    
    const Body* m_pBody;
    
    void initializeMembers()
    {
        m_pBody = nullptr;
    }
    
};

#endif /* Player_hpp */
