#include "World.hpp"
#include "Body.hpp"

#include "utils/MathUtils.h"

#include <string>

World::World()
{
    initializeMembers();

    b2Vec2 gravity(0.0f, GRAVITY);
    m_pBox2DWorld = new b2World(gravity);
}

World::~World()
{
    delete m_pBox2DWorld;
    
    initializeMembers();
}

void World::update(float delta)
{
    m_pBox2DWorld->Step(BOX2D_TIME_STEP, BOX2D_VELOCITY_ITERATIONS, BOX2D_POSITION_ITERATIONS);
}

const Body* World::createBody(const string& name, const b2BodyDef& bodyDef, const b2FixtureDef& fixtureDef)
{
    Body* pBody = new Body();
    pBody->m_name = name;
    
    pBody->m_pb2Body = m_pBox2DWorld->CreateBody(&bodyDef);
    pBody->m_pb2Body->CreateFixture(&fixtureDef);
    
    m_bodies.push_back(pBody);
    return pBody;
}

void World::receiveEvent(Event* pEvent) {
    for (int i = 0; i < m_bodies.size(); i++)
    {   
        Body* pBody = m_bodies[i];
        if (strcmp(pBody->m_name.c_str(), pEvent->getTarget().c_str()) == 0) {
            if (strcmp(pEvent->getName().c_str(), "right_start") == 0) {
                applySpeed(pBody->m_pb2Body, b2Vec2(PLAYER_RUNNING_SPEED, 0));
            } else if (strcmp(pEvent->getName().c_str(), "right_stop") == 0) {
                applySpeed(pBody->m_pb2Body, b2Vec2(0, 0));
            } else if (strcmp(pEvent->getName().c_str(), "left_start") == 0) {
                applySpeed(pBody->m_pb2Body, b2Vec2(-PLAYER_RUNNING_SPEED, 0));
            } else if (strcmp(pEvent->getName().c_str(), "left_stop") == 0) {
                applySpeed(pBody->m_pb2Body, b2Vec2(0, 0));
            } else if (strcmp(pEvent->getName().c_str(), "space_start") == 0) {
                const b2Vec2& currentVel = pBody->m_pb2Body->GetLinearVelocity();
                if (currentVel.y == 0) 
                {
                    pBody->m_pb2Body->ApplyLinearImpulse( b2Vec2(0, PLAYER_JUMPING_FORCE), pBody->m_pb2Body->GetWorldCenter(), true);
                } 
            }
        }
    }
}

void World::applySpeed(b2Body* pb2Body, const b2Vec2& speed)
{
    //source: https://gamedev.stackexchange.com/questions/47282/moving-player-in-box2d-without-forces
    b2Vec2 vel = pb2Body->GetLinearVelocity();
    b2Vec2 velChange = speed - vel;
    float impulseX = pb2Body->GetMass() * velChange.x;
    pb2Body->ApplyLinearImpulse( b2Vec2(impulseX, 0), pb2Body->GetWorldCenter(), true);
}