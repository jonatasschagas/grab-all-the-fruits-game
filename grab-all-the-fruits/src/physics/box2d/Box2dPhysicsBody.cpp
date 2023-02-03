#include "Box2dPhysicsBody.hpp"

#include <box2d/box2d.h>
#include "logic/GameConfiguration.h"

Box2dPhysicsBody::Box2dPhysicsBody(b2Body* pBox2DBody, const PhysicsBodyType& bodyType, const Vector2& rWorldSize) : m_bodyType(bodyType), m_worldSize(rWorldSize)
{
    initializeMembers();
    m_pBox2DBody = pBox2DBody;
    m_pBox2DBody->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);
    m_bodyType = bodyType;
}

Box2dPhysicsBody::~Box2dPhysicsBody()
{
    initializeMembers();
}

const PhysicsBodyType& Box2dPhysicsBody::getPhysicsBodyType() const
{
    return m_bodyType;
}

const Vector2 Box2dPhysicsBody::getGameSize() const
{
    const b2Shape* pShape = m_pBox2DBody->GetFixtureList()->GetShape();
    if (PhysicsBodyType::PhysicsBodyTypeDynamic == m_bodyType) 
    {
        return Vector2(
            pShape->m_radius * PIXELS_PER_METER * 2.0f, 
            pShape->m_radius * PIXELS_PER_METER * 2.0f
        );
    }
    else
    {
        b2AABB shapeAABB;
        b2Transform t;
        pShape->ComputeAABB(&shapeAABB, t, 0);
        float bodyWidth = shapeAABB.upperBound.x - shapeAABB.lowerBound.x;
        float bodyHeight = shapeAABB.upperBound.y - shapeAABB.lowerBound.y;

        return Vector2(bodyWidth * PIXELS_PER_METER, bodyHeight * PIXELS_PER_METER); 
    }
}

const Vector2 Box2dPhysicsBody::getGamePosition() const
{
    return Vector2(
        m_pBox2DBody->GetPosition().x * PIXELS_PER_METER,
        m_worldSize.y - m_pBox2DBody->GetPosition().y * PIXELS_PER_METER
    );
}
const Vector2 Box2dPhysicsBody::getPhysicsSize() const
{
    const b2Shape* pShape = m_pBox2DBody->GetFixtureList()->GetShape();
    if (PhysicsBodyType::PhysicsBodyTypeDynamic == m_bodyType) 
    {
        return Vector2(
            pShape->m_radius * 2.0f, 
            pShape->m_radius * 2.0f
        );
    }
    else
    {
         b2AABB shapeAABB;
        b2Transform t;
        pShape->ComputeAABB(&shapeAABB, t, 0);
        float bodyWidth = shapeAABB.upperBound.x - shapeAABB.lowerBound.x;
        float bodyHeight = shapeAABB.upperBound.y - shapeAABB.lowerBound.y;

        return Vector2(bodyWidth, bodyHeight); 
    }
}

const Vector2 Box2dPhysicsBody::getPhysicsPosition() const
{
    return Vector2(
        m_pBox2DBody->GetPosition().x,
        m_pBox2DBody->GetPosition().y
    );
}

const Vector2 Box2dPhysicsBody::getVelocity() const
{
    return Vector2(
        m_pBox2DBody->GetLinearVelocity().x,
        m_pBox2DBody->GetLinearVelocity().y
    );
}

void Box2dPhysicsBody::updatePhysicsPosition(const Vector2& rGamePosition)
{
    m_pBox2DBody->SetTransform(
        b2Vec2(rGamePosition.x * METERS_PER_PIXEL, (m_worldSize.y - rGamePosition.y) * METERS_PER_PIXEL),
        m_pBox2DBody->GetAngle()
    );
}

void Box2dPhysicsBody::applyForce(const Vector2& rForce, const PhysicsForceType& forceType)
{
    if (PhysicsForceType::PhysicsForceTypeImpulse == forceType)
    {
        m_pBox2DBody->ApplyLinearImpulse(
            b2Vec2(rForce.x, rForce.y),
            m_pBox2DBody->GetWorldCenter(),
            true
        );
    }
    else if (PhysicsForceType::PhysicsForceTypeForce == forceType)
    {
        m_pBox2DBody->ApplyForce(
            b2Vec2(rForce.x, rForce.y),
            m_pBox2DBody->GetWorldCenter(),
            true
        );
    }
}

void Box2dPhysicsBody::applyInstantForce(const Vector2& rForce)
{
    //source: https://gamedev.stackexchange.com/questions/47282/moving-player-in-box2d-without-forces
    b2Vec2 vel = m_pBox2DBody->GetLinearVelocity();
    b2Vec2 velChange = b2Vec2(rForce.x, rForce.y) - vel;
    float impulseX = m_pBox2DBody->GetMass() * velChange.x;
    m_pBox2DBody->ApplyLinearImpulse( b2Vec2(impulseX, 0), m_pBox2DBody->GetWorldCenter(), true);
}

void Box2dPhysicsBody::setOnCollideListener(PhysicsOnCollideListener* pOnCollideListener)
{
    m_pOnCollideListener = pOnCollideListener;
}

PhysicsOnCollideListener* Box2dPhysicsBody::getOnCollideListener() const
{
    return m_pOnCollideListener;
}

void Box2dPhysicsBody::setGameObject(GameObject* pGameObject)
{
    m_pGameObject = pGameObject;
}

GameObject* Box2dPhysicsBody::getGameObject() const
{
    return m_pGameObject;
}

void Box2dPhysicsBody::destroy()
{
    if (m_pBox2DBody != nullptr)
    {
        b2World* pBox2DWorld = m_pBox2DBody->GetWorld();
        pBox2DWorld->DestroyBody(m_pBox2DBody);
    }
}
