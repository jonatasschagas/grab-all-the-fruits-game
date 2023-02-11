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
    
    AnimatedObject(PlatformManager* pPlatformManager, DataCacheManager& rDataCacheManager, const string& animationFile, const string& type, const string& name);
    ~AnimatedObject();

    const string& getType() const override { return m_type; };
    const string& getName() const override { return m_name; };

    void update(float deltaTime) override;

private:

    string m_type;
    string m_name;

    void initializeMembers()
    {
        m_type = "";
        m_name = "";
    }

};

#endif //AnimatedObject_H
