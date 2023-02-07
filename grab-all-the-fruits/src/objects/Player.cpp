#include "Player.hpp"

#include "platform/PlatformManager.h"
#include "logic/World.hpp"
#include <box2d/box2d.h>
#include "physics/PhysicsBody.hpp"
#include "logic/GameConfiguration.h"
#include "utils/StringUtils.h"
#include <math.h>

#if IMGUI_ENABLED
#include "imgui/imgui.h"
#endif

#include <string>

Player::Player(
    PlatformManager* pPlatformManager, 
    PhysicsBody* pBody, 
    DataCacheManager& rDataCacheManager,
    EventListener* pEventListener) : 
    PhysicalAnimatedObject(
        pPlatformManager, 
        rDataCacheManager, 
        pBody, 
        "assets/characters/masked/masked_animation.json", 
        "player"
    ) 
{
    initializeMembers();

    m_pBody = pBody;
    m_pPlatformManager = pPlatformManager;
    m_pEventListener = pEventListener;

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
        if (isGrounded()) 
        {
            m_pBody->applyForce(Vector2(0, PLAYER_JUMPING_FORCE), PhysicsForceType::PhysicsForceTypeImpulse);
        } 
        else if (!m_isDoubleJumping && linearVel.y > 0)
        {
            m_pBody->applyForce(Vector2(0, PLAYER_JUMPING_FORCE * 0.5f), PhysicsForceType::PhysicsForceTypeImpulse);
            m_isDoubleJumping = true;
            play("double-jump");
        }
    }
    else if (pEvent->getName().compare("fruit-collected") == 0) {
        m_numFruitsCollected++;
        Event event("update-fruit-collected-hud");
        event.setData(m_numFruitsCollected);
        m_pEventListener->receiveEvent(&event);
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
    if (linearVel.y > PLAYER_JUMPING_FORCE)
    {
        // cap the jumping force
        m_pBody->setVelocity(Vector2(linearVel.x, PLAYER_JUMPING_FORCE));
    }

    if (linearVel.y > 0 && !m_isDoubleJumping)
    {
        play("jump");
        setFlip(linearVel .x < 0);
    }
    else if (!isGrounded() && linearVel .y < 0) {
        play("fall");
        setFlip(linearVel .x < 0);
    } 
    else if (isGrounded() && abs(linearVel.x) > 0.5f)
    {
        play("run");
        setFlip(linearVel .x < 0);
    }
    else if (isGrounded())
    {
        const bool flipped = isFlipped();
        play("idle");
        setFlip(flipped);
    }
    
    const Vector2 gamePosition = getGamePosition();
    setXY(gamePosition);

    if (isGrounded())
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
        m_numGroundContacts++;
    }
    else if (name.compare("front-sensor") == 0) {
        m_numFrontWallContacts++;
    }
    else if (name.compare("back-sensor") == 0) {
        m_numBackWallContacts++;
    }
}

void Player::onSensorTriggeredEnd(const string& name) 
{
    if (name.compare("bottom-sensor") == 0) {
        m_numGroundContacts--;
    }
    else if (name.compare("front-sensor") == 0) {
        m_numFrontWallContacts--;
    }
    else if (name.compare("back-sensor") == 0) {
        m_numBackWallContacts--;
    }
}

void Player::updateEditor()
{
    #if IMGUI_ENABLED
    
    ImGui::Begin("Player", nullptr);
    
    ImGui::Text("Touching front wall: %s", (m_numFrontWallContacts > 0 ? "true" : "false"));
    ImGui::Text("Touching back wall: %s", (m_numBackWallContacts > 0 ? "true" : "false"));
    ImGui::Text("Is Grounded: %s", (m_numGroundContacts > 0 ? "true" : "false"));
    ImGui::Text("Current Animation: %s", getCurrentAnimationName().c_str());
    
    const Vector2 linearVel = m_pBody->getVelocity();
    ImGui::Text("Linear Velocity: %f,%f", linearVel.x, linearVel.y);
    
    ImGui::End();

    #endif
}

bool Player::isGrounded() const
{
    return m_numGroundContacts > 0;
}
