#pragma once
#ifndef PhysicsOnCollideListener_hpp 
#define PhysicsOnCollideListener_hpp

#include <string>

using namespace std;

class PhysicsBody;

class PhysicsOnCollideListener 
{
public:

    virtual void onCollide(PhysicsBody* pOtherBody) = 0;

    virtual void onSensorTriggeredStart(const string& name) = 0;
    virtual void onSensorTriggeredEnd(const string& name) = 0;

};

#endif /* PhysicsOnCollideListener_hpp */
