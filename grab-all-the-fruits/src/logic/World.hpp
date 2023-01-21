#pragma once
#ifndef World_hpp 
#define World_hpp

struct Body;

#include "core/BaseDataStructures.h"
#include "event/EventListener.hpp"
#include "Map.hpp"
#include <vector>
#include <string>

using namespace std;


//TODO: move to config
const float PLAYER_RUNNING_SPEED = 2.5f;
const float PLAYER_JUMPING_SPEED = 12.f;

class World : public EventListener
{
public:
    
    World(const Map& map);
    ~World();
    
    void update(float delta);
    void receiveEvent(Event* pEvent) override;
    const Body* createBody(const string& name, const Vector2& position, const GameSize& size);

private:

    float calculateVXStep(float vx, float vxGloal, float deltaTime);
    float calculateXStep(float x, float vx, float deltaTime);

    const Map& m_map;
    float m_gravity;
    vector<Body*> m_bodies;

    void initializeMembers()
    {
        m_gravity = -5.4f;
        m_bodies.clear();
    }
    
};

#endif /* World_hpp */
