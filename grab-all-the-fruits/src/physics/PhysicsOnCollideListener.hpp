#pragma once
#ifndef PhysicsOnCollideListener_hpp 
#define PhysicsOnCollideListener_hpp

class PhysicsBody;

class PhysicsOnCollideListener 
{
public:

    virtual void onCollide(PhysicsBody* pOtherBody) = 0;

};

#endif /* PhysicsOnCollideListener_hpp */
