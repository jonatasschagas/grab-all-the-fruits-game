#pragma once
#ifndef AnimatedObject_H
#define AnimatedObject_H

#include <string>
#include "core/AnimatedSprite.hpp"
#include "GameObject.hpp"

using namespace std;

class AnimatedObject : public AnimatedSprite, public GameObject
{
public:
    
    AnimatedObject(PlatformManager* pPlatformManager, DataCacheManager& rDataCacheManager, const string& animationFile, const string& type);
    ~AnimatedObject();

    const string& getType() const override { return m_type; };

    void update(float deltaTime) override;

private:

    string m_type;

    void initializeMembers()
    {
        m_type = "";
    }

};

#endif //AnimatedObject_H
