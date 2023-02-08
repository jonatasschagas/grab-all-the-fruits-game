#include "MovingPlatform.hpp"

MovingPlatform::MovingPlatform(
        PlatformManager* pPlatformManager, 
        DataCacheManager& rDataCacheManager, 
        PhysicsBody* pPhysicsBody,
        const string& animationFile, 
        const string& name, 
        const string& type, 
        EventListener* pEventListener,
        const Vector2& initialPosition,
        const Vector2& finalPosition) : PhysicalAnimatedObject(
            pPlatformManager, 
            rDataCacheManager, 
            pPhysicsBody, 
            animationFile, 
            type), PhysicsOnCollideListener()
{
    initializeMembers();

    m_name = name;
    m_pEventListener = pEventListener;
    m_initialPosition = initialPosition;
    m_finalPosition = finalPosition;
    m_pPhysicsBody = pPhysicsBody;
}
MovingPlatform::~MovingPlatform()
{
    initializeMembers();
}

void MovingPlatform::update(const float delta)
{
    PhysicalAnimatedObject::update(delta);

    Vector2 currentPosition = getGamePosition();
    Vector2 direction = m_finalPosition - m_initialPosition;
    direction.normalize();
    Vector2 newPosition = currentPosition + direction * delta * 10.0f;
    if (direction.x > 0.0f && newPosition.x > m_finalPosition.x)
    {
        newPosition = m_finalPosition;
    }
    else if (direction.x < 0.0f && newPosition.x < m_finalPosition.x)
    {
        newPosition = m_finalPosition;
    }
    else if (direction.y > 0.0f && newPosition.y > m_finalPosition.y)
    {
        newPosition = m_finalPosition;
    }
    else if (direction.y < 0.0f && newPosition.y < m_finalPosition.y)
    {
        newPosition = m_finalPosition;
    }
    
    if (newPosition == m_finalPosition)
    {
        Vector2 temp = m_initialPosition;
        m_initialPosition = m_finalPosition;
        m_finalPosition = temp;
    }

    m_pPhysicsBody->setVelocity(direction * delta * MOVING_PLATFORM_SPEED);

    const Vector2 gamePosition = m_pPhysicsBody->getGamePosition();
    setXY(gamePosition);
}

void MovingPlatform::onCollide(PhysicsBody* pPhysicsBody)
{

}

void MovingPlatform::onSensorTriggeredStart(const string& name)
{

}

void MovingPlatform::onSensorTriggeredEnd(const string& name) 
{

}