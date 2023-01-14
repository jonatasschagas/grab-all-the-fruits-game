#ifndef View_h
#define View_h

#include "../core/Sprite.hpp"
#include "../event/EventListener.hpp"

class ViewManager;

class View : public Sprite
{
public:
    
	virtual void initialize(ViewManager* pViewManager) = 0;
    virtual void receiveEvent(Event* pEvent) = 0;
    virtual void readInput(int x, int y, bool pressed) = 0;
    virtual void updateEditor() {};
    
};


#endif /* View_h */
