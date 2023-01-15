#pragma once
#ifndef Event_hpp
#define Event_hpp

#include <stdio.h>
#include <string>
#include "core/BaseDataStructures.h"

using namespace std;

class Event
{
public:
    
    Event() { initializeMembers(); };
	Event(const string& eventName) { initializeMembers(); m_eventName = eventName; };
    Event(const string& eventName, void* pParam) { initializeMembers(); m_eventName = eventName; m_pParam = pParam;}
    Event(const string& eventName, int iData) { initializeMembers(); m_eventName = eventName; m_iData = iData;}
    ~Event() {};
    
    const string& getName() const { return m_eventName; };
    void setName(const string& eventName) { m_eventName = eventName; };
    
    void setParam(void* pParam) { m_pParam = pParam; };
    void* getParam() const { return m_pParam; };

    const int getData() const { return m_iData; };
    void setData(int iData) { m_iData = iData; };

    const GamePoint& getInputCoordinates() const { return m_inputCoordinates; };
    void setInputCoordinates(const GamePoint& inputCoordinates) { m_inputCoordinates = inputCoordinates; };

    void clone(Event* pOther) {
        m_eventName = pOther->getName();
        m_pParam = pOther->getParam();
        m_iData = pOther->getData();
    }
    
private:
    
    string m_eventName;
    void* m_pParam;
    int m_iData;
    GamePoint m_inputCoordinates;
    
    void initializeMembers() { 
        m_pParam = 0; 
        m_iData = 0;
    }
    
};

#endif /* Event_hpp */
