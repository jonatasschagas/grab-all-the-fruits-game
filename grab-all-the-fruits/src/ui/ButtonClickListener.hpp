#pragma once
#ifndef ButtonClickListener_hpp 
#define ButtonClickListener_hpp

#include <string>

using namespace std;

class ButtonClickListener
{
public:
    
    virtual ~ButtonClickListener() {};
    
    virtual void onClick(const string& rButtonName) = 0;

};

#endif /* ButtonClickListener_hpp */
