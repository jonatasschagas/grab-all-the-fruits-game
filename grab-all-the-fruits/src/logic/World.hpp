#pragma once
#ifndef World_hpp 
#define World_hpp

struct Body;

#include "core/BaseDataStructures.h"
#include "event/EventListener.hpp"
#include <vector>
#include <string>
#include "GameConfiguration.h"
#include <box2d/box2d.h>

using namespace std;


class World : public EventListener
{
public:
    
    World();
    ~World();
    
    void update(float delta);
    void receiveEvent(Event* pEvent) override;
    const Body* createBody(const string& name, const b2BodyDef& bodyDef, const b2FixtureDef& fixtureDef);

private:

    void applySpeed(b2Body* pb2Body, const b2Vec2& speed);

    vector<Body*> m_bodies;
    b2World* m_pBox2DWorld;

    void initializeMembers()
    {
        m_pBox2DWorld = nullptr;
        m_bodies.clear();
    }
    
};

#endif /* World_hpp */
