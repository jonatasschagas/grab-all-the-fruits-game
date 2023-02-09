#include "AnimatedObjectsFactory.hpp"

#include "WaypointAnimatedObject.hpp"
#include "FruitAnimatedObject.hpp"
#include "obstacles/StationaryObstacle.hpp"
#include "obstacles/SawObstacle.hpp"
#include "platforms/MovingPlatform.hpp"
#include "level/LevelManager.hpp"
#include "DisappearingAnimation.hpp"
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

Sprite* AnimatedObjectsFactory::createMetaTile(const int tileX, const int tileY, const TileConfig* pTileConfig, const Vector2& position, const Vector2& size)
{
    const string& objectName = pTileConfig->getProperty("name");
    const string& objectType = pTileConfig->getProperty("type");

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
            pAnimatedObject = createWaypoint(objectName, objectType, position, size);
        }
    }
    else if (objectType.compare("collectable") == 0)
    {
        pAnimatedObject = createCollectable(objectName, objectType, position, size);
    }  
    else if (objectType.compare("obstacle") == 0)
    {
        pAnimatedObject = createObstacle(tileX, tileY, objectName, objectType, position, size, pTileConfig);
    }  
    else if (objectType.compare("platform") == 0)
    {
        pAnimatedObject = createPlatform(tileX, tileY, objectName, objectType, position, size);
    }  
    
    return pAnimatedObject;
}

AnimatedObject* AnimatedObjectsFactory::createWaypoint(const string& objectName, const string& objectType, Vector2 position, Vector2 size)
{
    string animationFile = m_animatedObjectsPath + "/waypoints/" + objectName + "_animation.json";

    WaypointAnimatedObject* pWaypoint = new WaypointAnimatedObject(
            m_pPlatformManager, 
            m_rDataCacheManager, 
            m_pEventListener,
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

AnimatedObject* AnimatedObjectsFactory::createCollectable(const string& objectName, const string& objectType, Vector2 position, Vector2 size)
{
    string animationFile = m_animatedObjectsPath + "/collectables/" + objectName + "_animation.json";

    // wiring up to the physics engine
    PhysicsBody* pSensor = m_pWorld->createSensor(position, size);
    
    FruitAnimatedObject* pFruit = new FruitAnimatedObject(
            m_pPlatformManager, 
            m_rDataCacheManager, 
            pSensor,
            animationFile, 
            objectName,
            objectType,
            m_pEventListener);
    
    pSensor->setGameObject(pFruit);
    pSensor->setOnCollideListener(pFruit);

    pFruit->play("idle");

    return pFruit;
}

Player* AnimatedObjectsFactory::createPlayer(const Vector2& position, const Vector2& size)
{
    PhysicsBody* pBody = m_pWorld->createDynamicBody(position, size, 1, 1, 0, 1.0f);
    Player* pPlayer = new Player(m_pPlatformManager, pBody, m_rDataCacheManager, m_pEventListener);
    pBody->setGameObject(pPlayer);
    pBody->setOnCollideListener(pPlayer);
    return pPlayer;
}

AnimatedObject* AnimatedObjectsFactory::createDisappearingAnimation(const Vector2& position, const Vector2& size)
{
    string animationFile = m_animatedObjectsPath + "/animations/collected_animation.json";
    AnimatedObject* pAnimatedObject = new DisappearingAnimation(m_pPlatformManager, m_rDataCacheManager, animationFile);
    pAnimatedObject->setXY(position.x, position.y);
    pAnimatedObject->setSize(size.x, size.y);
    return pAnimatedObject;
}

AnimatedObject* AnimatedObjectsFactory::createObstacle(const int tileX, const int tileY, const string& objectName, const string& objectType, Vector2 position, Vector2 size, const TileConfig* pTileConfig)
{
    if (objectName.compare("spikes") == 0 || objectName.compare("fire") == 0 || objectName.compare("fan") == 0)
    {
        string animationFile = m_animatedObjectsPath + "/obstacles/" + objectName + "_animation.json";

        Vector2 tileSize = m_pLevelManager->getTileSize();
        Vector2 finalSize(
            stoi(pTileConfig->getProperty("tileWidth")) * tileSize.x, 
            stoi(pTileConfig->getProperty("tileHeight")) * tileSize.y
        );
        
        // wiring up to the physics engine
        PhysicsBody* pSensor = m_pWorld->createSensor(position, finalSize);

        StationaryObstacle* pObstacle = new StationaryObstacle(
            m_pPlatformManager, 
            m_rDataCacheManager, 
            pSensor,
            animationFile, 
            objectName,
            objectType,
            m_pEventListener
        );

        pSensor->setGameObject(pObstacle);
        pSensor->setOnCollideListener(pObstacle);

        pObstacle->setXY(position.x, position.y);
        pObstacle->setSize(finalSize.x, finalSize.y);
        
        pObstacle->play("idle");

        return pObstacle;
    }
    else if (objectName.compare("saw") == 0)
    {
        string animationFile = m_animatedObjectsPath + "/obstacles/" + objectName + "_animation.json";

        const Trap* pTrap = m_pLevelManager->findTrap(tileX, tileY);

        Vector2 tileSize = m_pLevelManager->getTileSize();
        Vector2 initialPosition = position;
        Vector2 finalPosition = Vector2(pTrap->targetTileX * tileSize.x, pTrap->targetTileY * tileSize.y);
        Vector2 finalSize(pTrap->widthInTiles * tileSize.x, pTrap->heightInTiles * tileSize.y);

        // wiring up to the physics engine
        PhysicsBody* pKinematic = m_pWorld->createKinematicBody(position, finalSize, 0, 0);
        
        SawObstacle* pSaw = new SawObstacle(
            m_pPlatformManager, 
            m_rDataCacheManager, 
            pKinematic,
            animationFile, 
            objectName,
            objectType,
            m_pEventListener,
            initialPosition,
            finalPosition
        );

        pKinematic->setGameObject(pSaw);
        pKinematic->setOnCollideListener(pSaw);

        pSaw->setXY(position.x - tileSize.x, position.y - tileSize.y);
        pSaw->setPivotAtCenter(true);
        pSaw->setSize(finalSize);
        
        pSaw->play("idle");

        return pSaw;
    }
    else
    {
        return nullptr;
    }
}

AnimatedObject* AnimatedObjectsFactory::createPlatform(const int tileX, const int tileY, const string& objectName, const string& objectType, Vector2 position, Vector2 size)
{
    if (objectName.compare("moving-platform") == 0)
    {
        string animationFile = m_animatedObjectsPath + "/platforms/" + objectName + "_animation.json";

        const Platform* pPlatform = m_pLevelManager->findPlatform(tileX, tileY);

        Vector2 tileSize = m_pLevelManager->getTileSize();
        Vector2 initialPosition = position;
        Vector2 finalPosition = Vector2(pPlatform->targetTileX * tileSize.x, pPlatform->targetTileY * tileSize.y);
        Vector2 finalSize(pPlatform->widthInTiles * tileSize.x, pPlatform->heightInTiles * tileSize.y);

        // wiring up to the physics engine
        // needs friction to be 1 to avoid sliding
        PhysicsBody* pKinematic = m_pWorld->createKinematicBody(position, finalSize, 10, 0);
        
        MovingPlatform* pMovingPlatform = new MovingPlatform(
            m_pPlatformManager, 
            m_rDataCacheManager, 
            pKinematic,
            animationFile, 
            objectName,
            objectType,
            m_pEventListener,
            initialPosition,
            finalPosition
        );

        pKinematic->setGameObject(pMovingPlatform);
        pKinematic->setOnCollideListener(pMovingPlatform);

        pMovingPlatform->setXY(position.x, position.y);
        pMovingPlatform->setSize(finalSize);
        
        pMovingPlatform->play("idle");

        return pMovingPlatform;
    }
    else
    {
        return nullptr;
    }
}

void AnimatedObjectsFactory::setLevelManager(LevelManager* pLevelManager)
{
    m_pLevelManager = pLevelManager;
}