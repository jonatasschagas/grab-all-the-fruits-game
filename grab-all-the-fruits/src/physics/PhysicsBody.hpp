#pragma once
#ifndef PhysicsBody_hpp 
#define PhysicsBody_hpp

#include "core/BaseDataStructures.h"
#include "PhysicsOnCollideListener.hpp"
#include "objects/GameObject.hpp"

enum PhysicsBodyType
{
    PhysicsBodyTypeStatic,
    PhysicsBodyTypeDynamic,
    PhysicsBodyTypeKinematic
};

enum PhysicsForceType
{
    PhysicsForceTypeImpulse,
    PhysicsForceTypeForce
};

class PhysicsBody
{
public:

    virtual ~PhysicsBody() {}

    virtual const PhysicsBodyType& getPhysicsBodyType() const = 0;
    virtual const Vector2 getGameSize() const = 0;
    virtual const Vector2 getGamePosition() const = 0;
    virtual const Vector2 getPhysicsSize() const = 0;
    virtual const Vector2 getPhysicsPosition() const = 0;
    virtual const Vector2 getVelocity() const = 0;

    virtual void setGamePosition(const Vector2& rPosition) = 0;
    virtual void setGameObject(GameObject* pGameObject) = 0;
    virtual GameObject* getGameObject() const = 0;

    virtual void applyForce(const Vector2& rForce, const PhysicsForceType& forceType) = 0;
    virtual void applyInstantForce(const Vector2& rForce) = 0;

    virtual void setOnCollideListener(PhysicsOnCollideListener* pOnCollideListener) = 0;

};

#endif /* PhysicsBody_hpp */
