#pragma once
#ifndef GroundObject_hpp 
#define GroundObject_hpp

#include "GameObject.hpp"
#include <string>

using namespace std;

class GroundObject : public GameObject
{
public:
    
    GroundObject() {}
    ~GroundObject() {};
    
    const string& getType() const override { return m_type; }

private:

    string m_type = "ground";
};

#endif /* GroundObject_hpp */
