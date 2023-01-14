#include "ViewManager.hpp"

PlatformManager* ViewManager::s_pPlatformManager = 0;
PlatformManager* ViewManager::getPlatformManager()
{
    return s_pPlatformManager;
}

ViewManager* ViewManager::s_pViewManager = 0;

void ViewManager::setPlatformManager(PlatformManager* pPlatformManager)
{
    s_pPlatformManager = pPlatformManager;
}

ViewManager::ViewManager()
{
    initializeMembers();
}

ViewManager::~ViewManager()
{
    delete m_pCurrentView;
    delete s_pPlatformManager;
    
    initializeMembers();
}

void ViewManager::receiveEvent(Event* pEvent)
{
    m_pCurrentView->receiveEvent(pEvent);
}

void ViewManager::publishEvent(Event* pEvent)
{
    s_pViewManager->receiveEvent(pEvent);
}

void ViewManager::render()
{
    if (m_pCurrentView != nullptr) 
    {
        m_pCurrentView->render();
    }
}

void ViewManager::readInput(int x, int y, bool pressed)
{
	m_pCurrentView->readInput(x, y, pressed);
}

void ViewManager::update(float delta)
{
    if (m_pCurrentView != nullptr) 
    {
        m_pCurrentView->update(delta);
    }
    
	// safely deleting previous view now
	if (m_pPreviousView != nullptr)
	{
		delete m_pPreviousView;
		m_pPreviousView = nullptr;
	}
	
}

void ViewManager::updateEditor()
{
    m_pCurrentView->updateEditor();
}

void ViewManager::switchView(const string& viewName)
{

    if (m_views.find(viewName) != m_views.end())
    {
        if (m_pCurrentView != nullptr)
        {
            delete m_pCurrentView;
        }
        
        m_pCurrentView = m_views[viewName];
        m_pCurrentView->initialize(this);
    }
    else
    {
        printf("ViewManager::switchView: View %s not found)", viewName.c_str());
    }
}

void ViewManager::addView(const string& viewName, View* pView)
{
    m_views[viewName] = pView;
}

DataCacheManager* ViewManager::getDataCacheManager()
{
    return &m_dataCacheManager;
}