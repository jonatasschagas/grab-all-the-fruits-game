#pragma once
#ifndef Map_hpp 
#define Map_hpp

#include "core/TileMapSprite.hpp"
#include "core/Vector2.h"
#include "event/EventListener.hpp"
#include "logic/World.hpp"

class Map
{
public:
    
    Map(World* pWorld, TileMapSprite* pTileMapSprite, EventListener* pEventListener);
    ~Map();

    void updateCameraPosition(const Vector2& rCameraPosition);

    void shakeCamera(float intensity);

    void updateCameraShake(float deltaTime);

private:

    void createMapInPhysicsWorld();
    
    const bool isTileGround(const int& tileX, const int& tileY) const;
    
    TileMapSprite* m_pTileMapSprite;
    EventListener* m_pEventListener;
    World* m_pWorld;

    // camera
    int m_cameraStartTileX;
    int m_cameraStartTileY;
    float m_cameraCenterX;
    float m_cameraCenterY;
    
    bool m_cameraShaking;
    float m_cameraShakeIntensity;
    float m_cameraShakeAngle;
    float m_cameraShakeOffsetX;
    float m_cameraShakeOffsetY;
    float m_cameraShakeAccumulator;
    
    void initializeMembers()
    {
        m_pTileMapSprite = nullptr;
        m_pEventListener = nullptr;
        m_pWorld = nullptr;

        m_cameraStartTileX = 0;
        m_cameraStartTileY = 0;
        m_cameraCenterX = 0;
        m_cameraCenterY = 0;
        
        m_cameraShaking = false;
        m_cameraShakeIntensity = 0;
        m_cameraShakeAngle = 0;
        m_cameraShakeOffsetX = 0;
        m_cameraShakeOffsetY = 0;
        m_cameraShakeAccumulator = 0;
    }
    
};

#endif /* Map_hpp */
