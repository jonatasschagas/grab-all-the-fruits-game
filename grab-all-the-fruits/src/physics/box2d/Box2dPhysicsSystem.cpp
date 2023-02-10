#include "Box2dPhysicsSystem.hpp"

#include "logic/GameConfiguration.h"
#include <box2d/box2d.h>
#include "Box2dPhysicsBody.hpp"
#include "Box2dDebugRenderer.hpp"
#include "physics/PhysicsOnCollideListener.hpp"

Box2dPhysicsSystem::Box2dPhysicsSystem(const Vector2& rWorldSize, PlatformManager* pPlatformManager) : b2ContactListener(), 
    m_worldSize(rWorldSize)
{
    initializeMembers();
    m_pPlatformManager = pPlatformManager;
    m_pDebugRenderer = new Box2dDebugRenderer(m_worldSize, m_pPlatformManager);
}

Box2dPhysicsSystem::~Box2dPhysicsSystem()
{
    delete m_pBox2DWorld;
    delete m_pDebugRenderer;
    initializeMembers();
}

void Box2dPhysicsSystem::initWorld(const Vector2& rGravity)
{
    b2Vec2 gravity(0.0f, GRAVITY);
    m_pBox2DWorld = new b2World(gravity);
    m_pBox2DWorld->SetContactListener(this);
    m_pBox2DWorld->SetDebugDraw(m_pDebugRenderer);
    m_pDebugRenderer->SetFlags( b2Draw::e_shapeBit);
}

void Box2dPhysicsSystem::update(float delta)
{
    m_pBox2DWorld->Step(BOX2D_TIME_STEP, BOX2D_VELOCITY_ITERATIONS, BOX2D_POSITION_ITERATIONS);
}

void Box2dPhysicsSystem::renderDebug(const Vector2& rOffset)
{
    m_pDebugRenderer->updateOffset(rOffset);
    m_pBox2DWorld->DebugDraw();
}

PhysicsBody* Box2dPhysicsSystem::createDynamicBody(
    const Vector2& rGamePosition, 
    const Vector2& rGameSize,
    float weight,
    float friction,
    float restituition,
    float gravityScale,
    const PhysicsShape& physicsShape)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = toPhysicsWorld(rGamePosition);

    b2FixtureDef fixtureDef;
    if (physicsShape == PhysicsShape_Circle)
    {
        b2CircleShape circleShape = createCircleShape(rGameSize);
        fixtureDef.shape = &circleShape;
    }
    else
    {
        b2PolygonShape squareShape = createBoxShape(rGameSize);
        fixtureDef.shape = &squareShape;
    }
    
    fixtureDef.density = weight;
    fixtureDef.friction = friction;
    fixtureDef.restitution = restituition;

    b2Body* b2Body = m_pBox2DWorld->CreateBody(&bodyDef);
    b2Body->CreateFixture(&fixtureDef);
    b2Body->SetGravityScale(gravityScale);
    b2Body->SetSleepingAllowed(false);
    
    // do not rotate the body freely
    b2Body->SetFixedRotation(true);

    Box2dPhysicsBody* pPhysicsBody = new Box2dPhysicsBody(b2Body, PhysicsBodyType::PhysicsBodyTypeDynamic, m_worldSize);
    return pPhysicsBody;
}

PhysicsBody* Box2dPhysicsSystem::createStaticBody(
    const Vector2& rGamePosition, 
    const Vector2& rGameSize,
    float friction,
    float restituition,
    const PhysicsShape& physicsShape)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position = toPhysicsWorld(rGamePosition);

    b2FixtureDef fixtureDef;
    if (physicsShape == PhysicsShape_Circle)
    {
        b2CircleShape circleShape = createCircleShape(rGameSize);
        fixtureDef.shape = &circleShape;
    }
    else
    {
        b2PolygonShape squareShape = createBoxShape(rGameSize);
        fixtureDef.shape = &squareShape;
    }
    fixtureDef.friction = friction;
    fixtureDef.restitution = restituition; // bounciness
    
    b2Body* b2Body = m_pBox2DWorld->CreateBody(&bodyDef);
    b2Body->CreateFixture(&fixtureDef);
    
    Box2dPhysicsBody* pPhysicsBody = new Box2dPhysicsBody(b2Body, PhysicsBodyType::PhysicsBodyTypeStatic, m_worldSize);
    return pPhysicsBody;
}

PhysicsBody* Box2dPhysicsSystem::createKinematicBody(
    const Vector2& rGamePosition, 
    const Vector2& rGameSize,
    float friction,
    float restituition,
    const PhysicsShape& physicsShape) 
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_kinematicBody;
    bodyDef.position = toPhysicsWorld(rGamePosition);

    b2FixtureDef fixtureDef;
    if (physicsShape == PhysicsShape_Circle)
    {
        b2CircleShape circleShape = createCircleShape(rGameSize);
        fixtureDef.shape = &circleShape;
    }
    else
    {
        b2PolygonShape squareShape = createBoxShape(rGameSize);
        fixtureDef.shape = &squareShape;
    }
    fixtureDef.friction = friction;
    fixtureDef.restitution = restituition; // bounciness
    
    b2Body* b2Body = m_pBox2DWorld->CreateBody(&bodyDef);
    b2Body->CreateFixture(&fixtureDef);
    
    Box2dPhysicsBody* pPhysicsBody = new Box2dPhysicsBody(b2Body, PhysicsBodyType::PhysicsBodyTypeKinematic, m_worldSize);
    return pPhysicsBody;
}

PhysicsBody* Box2dPhysicsSystem::createSensor(const Vector2& rGamePosition, const Vector2& rGameSize)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position = toPhysicsWorld(rGamePosition);

    b2PolygonShape squareShape = createBoxShape(rGameSize);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &squareShape;
    fixtureDef.isSensor = true;
    
    b2Body* b2Body = m_pBox2DWorld->CreateBody(&bodyDef);
    b2Body->CreateFixture(&fixtureDef);
    
    Box2dPhysicsBody* pPhysicsBody = new Box2dPhysicsBody(b2Body, PhysicsBodyType::PhysicsBodyTypeSensor, m_worldSize);
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
    Box2dPhysicsBody* pBodyA = nullptr;
    Box2dPhysicsBody* pBodyB = nullptr;

    b2Fixture* pFixtureA = contact->GetFixtureA();
    b2BodyUserData bodyUserData = pFixtureA->GetBody()->GetUserData();
    if (bodyUserData.pointer)
    {
        pBodyA = reinterpret_cast<Box2dPhysicsBody*>(bodyUserData.pointer);
    }

    b2Fixture* pFixtureB = contact->GetFixtureB();
    bodyUserData = pFixtureB->GetBody()->GetUserData();
    if (bodyUserData.pointer)
    {
        pBodyB = reinterpret_cast<Box2dPhysicsBody*>(bodyUserData.pointer);
    }

    PhysicsOnCollideListener* pOnCollideListener = pBodyA->getOnCollideListener();
    if (pOnCollideListener != nullptr)
    {
        if (pFixtureA->IsSensor() && pFixtureA->GetUserData().pointer)
        {
            string* pName = reinterpret_cast<string*>(pFixtureA->GetUserData().pointer);
            pOnCollideListener->onSensorTriggeredStart(*pName, pBodyB);
        }
        else
        {
            pOnCollideListener->onCollideStart(pBodyB);
        }
    }

    pOnCollideListener = pBodyB->getOnCollideListener();
    if (pOnCollideListener != nullptr)
    {
        if (pFixtureB->IsSensor() && pFixtureB->GetUserData().pointer)
        {
            pOnCollideListener->onSensorTriggeredStart(reinterpret_cast<char*>(pFixtureB->GetUserData().pointer), pBodyA);
        }
        else
        {
            pOnCollideListener->onCollideStart(pBodyA);
        }
    }
}

void Box2dPhysicsSystem::EndContact(b2Contact* contact)
{
    Box2dPhysicsBody* pBodyA = nullptr;
    Box2dPhysicsBody* pBodyB = nullptr;

    b2Fixture* pFixtureA = contact->GetFixtureA();
    b2BodyUserData bodyUserData = pFixtureA->GetBody()->GetUserData();
    if (bodyUserData.pointer)
    {
        pBodyA = reinterpret_cast<Box2dPhysicsBody*>(bodyUserData.pointer);
    }

    b2Fixture* pFixtureB = contact->GetFixtureB();
    bodyUserData = pFixtureB->GetBody()->GetUserData();
    if (bodyUserData.pointer)
    {
        pBodyB = reinterpret_cast<Box2dPhysicsBody*>(bodyUserData.pointer);
    }

    PhysicsOnCollideListener* pOnCollideListener = pBodyA->getOnCollideListener();
    if (pOnCollideListener != nullptr)
    {
        if (pFixtureA->IsSensor() && pFixtureA->GetUserData().pointer)
        {
            string* pName = reinterpret_cast<string*>(pFixtureA->GetUserData().pointer);
            pOnCollideListener->onSensorTriggeredEnd(*pName, pBodyB);
        }
        else
        {
            pOnCollideListener->onCollideEnd(pBodyB);
        }
    }

    pOnCollideListener = pBodyB->getOnCollideListener();
    if (pOnCollideListener != nullptr)
    {
        if (pFixtureB->IsSensor() && pFixtureB->GetUserData().pointer)
        {
            pOnCollideListener->onSensorTriggeredEnd(reinterpret_cast<char*>(pFixtureB->GetUserData().pointer), pBodyA);
        }
        else
        {
            pOnCollideListener->onCollideEnd(pBodyA);
        }
    }
}

b2PolygonShape Box2dPhysicsSystem::createBoxShape(const Vector2& rGameSize) const
{
    b2PolygonShape boxShape;
    boxShape.SetAsBox(
        (rGameSize.x * METERS_PER_PIXEL)/2, 
        (rGameSize.y * METERS_PER_PIXEL)/2
    );
    return boxShape;
}

b2CircleShape Box2dPhysicsSystem::createCircleShape(const Vector2& rGameSize) const
{
    b2CircleShape circleShape;
    circleShape.m_radius = (rGameSize.x * METERS_PER_PIXEL)/2;
    return circleShape;
}
