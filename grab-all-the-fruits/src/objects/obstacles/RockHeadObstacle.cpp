#include "RockHeadObstacle.hpp"

#include "objects/GameObject.hpp"

 RockHeadObstacle::RockHeadObstacle(
        PlatformManager* pPlatformManager, 
        DataCacheManager& rDataCacheManager, 
        PhysicsBody* pPhysicsBody,
        const string& animationFile, 
        const string& name, 
        const string& type, 
        EventListener* pEventListener,
        const Vector2& initialPosition,
        const float idleTime,
        const float crushTime) : 
        PhysicalAnimatedObject(
            pPlatformManager, 
            rDataCacheManager, 
            pPhysicsBody, 
            animationFile, 
            type, 
            name
        ),
        PhysicsOnCollideListener()
{
    initializeMembers();
    m_pEventListener = pEventListener;
    m_pPhysicsBody = pPhysicsBody;
    m_initialPosition = initialPosition;
    m_idleTime = idleTime;
    m_crushTime = crushTime;
    m_state = RockHeadObstacleState_Idle;
}

RockHeadObstacle::~RockHeadObstacle()
{
    initializeMembers();
}

void RockHeadObstacle::onCollideStart(PhysicsBody* pPhysicsBody)
{
    GameObject* pGameObject = pPhysicsBody->getGameObject();
    if (pGameObject->getName().compare("ground") == 0)
    {
        Event event("ground-collide");
        //TODO: Parameterize this -> intensity of the collision for camera shake
        event.setFloatData(15);
        m_pEventListener->receiveEvent(&event);
        m_state = RockHeadObstacleState_Crushed;
        m_stateAccumulator = 0;
        playSoundEffect("thud");
    }
    else if (pPhysicsBody->getGameObject()->getType().compare("player") == 0)
    {
        Event event("player-died");
        event.setInputCoordinates(pPhysicsBody->getGamePosition());
        m_pEventListener->receiveEvent(&event);
    }
}

void RockHeadObstacle::update(const float delta)
{
    PhysicalAnimatedObject::update(delta);

    switch (m_state)
    {
        case RockHeadObstacleState_Idle:
        {
            play("blink");
            m_stateAccumulator += delta;
            if (m_stateAccumulator > m_idleTime)
            {
                m_stateAccumulator = 0;
                m_state = RockHeadObstacleState_Moving;
                m_goingUp = false;
                m_pPhysicsBody->setGravityScale(1);
                m_pPhysicsBody->setVelocity(Vector2::ZERO);
                play("idle");
            }
            break;
        }
        case RockHeadObstacleState_Moving:
        {
            Vector2 gamePosition = m_pPhysicsBody->getGamePosition();
            if (abs(gamePosition.distance(m_initialPosition)) < 1 && m_goingUp)
            {
                m_goingUp = false;
                m_pPhysicsBody->setGravityScale(0);
                m_state = RockHeadObstacleState_Idle;
                m_pPhysicsBody->setVelocity(Vector2::ZERO);
                m_stateAccumulator = 0;
            }
            break;
        }
        case RockHeadObstacleState_Falling:
        {
            break;
        }
        case RockHeadObstacleState_Crushed:
        {
            m_stateAccumulator += delta;
            play("bottom");
            if (m_stateAccumulator > m_crushTime)
            {
                m_stateAccumulator = 0;
                m_state = RockHeadObstacleState_Moving;
                m_pPhysicsBody->setGravityScale(-1);
                m_goingUp = true;
            }
            break;
        }
        default:
        {
            break;
        }
    }

    const Vector2 gamePosition = m_pPhysicsBody->getGamePosition();
    setPosition(gamePosition);
}