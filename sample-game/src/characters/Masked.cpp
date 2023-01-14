#include "Masked.hpp"

Masked::Masked(DataCacheManager& rDataCacheManager) : AnimatedSprite(rDataCacheManager, "assets/characters/masked/masked_animation.json") 
{
    initializeMembers();

    setSize(12, 12);
    play("idle");
}

Masked::~Masked() {}
    
void Masked::receiveEvent(Event* pEvent) 
{
}

void Masked::update(float delta) 
{
    AnimatedSprite::update(delta);
}
