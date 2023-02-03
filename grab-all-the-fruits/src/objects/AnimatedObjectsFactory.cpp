#include "AnimatedObjectsFactory.hpp"

#include "WaypointAnimatedObject.hpp"
#include "FruitAnimatedObject.hpp"
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
            pAnimatedObject = createWaypoint(animationFile, objectName, objectType, position, size);
        }
    }
    else if (objectType.compare("collectable") == 0)
    {
        pAnimatedObject = createCollectable(animationFile, objectName, objectType, position, size);
    }  
    else 
    {
        pAnimatedObject = new AnimatedObject(m_pPlatformManager, m_rDataCacheManager, animationFile, objectType);
    }
    
    return pAnimatedObject;
}

AnimatedObject* AnimatedObjectsFactory::createWaypoint(const string& animationFile, const string& objectName, const string& objectType, Vector2 position, Vector2 size)
{
    WaypointAnimatedObject* pWaypoint = new WaypointAnimatedObject(
            m_pPlatformManager, 
            m_rDataCacheManager, 
            animationFile, 
            objectName,
            objectType);
    
    // wiring up to the physics engine
    PhysicsBody* pSensor = m_pWorld->createSensor(position, size);
    pSensor->setGameObject(pWaypoint);
    pSensor->setOnCollideListener(pWaypoint);

    pWaypoint->play("idle");

    return pWaypoint;
}

AnimatedObject* AnimatedObjectsFactory::createCollectable(const string& animationFile, const string& objectName, const string& objectType, Vector2 position, Vector2 size)
{
    FruitAnimatedObject* pFruit = new FruitAnimatedObject(
            m_pPlatformManager, 
            m_rDataCacheManager, 
            animationFile, 
            objectName,
            objectType,
            m_pEventListener);
    
    // wiring up to the physics engine
    PhysicsBody* pSensor = m_pWorld->createSensor(position, size);
    pSensor->setGameObject(pFruit);
    pSensor->setOnCollideListener(pFruit);

    pFruit->play("idle");

    return pFruit;
}

Player* AnimatedObjectsFactory::createPlayer(const Vector2& position, const Vector2& size)
{
    PhysicsBody* pBody = m_pWorld->createDynamicBody(position, size, 1, 0, 0, 1.0f);
    Player* pPlayer = new Player(m_pPlatformManager, pBody, m_rDataCacheManager);
    pBody->setGameObject(pPlayer);
    return pPlayer;
}

AnimatedObject* AnimatedObjectsFactory::createGenericObject(const string& animatedObject, const Vector2& position, const Vector2& size)
{
    string animationFile = m_animatedObjectsPath + "/" + animatedObject + "_animation.json";
    AnimatedObject* pAnimatedObject = new AnimatedObject(m_pPlatformManager, m_rDataCacheManager, animationFile, animatedObject);
    pAnimatedObject->setXY(position.x, position.y);
    pAnimatedObject->setSize(size.x, size.y);
    return pAnimatedObject;
}