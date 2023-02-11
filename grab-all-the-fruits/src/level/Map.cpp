#include "Map.hpp"

#include <math.h>
#include "utils/MathUtils.h"
#include "utils/StringUtils.h"
#include "objects/GroundObject.hpp"
#include "utils/PerlinNoise.h"

 Map::Map(World* pWorld, TileMapSprite* pTileMapSprite, EventListener* pEventListener)
 {
    initializeMembers();
    m_pTileMapSprite = pTileMapSprite;
    m_pWorld = pWorld;
    m_pEventListener = pEventListener;

    createMapInPhysicsWorld();
 }

Map::~Map()
{
    initializeMembers();
}

void Map::createMapInPhysicsWorld()
{
    for (int tileX = 0; tileX <= m_pTileMapSprite->getTileMapWidth(); tileX++)
    {
        for (int tileY = 0; tileY <= m_pTileMapSprite->getTileMapHeight(); tileY++)
        {
            if (isTileGround(tileX, tileY))
            {
                PhysicsBody* pBody = m_pWorld->createStaticBody(
                    Vector2(tileX * m_pTileMapSprite->getTileSizeInGameUnits().x, tileY * m_pTileMapSprite->getTileSizeInGameUnits().y),
                    Vector2(m_pTileMapSprite->getTileSizeInGameUnits().x, m_pTileMapSprite->getTileSizeInGameUnits().y),
                    0.f, // friction
                    0.f // restitution
                );
                GroundObject* pGround = new GroundObject(m_pEventListener);
                pBody->setGameObject(pGround);
                pBody->setOnCollideListener(pGround);
            }
        }
    }
}

const bool Map::isTileGround(const int& tileX, const int& tileY) const
{
    int tileData = m_pTileMapSprite->getTileData(tileX, tileY, "ground");
    return tileData != 0;
}
 
 void Map::updateCameraPosition(const Vector2& rCameraPosition)
 {

    const int screenSizeX = m_pTileMapSprite->getScreenWidthInGameUnits();
    const int screenSizeY = m_pTileMapSprite->getScreenHeightInGameUnits();
    const int worldUnitsX = m_pTileMapSprite->getMapSizeInGameUnits().x;
    const int worldUnitsY = m_pTileMapSprite->getMapSizeInGameUnits().y;
    const int halfScreenHorizontal = screenSizeX/2;
    const int halfScreenVertical = screenSizeY/2;
    float levelMaxX = worldUnitsX - halfScreenHorizontal;
    
    float xOffSet = 0;
    float yOffSet = 0;
    if (rCameraPosition.x < halfScreenHorizontal)
    {
        xOffSet = 0;
    }
    else if (rCameraPosition.x > levelMaxX)
    {
        xOffSet = levelMaxX - halfScreenHorizontal;
    }
    else
    {
        xOffSet = rCameraPosition.x - halfScreenHorizontal;
    }
    
    if (rCameraPosition.y < halfScreenVertical)
    {
        yOffSet = 0;
    }
    else
    {
        // capping the vertical offset to 100.f, so that the camera doesnt go underground.
        yOffSet = min(rCameraPosition.y - halfScreenVertical - m_pTileMapSprite->getTileSizeInGameUnits().y, 100.f);
    }
    
    float targetX = xOffSet + m_cameraShakeOffsetX;
    float targetY = yOffSet + m_cameraShakeOffsetY;

    m_pTileMapSprite->setXOffSet(targetX);
    m_pTileMapSprite->setYOffSet(targetY);
 }

void Map::updateCameraShake(float deltaTime)
{
    if (!m_cameraShaking)
        return;
    
    float maxAngle = .1f;
    float maxOffset = 0.5f;
    float seed = getRandomFloat(1, 10);
    
    m_cameraShakeAngle = maxAngle * m_cameraShakeIntensity * perlin::noise(seed, deltaTime);
    m_cameraShakeOffsetX = maxOffset * m_cameraShakeIntensity * perlin::noise(seed + 1, deltaTime);
    m_cameraShakeOffsetY = maxOffset * m_cameraShakeIntensity * perlin::noise(seed + 2, deltaTime);
    m_cameraShakeAccumulator += deltaTime;
    
    //TODO: parameterize this
    if (m_cameraShakeAccumulator > 1.5f)
    {
        m_cameraShaking = false;
        m_cameraShakeAngle = 0;
        m_cameraShakeOffsetX = 0;
        m_cameraShakeOffsetY = 0;
        m_cameraShakeAccumulator = 0;
    }
}

void Map::shakeCamera(float intensity)
{
    if (intensity < 0)
    {
        intensity = getRandomFloat(5, 10);
    }

    m_cameraShaking = true;
    m_cameraShakeIntensity = intensity;
}