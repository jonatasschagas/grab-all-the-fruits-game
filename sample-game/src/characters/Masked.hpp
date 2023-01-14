#ifndef Masked_hpp
#define Masked_hpp

#include <stdio.h>
#include "event/EventListener.hpp"
#include "core/AnimatedSprite.hpp"

class Masked : public EventListener, public AnimatedSprite
{
public:
    
    Masked(DataCacheManager& rDataCacheManager);
    ~Masked();
    
    void receiveEvent(Event* pEvent) override;
    void update(float delta) override;

private:
    
    void initializeMembers()
    {
    }
    
};

#endif /* Player_hpp */
