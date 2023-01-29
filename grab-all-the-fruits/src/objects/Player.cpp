#include "Player.hpp"

#include "platform/PlatformManager.h"
#include "logic/World.hpp"
#include <box2d/box2d.h>
#include "physics/PhysicsBody.hpp"

#include <string>

Player* Player::create(PlatformManager* pPlatformManager, World* pWorld, DataCacheManager& rDataCacheManager)
{
    //TODO: start position and size should be passed as parameters
    PhysicsBody* pBody = pWorld->createDynamicBody(Vector2(5, 15), Vector2(5, 5), 1, 0, 0, 1.0f);
    Player* pPlayer = new Player(pPlatformManager, pBody, rDataCacheManager);
    pBody->setGameObject(pPlayer);
    return pPlayer;
}

void Player::onCollide(PhysicsBody* pOtherBody)
{
    // TODO: compute here what to do...
    /*
    GameObject* pOtherGameObject = pOtherBody->getGameObject();
    if (pOtherGameObject != nullptr)
    {
        const string& type = pOtherGameObject->getType();
    }        
    */
}

Player::Player(PlatformManager* pPlatformManager, const PhysicsBody* pBody, DataCacheManager& rDataCacheManager) : AnimatedSprite(pPlatformManager, rDataCacheManager, "assets/characters/masked/masked_animation.json") 
{
    initializeMembers();

    m_pBody = pBody;
    m_pPlatformManager = pPlatformManager;
    setSize(m_pBody->getGameSize());
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

    const Vector2 linearVel = m_pBody->getVelocity();

    if (linearVel .y < 0) {
        play("jump");
        setFlip(linearVel .x < 0);
    }
    else if (linearVel .y > 0) {
        play("fall");
        setFlip(linearVel .x < 0);
    } 
    else if (linearVel .x != 0)
    {
        play("run");
        setFlip(linearVel .x < 0);
    }
    else
    {
        const bool flipped = isFlipped();
        play("idle");
        setFlip(flipped);
    }
    
    const Vector2 gamePosition = getGamePosition();
    setXY(gamePosition);
}

const Vector2 Player::getGamePosition()
{ 
    return m_pBody->getGamePosition(); 
}
