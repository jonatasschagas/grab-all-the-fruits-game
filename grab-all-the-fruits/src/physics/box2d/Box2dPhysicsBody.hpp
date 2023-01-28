#pragma once
#ifndef Box2dPhysicsBody_hpp 
#define Box2dPhysicsBody_hpp

#include "physics/PhysicsBody.hpp"
#include <box2d/b2_body.h>

class Box2dPhysicsBody : public PhysicsBody
{
public:

    Box2dPhysicsBody(b2Body* pBox2DBody, const PhysicsBodyType& bodyType, const Vector2& rWorldSize);
    ~Box2dPhysicsBody();

    const PhysicsBodyType& getPhysicsBodyType() const override;
    const Vector2 getGameSize() const override;
    const Vector2 getGamePosition() const override;
    const Vector2 getPhysicsSize() const override;
    const Vector2 getPhysicsPosition() const override;
    const Vector2 getVelocity() const override;
    void setGameObject(GameObject* pGameObject) override;
    GameObject* getGameObject() const override;

    void setGamePosition(const Vector2& rPosition) override;

    void applyForce(const Vector2& rForce, const PhysicsForceType& forceType) override;
    void applyInstantForce(const Vector2& rForce) override;

    void setOnCollideListener(PhysicsOnCollideListener* pOnCollideListener) override;
    PhysicsOnCollideListener* getOnCollideListener() const;

private:

    b2Body* m_pBox2DBody;
    PhysicsOnCollideListener* m_pOnCollideListener;
    PhysicsBodyType m_bodyType;
    Vector2 m_worldSize;
    GameObject* m_pGameObject;

    void initializeMembers()
    {
        m_pBox2DBody = nullptr;
        m_pOnCollideListener = nullptr;
        m_pGameObject = nullptr;
    }    

};

#endif /* Box2dPhysicsBody_hpp */
