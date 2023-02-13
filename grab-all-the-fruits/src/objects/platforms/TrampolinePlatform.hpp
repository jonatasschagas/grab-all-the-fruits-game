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

    void onCollideStart(PhysicsBody* pPhysicsBody) override;
    
private:

    EventListener* m_pEventListener;

    void initializeMembers()
    {
        m_pEventListener = nullptr;
    }

};

#endif //TrampolinePlatform_H
