#pragma once
#ifndef FruitAnimatedObject_H
#define FruitAnimatedObject_H

#include <string>
#include "PhysicalAnimatedObject.hpp"
#include "physics/PhysicsOnCollideListener.hpp"
#include "data/DataCacheManager.hpp"
#include "event/EventListener.hpp"

class PlatformManager;

using namespace std;

class FruitAnimatedObject : public PhysicalAnimatedObject, public PhysicsOnCollideListener
{
public:
    
    FruitAnimatedObject(
        PlatformManager* pPlatformManager, 
        DataCacheManager& rDataCacheManager, 
        PhysicsBody* pPhysicsBody,
        const string& animationFile, 
        const string& name, 
        const string& type, 
        EventListener* pEventListener);
    ~FruitAnimatedObject();

    const string& getName() const { return m_name; }

    void onCollideStart(PhysicsBody* pPhysicsBody) override;

private:

    string m_name;
    EventListener* m_pEventListener;
    bool m_isCollected;

    void initializeMembers()
    {
        m_name = "";
        m_pEventListener = nullptr;
        m_isCollected = false;
    }

};

#endif //FruitAnimatedObject_H
