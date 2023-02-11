#pragma once
#ifndef GroundObject_hpp 
#define GroundObject_hpp

#include "GameObject.hpp"
#include "physics/PhysicsOnCollideListener.hpp"
#include "event/EventListener.hpp"
#include <string>

using namespace std;

class GroundObject : public GameObject, public PhysicsOnCollideListener
{
public:
    
    GroundObject(EventListener* pEventListener);
    ~GroundObject();
    
    const string& getType() const override { return m_type; }
    const string& getName() const override { return m_name; }

    void onCollideStart(PhysicsBody* pPhysicsBody) override;

private:

    string m_type = "ground";
    string m_name = "ground";
    EventListener* m_pEventListener;

    void initializeMembers()
    {
        m_pEventListener = nullptr;
    }
};

#endif /* GroundObject_hpp */
