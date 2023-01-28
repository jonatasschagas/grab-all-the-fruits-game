#pragma once
#ifndef GameObject_hpp 
#define GameObject_hpp

#include <string>

using namespace std;

class GameObject
{
public:
    
    GameObject() {}
    ~GameObject() {};
    
    virtual const string& getType() const = 0;
};

#endif /* GameObject_hpp */
