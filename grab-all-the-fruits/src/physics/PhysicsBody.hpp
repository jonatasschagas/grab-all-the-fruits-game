#pragma once
#ifndef PhysicsBody_hpp 
#define PhysicsBody_hpp

#include "core/Vector2.h"
#include "PhysicsOnCollideListener.hpp"
#include "objects/GameObject.hpp"
#include <string>

using namespace std;

enum PhysicsBodyType
{
    PhysicsBodyTypeStatic,
    PhysicsBodyTypeDynamic,
    PhysicsBodyTypeKinematic,
    PhysicsBodyTypeSensor
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

    virtual void updatePhysicsPosition(const Vector2& rGamePosition) = 0;
    virtual void setGameObject(GameObject* pGameObject) = 0;
    virtual GameObject* getGameObject() const = 0;

    virtual void applyForce(const Vector2& rForce, const PhysicsForceType& forceType) = 0;
    virtual void applyInstantForce(const Vector2& rForce) = 0;

    virtual void setOnCollideListener(PhysicsOnCollideListener* pOnCollideListener) = 0;

    virtual void addSensor(const string& name, const Vector2& position, const Vector2& size) = 0;

    virtual void destroy() = 0;

};

#endif /* PhysicsBody_hpp */
