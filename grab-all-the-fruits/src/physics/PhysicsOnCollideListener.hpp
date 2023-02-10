#pragma once
#ifndef PhysicsOnCollideListener_hpp 
#define PhysicsOnCollideListener_hpp

#include <string>

using namespace std;

class PhysicsBody;

class PhysicsOnCollideListener 
{
public:

    virtual void onCollideStart(PhysicsBody* pOtherBody) {}
    virtual void onCollideEnd(PhysicsBody* pOtherBody) {}

    virtual void onSensorTriggeredStart(const string& name, PhysicsBody* pOtherBody) {}
    virtual void onSensorTriggeredEnd(const string& name, PhysicsBody* pOtherBody) {}

};

#endif /* PhysicsOnCollideListener_hpp */
