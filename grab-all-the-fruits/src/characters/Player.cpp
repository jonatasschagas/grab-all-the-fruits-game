#include "Player.hpp"

#include "platform/PlatformManager.h"
#include "logic/Body.hpp"

#include <string>

Player::Player(PlatformManager* pPlatformManager, const Body* pBody, DataCacheManager& rDataCacheManager) : AnimatedSprite(pPlatformManager, rDataCacheManager, "assets/characters/masked/masked_animation.json") 
{
    initializeMembers();

    m_pBody = pBody;
    setSize(5, 5);
    play("idle");
}

Player::~Player() {}
    
void Player::receiveEvent(Event* pEvent) 
{
}

void Player::update(float delta) 
{
    AnimatedSprite::update(delta);
    
    if (m_pBody == nullptr)
    {
        return;
    }

    if (m_pBody->m_doubleJumping && m_pBody->m_vy > 0) {
        play("double_jump");
    }
    else if (m_pBody->m_vy > 0) {
        play("jump");
    }
    else if (m_pBody->m_vy < 0) {
        play("fall");
    } 
    else if (m_pBody->m_vxGoal < 0 || m_pBody->m_vxGoal > 0)
    {
        play("run");
        setFlip(m_pBody->m_vxGoal < 0);
    }
    else
    {
        const bool flipped = isFlipped();
        play("idle");
        setFlip(flipped);
    }
    
    setXY(m_pBody->m_x, m_pBody->m_y);
}
