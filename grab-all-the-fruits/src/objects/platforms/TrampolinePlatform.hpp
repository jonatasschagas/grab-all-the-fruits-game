#pragma once
#ifndef TrampolinePlatform_H
#define TrampolinePlatform_H

#include <string>
#include "objects/PhysicalAnimatedObject.hpp"
#include "physics/PhysicsOnCollideListener.hpp"
#include "data/DataCacheManager.hpp"
#include "event/EventListener.hpp"

class PlatformManager;

using namespace std;

class TrampolinePlatform : public PhysicalAnimatedObject, public PhysicsOnCollideListener
{
public:
    
    TrampolinePlatform(
        PlatformManager* pPlatformManager, 
        DataCacheManager& rDataCacheManager, 
        PhysicsBody* pPhysicsBody,
        const string& animationFile, 
        const string& name, 
        const string& type, 
        EventListener* pEventListener);
    ~TrampolinePlatform();

    const string& getName() const { return m_name; }

    void onCollideStart(PhysicsBody* pPhysicsBody) override;
    
private:

    string m_name;
    EventListener* m_pEventListener;

    void initializeMembers()
    {
        m_name = "";
        m_pEventListener = nullptr;
    }

};

#endif //TrampolinePlatform_H
