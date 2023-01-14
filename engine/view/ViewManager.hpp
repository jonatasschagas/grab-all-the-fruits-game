#pragma once
#ifndef ViewManager_hpp
#define ViewManager_hpp

#include "../event/EventListener.hpp"
#include "View.h"
#include "../platform/PlatformManager.h"
#include "../data/DataCacheManager.hpp"

#include <stdio.h>
#include <vector>
#include <string>
#include <map>

using namespace std;

class ViewManager : public EventListener
{
public:
    
    ~ViewManager();
    
    void receiveEvent(Event* pEvent);
    void render();
    void readInput(int x, int y, bool pressed);
    void update(float delta);
    
    void updateEditor();
    
    void addView(const string& viewName, View* pView);
    void switchView(const string& viewName);
    
    DataCacheManager* getDataCacheManager();
    
    static void setPlatformManager(PlatformManager* pPlatformManager);
    static PlatformManager* getPlatformManager();
    static void publishEvent(Event* pEvent);

    static ViewManager* getInstance()
    {
        if (s_pViewManager == nullptr)
        {
            s_pViewManager = new ViewManager();
        }
        return s_pViewManager;
    }

private:
    
    ViewManager();
    
    static PlatformManager* s_pPlatformManager;
    static ViewManager* s_pViewManager;

    DataCacheManager m_dataCacheManager;
    
    View* m_pCurrentView;
	View* m_pPreviousView;

    map<string, View*> m_views;
    
    void initializeMembers()
    {
        m_pCurrentView = nullptr;
		m_pPreviousView = nullptr;
        m_views.clear();
    }
    
};

#endif /* ViewManager_hpp */
