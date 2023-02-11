#include "FallingBlockObstacle.hpp"

FallingBlockObstacle::FallingBlockObstacle(
        PlatformManager* pPlatformManager, 
        DataCacheManager& rDataCacheManager, 
        PhysicsBody* pPhysicsBody,
        const string& animationFile, 
        const string& name, 
        const string& type, 
        EventListener* pEventListener, 
        const Vector2& size,
        const Vector2& tileSize,
        const int distanceToGroundInTiles) :
         PhysicalAnimatedObject(
            pPlatformManager, 
            rDataCacheManager, 
            pPhysicsBody, 
            animationFile, 
            type,
            name), 
        PhysicsOnCollideListener()
{
    initializeMembers();

    Vector2 sensorSize = size;
    sensorSize.y = tileSize.y * distanceToGroundInTiles;

    m_pEventListener = pEventListener;
    m_pPhysicsBody = pPhysicsBody;
    m_pPhysicsBody->addSensor("falling-block-sensor", Vector2(0, -sensorSize.y * 0.65f), sensorSize);
    
    // disable gravity
    m_pPhysicsBody->setGravityScale(0);
}

FallingBlockObstacle::~FallingBlockObstacle()
{
    initializeMembers();
}

void FallingBlockObstacle::onSensorTriggeredStart(const string& name, PhysicsBody* pOtherBody)
{
    GameObject* pOtherGameObject = pOtherBody->getGameObject();
    if (name.compare("falling-block-sensor") == 0 && pOtherGameObject != nullptr && pOtherGameObject->getType().compare("player") == 0)
    {
        // enable gravity
        m_pPhysicsBody->setGravityScale(1);
        m_triggered = true;
    }
}

void FallingBlockObstacle::onCollideStart(PhysicsBody* pPhysicsBody) 
{
    GameObject* pGameObject = pPhysicsBody->getGameObject();
    if (pGameObject != nullptr && pGameObject->getType().compare("player") == 0 && m_triggered && !m_hasFallen)
    {
        Event event("player-died");
        event.setInputCoordinates(getGamePosition());
        m_pEventListener->receiveEvent(&event);
    }
    else if (pGameObject != nullptr && pGameObject->getType().compare("ground") == 0 && m_triggered)
    {
        m_hasFallen = true;
    }
}

void FallingBlockObstacle::update(const float delta)
{
    PhysicalAnimatedObject::update(delta);

    if (m_hasFallen && m_triggered)
    {
        // cancel the velocity, so player cant push it around
        m_pPhysicsBody->setVelocity(Vector2(0, 0));
    }

    const Vector2& position = m_pPhysicsBody->getGamePosition();
    setPosition(position);
}
