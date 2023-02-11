#include "GroundObject.hpp"

#include "physics/PhysicsBody.hpp"
#include "objects/obstacles/FallingBlockObstacle.hpp"

GroundObject::GroundObject(EventListener* pEventListener)
{
    initializeMembers();
    m_pEventListener = pEventListener;
}

GroundObject::~GroundObject()
{
    initializeMembers();
}
    
void GroundObject::onCollideStart(PhysicsBody* pPhysicsBody)
{
    GameObject* pGameObject = pPhysicsBody->getGameObject();
    if (pGameObject->getName().compare("falling-block") == 0)
    {
        FallingBlockObstacle* pFallingBlock = dynamic_cast<FallingBlockObstacle*>(pGameObject);
        if (pFallingBlock->hasFallen())
        {
            Event event("ground-collide");
            //TODO: Parameterize this -> intensity of the collision for camera shake
            event.setFloatData(5);
            m_pEventListener->receiveEvent(&event);
        }
    }
}
