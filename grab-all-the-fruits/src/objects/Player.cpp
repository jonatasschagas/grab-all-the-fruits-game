#include "Player.hpp"

#include "platform/PlatformManager.h"
#include "logic/World.hpp"
#include <box2d/box2d.h>
#include "physics/PhysicsBody.hpp"
#include "logic/GameConfiguration.h"
#include "utils/StringUtils.h"

#if IMGUI_ENABLED
#include "imgui/imgui.h"
#endif

#include <string>

Player::Player(
    PlatformManager* pPlatformManager, 
    PhysicsBody* pBody, 
    DataCacheManager& rDataCacheManager) : PhysicalAnimatedObject(pPlatformManager, rDataCacheManager, pBody, "assets/characters/masked/masked_animation.json", "player") 
{
    initializeMembers();

    m_pBody = pBody;
    m_pPlatformManager = pPlatformManager;
    setSize(m_pBody->getGameSize());
    play("idle");

    float width = getSize().x;
    float height = getSize().y;

    pBody->addSensor("front-sensor", Vector2(width/2, 0), Vector2(width/2, height/2));
    pBody->addSensor("back-sensor", Vector2(-width/2, 0), Vector2(width/2, height/2));
    pBody->addSensor("bottom-sensor", Vector2(0, -height/2), Vector2(width/2, height/2));

    setOnAnimationFinishedCallback("double-jump", [this]() {
        play("fall");
    });
}

Player::~Player() {}
    
void Player::receiveEvent(Event* pEvent) 
{
    if (pEvent->getName().compare("right_start") == 0) {
        m_pBody->applyInstantForce(Vector2(PLAYER_RUNNING_SPEED, 0));
    } 
    else if (pEvent->getName().compare("right_stop") == 0) {
        m_pBody->applyInstantForce(Vector2(0, 0));
    } 
    else if (pEvent->getName().compare("left_start") == 0) {
        m_pBody->applyInstantForce(Vector2(-PLAYER_RUNNING_SPEED, 0));
    } 
    else if (pEvent->getName().compare("left_stop") == 0) {
        m_pBody->applyInstantForce(Vector2(0, 0));
    } 
    else if (pEvent->getName().compare("space_start") == 0) {
        
        const Vector2 linearVel = m_pBody->getVelocity();
        if (m_isGrounded) 
        {
            m_pBody->applyForce(Vector2(0, PLAYER_JUMPING_FORCE), PhysicsForceType::PhysicsForceTypeImpulse);
            play("jump");
        } 
        else if (!m_isDoubleJumping && linearVel.y > 0)
        {
            m_pBody->applyForce(Vector2(0, PLAYER_JUMPING_FORCE * 0.5f), PhysicsForceType::PhysicsForceTypeImpulse);
            m_isDoubleJumping = true;
            play("double-jump");
        }
    }
}

void Player::update(float delta) 
{
    AnimatedSprite::update(delta);
    
    if (m_pBody == nullptr)
    {
        return;
    }

    const Vector2 linearVel = m_pBody->getVelocity();

    if (!m_isGrounded && linearVel .y < 0) {
        play("fall");
        setFlip(linearVel .x < 0);
    } 
    else if (m_isGrounded && linearVel .x != 0)
    {
        play("run");
        setFlip(linearVel .x < 0);
    }
    else if (m_isGrounded)
    {
        const bool flipped = isFlipped();
        play("idle");
        setFlip(flipped);
    }
    
    const Vector2 gamePosition = getGamePosition();
    setXY(gamePosition);

    if (m_isGrounded)
    {
        m_isDoubleJumping = false;
    }
}

const Vector2 Player::getGamePosition()
{ 
    return m_pBody->getGamePosition(); 
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

void Player::onSensorTriggeredStart(const string& name) 
{
    if (name.compare("bottom-sensor") == 0) {
        m_isGrounded = true;
    }
    else if (name.compare("front-sensor") == 0) {
        m_isTouchingFrontWall = true;
    }
    else if (name.compare("back-sensor") == 0) {
        m_isTouchingBackWall = true;
    }
}

void Player::onSensorTriggeredEnd(const string& name) 
{
    if (name.compare("bottom-sensor") == 0) {
        m_isGrounded = false;
    }
    else if (name.compare("front-sensor") == 0) {
        m_isTouchingFrontWall = false;
    }
    else if (name.compare("back-sensor") == 0) {
        m_isTouchingBackWall = false;
    }
}

void Player::updateEditor()
{
    #if IMGUI_ENABLED
    
    ImGui::Begin("Player", nullptr);
    
    ImGui::Text("Touching front wall: %s", (m_isTouchingFrontWall ? "true" : "false"));
    ImGui::Text("Touching back wall: %s", (m_isTouchingBackWall ? "true" : "false"));
    ImGui::Text("Is Grounded: %s", (m_isGrounded ? "true" : "false"));
    
    ImGui::End();

    #endif
}
