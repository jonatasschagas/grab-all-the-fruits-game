#include "Player.hpp"

#include "platform/PlatformManager.h"
#include "logic/Body.hpp"
#include "logic/World.hpp"
#include <box2d/box2d.h>

#include <string>

Player* Player::create(PlatformManager* pPlatformManager, World* pWorld, DataCacheManager& rDataCacheManager)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    //TODO: Player should be created based on position configured in the map
    bodyDef.position.Set(.5f, 10.0f);

    b2CircleShape circleShape;
    circleShape.m_radius = 0.5f; // 1 meter diameter

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circleShape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.f;
    
    const Body* pBody = pWorld->createBody("player", bodyDef, fixtureDef);

    Player* pPlayer = new Player(pPlatformManager, pBody, rDataCacheManager);
    return pPlayer;
}

Player::Player(PlatformManager* pPlatformManager, const Body* pBody, DataCacheManager& rDataCacheManager) : AnimatedSprite(pPlatformManager, rDataCacheManager, "assets/characters/masked/masked_animation.json") 
{
    initializeMembers();

    m_pBody = pBody;
    const b2Vec2& size = m_pBody->getSizeInPixels();
    setSize(size.x, size.y);
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

    if (m_pBody->m_pb2Body->GetLinearVelocity().y < 0) {
        play("jump");
    }
    else if (m_pBody->m_pb2Body->GetLinearVelocity().y > 0) {
        play("fall");
    } 
    else if (m_pBody->m_pb2Body->GetLinearVelocity().x != 0)
    {
        play("run");
        setFlip(m_pBody->m_pb2Body->GetLinearVelocity().x < 0);
    }
    else
    {
        const bool flipped = isFlipped();
        play("idle");
        setFlip(flipped);
    }
    
    const b2Vec2& position = m_pBody->getPositionInPixels();
    setXYInvertedY(position.x, position.y);
}
