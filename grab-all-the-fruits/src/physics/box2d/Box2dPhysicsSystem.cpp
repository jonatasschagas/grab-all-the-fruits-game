#include "Box2dPhysicsSystem.hpp"

#include "logic/GameConfiguration.h"
#include <box2d/box2d.h>
#include "Box2dPhysicsBody.hpp"
#include "physics/PhysicsOnCollideListener.hpp"

Box2dPhysicsSystem::Box2dPhysicsSystem(const Vector2& rWorldSize) : b2ContactListener(), 
    m_worldSize(rWorldSize)
{
    initializeMembers();
}

Box2dPhysicsSystem::~Box2dPhysicsSystem()
{
    delete m_pBox2DWorld;
    initializeMembers();
}

void Box2dPhysicsSystem::initWorld(const Vector2& rGravity)
{
    b2Vec2 gravity(0.0f, GRAVITY);
    m_pBox2DWorld = new b2World(gravity);
    m_pBox2DWorld->SetContactListener(this);
}

void Box2dPhysicsSystem::update(float delta)
{
    m_pBox2DWorld->Step(BOX2D_TIME_STEP, BOX2D_VELOCITY_ITERATIONS, BOX2D_POSITION_ITERATIONS);
}

PhysicsBody* Box2dPhysicsSystem::createDynamicBody(
    const Vector2& rGamePosition, 
    const Vector2& rGameSize,
    float weight,
    float friction,
    float restituition,
    float gravityScale)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = toPhysicsWorld(rGamePosition);

    b2CircleShape circleShape;
    circleShape.m_radius = (rGameSize.x * METERS_PER_PIXEL)/2.0f;

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circleShape;
    fixtureDef.density = weight;
    fixtureDef.friction = friction;
    fixtureDef.restitution = restituition;
    
    b2Body* b2Body = m_pBox2DWorld->CreateBody(&bodyDef);
    b2Body->CreateFixture(&fixtureDef);
    b2Body->SetGravityScale(gravityScale);
    
    // do not rotate the body freely
    b2Body->SetFixedRotation(true);

    Box2dPhysicsBody* pPhysicsBody = new Box2dPhysicsBody(b2Body, PhysicsBodyType::PhysicsBodyTypeDynamic, m_worldSize);
    return pPhysicsBody;
}

PhysicsBody* Box2dPhysicsSystem::createStaticBody(
    const Vector2& rGamePosition, 
    const Vector2& rGameSize,
    float friction,
    float restituition)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position = toPhysicsWorld(rGamePosition);

    b2PolygonShape squareShape;
    squareShape.SetAsBox(
        (rGameSize.x * METERS_PER_PIXEL)/2, 
        (rGameSize.y * METERS_PER_PIXEL)/2
    );

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &squareShape;
    fixtureDef.restitution = restituition; // how bouncy it is
    fixtureDef.friction = friction; // how slippery it is
    
    b2Body* b2Body = m_pBox2DWorld->CreateBody(&bodyDef);
    b2Body->CreateFixture(&fixtureDef);
    
    Box2dPhysicsBody* pPhysicsBody = new Box2dPhysicsBody(b2Body, PhysicsBodyType::PhysicsBodyTypeStatic, m_worldSize);
    return pPhysicsBody;
}

b2Vec2 Box2dPhysicsSystem::toPhysicsWorld(const Vector2& rGamePosition) const
{
    const float xPhysicsWorldInMeters = rGamePosition.x * METERS_PER_PIXEL;
    // y is inverted in the physics world
    const float yPhysicsWorldInMeters = (m_worldSize.y - rGamePosition.y) * METERS_PER_PIXEL;
    return b2Vec2(xPhysicsWorldInMeters, yPhysicsWorldInMeters);
}

void Box2dPhysicsSystem::BeginContact(b2Contact* contact)
{
}

void Box2dPhysicsSystem::EndContact(b2Contact* contact)
{
    Box2dPhysicsBody* pBodyA = nullptr;
    Box2dPhysicsBody* pBodyB = nullptr;

    b2BodyUserData bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
    if (bodyUserData.pointer)
    {
        pBodyA = reinterpret_cast<Box2dPhysicsBody*>(bodyUserData.pointer);
    }
        
    bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
    if (bodyUserData.pointer)
    {
        pBodyB = reinterpret_cast<Box2dPhysicsBody*>(bodyUserData.pointer);
    }

    PhysicsOnCollideListener* pOnCollideListener = pBodyA->getOnCollideListener();
    if (pOnCollideListener != nullptr)
    {
        pOnCollideListener->onCollide(pBodyB);
    }

    pOnCollideListener = pBodyB->getOnCollideListener();
    if (pOnCollideListener != nullptr)
    {
        pOnCollideListener->onCollide(pBodyA);
    }
}
