#include "TogglePlatform.hpp"

#include "objects/GameObject.hpp"

TogglePlatform::TogglePlatform(
        PlatformManager* pPlatformManager, 
        DataCacheManager& rDataCacheManager, 
        PhysicsBody* pPhysicsBody,
        const string& animationFile, 
        const string& name, 
        const string& type, 
        EventListener* pEventListener,
        const Vector2& initialPosition,
        const Vector2& finalPosition,
        const float movingSpeed) : PhysicalAnimatedObject(
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
    m_movingSpeed = movingSpeed;

    play("moving");
}

TogglePlatform::~TogglePlatform()
{
    initializeMembers();
}

void TogglePlatform::update(const float delta)
{
    PhysicalAnimatedObject::update(delta);

    if (!m_isMoving)
    {
        m_pPhysicsBody->setVelocity(Vector2(0, 0));   
    }
    else
    {
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

        m_pPhysicsBody->setVelocity(direction * delta * m_movingSpeed);
    }

    const Vector2 gamePosition = m_pPhysicsBody->getGamePosition();
    setXY(gamePosition);
}

void TogglePlatform::onCollideStart(PhysicsBody* pPhysicsBody)
{
    GameObject* pGameObject = pPhysicsBody->getGameObject();
    if (pGameObject != nullptr && pGameObject->getType().compare("player") == 0)
    {
        m_isMoving = false;
        play("stop");
    }
}

void TogglePlatform::onCollideEnd(PhysicsBody* pPhysicsBody)
{
    GameObject* pGameObject = pPhysicsBody->getGameObject();
    if (pGameObject != nullptr && pGameObject->getType().compare("player") == 0)
    {
        m_isMoving = true;
        play("moving");
    }
}