#include "FireObstacle.hpp"


FireObstacle::FireObstacle(
    PlatformManager* pPlatformManager, 
    DataCacheManager& rDataCacheManager, 
    PhysicsBody* pPhysicsBody,
    const string& animationFile, 
    const string& name, 
    const string& type, 
    EventListener* pEventListener, 
    const float fireInterval,
    const Vector2& size) : 
    PhysicalAnimatedObject(
        pPlatformManager, 
        rDataCacheManager, 
        pPhysicsBody, 
        animationFile, 
        type), 
    PhysicsOnCollideListener()
{
    initializeMembers();

    m_name = name;
    m_pEventListener = pEventListener;
    m_fireInterval = fireInterval;
    m_fireTimer = 0;
    m_fireOn = false;

    play("fire_off");
    
    m_pPhysicsBody = pPhysicsBody;
    m_pPhysicsBody->addSensor("fire", Vector2(0, size.y/2.f), Vector2(size.x, size.y/2.f));
}

FireObstacle::~FireObstacle()
{
    initializeMembers();
}

void FireObstacle::update(const float delta)
{
    PhysicalAnimatedObject::update(delta);

    m_fireTimer += delta;
    if (m_fireTimer >= m_fireInterval)
    {
        m_fireTimer = 0;
        m_fireOn = !m_fireOn;
        if (m_fireOn)
        {
            play("fire_on");
        }
        else
        {
            play("fire_off");
        }
    }
}

void FireObstacle::onSensorTriggeredStart(const string& name, PhysicsBody* pOtherBody)
{
    if (name.compare("fire") == 0 && m_fireOn)
    {
        Event event("player-died");
        event.setInputCoordinates(getGamePosition());
        m_pEventListener->receiveEvent(&event);
    }
}
