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
    void setVelocity(const Vector2& rVelocity) override;
    void setGameObject(GameObject* pGameObject) override;
    
    GameObject* getGameObject() const override;

    void updatePhysicsPosition(const Vector2& rGamePosition) override;
    
    void applyForce(const Vector2& rForce, const PhysicsForceType& forceType) override;
    void applyInstantForce(const Vector2& rForce) override;

    void setOnCollideListener(PhysicsOnCollideListener* pOnCollideListener) override;
    PhysicsOnCollideListener* getOnCollideListener() const;

    void addSensor(const string& name, const Vector2& position, const Vector2& size) override;

    void lockHorizontalMovement() override;

    void destroy() override;

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
