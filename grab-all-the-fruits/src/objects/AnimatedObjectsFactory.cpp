#include "AnimatedObjectsFactory.hpp"

#include "WaypointAnimatedObject.hpp"
#include "Player.hpp"
#include "AnimatedObject.hpp"
#include "event/EventListener.hpp"
#include "platform/PlatformManager.h"
#include "logic/World.hpp"

AnimatedObjectsFactory::AnimatedObjectsFactory(const string& animatedObjectsPath, PlatformManager* pPlatformManager, DataCacheManager& rDataCacheManager, World* pWorld, EventListener* pEventListener) : m_rDataCacheManager(rDataCacheManager)
{
    initializeMembers();

    m_pPlatformManager = pPlatformManager;
    m_animatedObjectsPath = animatedObjectsPath;
    m_pEventListener = pEventListener;
    m_pWorld = pWorld;
}

AnimatedObjectsFactory::~AnimatedObjectsFactory()
{
    initializeMembers();
}

Sprite* AnimatedObjectsFactory::createMetaTile(const TileConfig* pTileConfig, const Vector2& position, const Vector2& size)
{
    const string& objectName = pTileConfig->getProperty("name");
    const string& objectType = pTileConfig->getProperty("type");

    string animationFile = m_animatedObjectsPath + "/" + objectName + "_animation.json";

    AnimatedObject* pAnimatedObject = nullptr;

    if (objectType.compare("waypoint") == 0)
    {
        if (objectName.compare("start-level") == 0)
        {
            Event event;
            event.setName("create-player");
            event.setInputCoordinates(position);
            m_pEventListener->receiveEvent(&event);
            return nullptr;
        }
        else
        {
            pAnimatedObject = createWaypoint(animationFile, objectName, position, size);
        }
    } 
    else 
    {
        pAnimatedObject = new AnimatedObject(m_pPlatformManager, m_rDataCacheManager, animationFile, objectType);
    }
    
    return pAnimatedObject;
}

AnimatedObject* AnimatedObjectsFactory::createWaypoint(const string& animationFile, const string& objectName, Vector2 position, Vector2 size)
{
    WaypointAnimatedObject* pWaypoint = new WaypointAnimatedObject(
            m_pPlatformManager, 
            m_rDataCacheManager, 
            animationFile, 
            objectName);
    
    // wiring up to the physics engine
    PhysicsBody* pSensor = m_pWorld->createSensor(position, size);
    pSensor->setGameObject(pWaypoint);
    pSensor->setOnCollideListener(pWaypoint);

    pWaypoint->play("idle");

    return pWaypoint;
}

Player* AnimatedObjectsFactory::createPlayer(const Vector2& position, const Vector2& size)
{
    PhysicsBody* pBody = m_pWorld->createDynamicBody(position, size, 1, 0, 0, 1.0f);
    Player* pPlayer = new Player(m_pPlatformManager, pBody, m_rDataCacheManager);
    pBody->setGameObject(pPlayer);
    return pPlayer;
}
