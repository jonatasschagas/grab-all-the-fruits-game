#pragma once
#ifndef PhysicsSystem_hpp 
#define PhysicsSystem_hpp

#include "core/Vector2.h"
#include "PhysicsBody.hpp"

class PhysicsSystem 
{
public:

    virtual ~PhysicsSystem() {}

    virtual void initWorld(const Vector2& rGravity) = 0;
    virtual void update(float delta) = 0;
    virtual void renderDebug(const Vector2& rOffset) = 0;
    
    virtual PhysicsBody* createDynamicBody(
        const Vector2& rGamePosition, 
        const Vector2& rGameSize,
        float weight,
        float friction,
        float restituition,
        float gravityScale) = 0;

    virtual PhysicsBody* createStaticBody(
        const Vector2& rGamePosition, 
        const Vector2& rGameSize,
        float friction,
        float restituition) = 0;
    
    virtual PhysicsBody* createKinematicBody(
        const Vector2& rGamePosition, 
        const Vector2& rGameSize,
        float friction,
        float restituition) = 0;

    virtual PhysicsBody* createSensor(
        const Vector2& rGamePosition, 
        const Vector2& rGameSize) = 0;
};

#endif /* PhysicsSystem_hpp */
