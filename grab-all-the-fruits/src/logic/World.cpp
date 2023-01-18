#include "World.hpp"
#include "Body.hpp"

#include "utils/MathUtils.h"

#include <string>

World::World()
{
    initializeMembers();
}

World::~World()
{
}

void World::update(float delta)
{
    for (int i = 0; i < m_bodies.size(); i++)
    {
        
        Body* pBody = m_bodies[i];
    
        if (pBody->m_decelerating)
            pBody->m_vxGoal = ((pBody->m_vxGoal * 0.9f) - pBody->m_vxGoal)/delta;

        bool facingRight =  pBody->m_facingRight;
        float newVx = calculateVXStep(pBody->m_vx, pBody->m_vxGoal, delta);
        float nextX = calculateXStep(pBody->m_x, newVx, delta);

        float diffAdjusting = abs(pBody->m_adjustX - pBody->m_x);
        // when the x position is very close to be adjusted we stop adjusting
        if (diffAdjusting < 0.1f)
            pBody->m_adjustingX = false;

        if (pBody->m_adjustingX)
        {
            // in here we make sure that the character won't seep inside the wall
            // by adjusting his position on the x axis little by little each frame
            pBody->m_x = lerp(pBody->m_x, pBody->m_adjustX, delta);
        }
        
        //TODO: hook up the map manager        
        //const bool willCollideWithWall = m_rMapManager.isWall(nextX, pc->m_y, pc->m_width, pc->m_height, facingRight);
        const bool willCollideWithWall = nextX < 0 || nextX > 100;
        if (!willCollideWithWall)
        {
            pBody->m_x = nextX;
        }
        pBody->m_vx = newVx;

        float nextY = pBody->m_y + pBody->m_vy * delta;
        //TODO: hook up the map manager        
        //float groundTileY = m_rMapManager.getGroundTileY(pBody->m_x, pBody->m_y);
        //bool isCeiling = m_rMapManager.isGround(pBody->m_x, nextY);
        float groundTileY = 1;
        bool isCeiling = false;
        
        if (nextY > pBody->m_y && isCeiling)
        {
            // not allowing character to go thru the roof
            pBody->m_vy = 0;
            pBody->m_grounded = false;
        }
        else if (nextY <= groundTileY && groundTileY > 0)
        {
            // grounding
            pBody->m_y = groundTileY;
            pBody->m_vy = 0;

            pBody->m_decelerating = false;
            pBody->m_grounded = true;
            pBody->m_doubleJumping = false;

            //TODO: hook up the map manager
            // adjust m_x so that character won't go inside the wall
            /*
            const bool collidesWithWallFront = m_rMapManager.isWall(pBody->m_x, pBody->m_y, pBody->m_width, pBody->m_height, true);
            const bool collidesWithWallBack = m_rMapManager.isWall(pBody->m_x, pBody->m_y, pBody->m_width, pBody->m_height, false);
            if(!pBody->m_adjustingX && (collidesWithWallFront || collidesWithWallBack))
            {
                if (collidesWithWallFront)
                    pBody->m_adjustX = m_rMapManager.getTileXFloor(pBody->m_x) * m_rGameConfigurationData.getTileSizeInWorldUnits();
                else
                    pBody->m_adjustX = m_rMapManager.getTileXCeil(pBody->m_x) * m_rGameConfigurationData.getTileSizeInWorldUnits();

                pBody->m_adjustingX = true;
            }
            */
        }
         else
        {
            if (pBody->m_vy <= 0)
            {
                pBody->m_dropHeight += (pBody->m_y - nextY);
            }
            else
            {
                pBody->m_dropHeight = 0;
            }
            
            pBody->m_y = nextY;
            pBody->m_vy = pBody->m_vy + m_gravity * delta;
            pBody->m_grounded = false;
        }
    }
}

const Body* World::createBody(const string& name, const Vector2& position, const GameSize& size)
{
    Body* pBody = new Body();
    pBody->m_name = name;
    pBody->m_x = position.x;
    pBody->m_y = position.y;
    pBody->m_width = size.w;
    pBody->m_height = size.h;
    m_bodies.push_back(pBody);
    return pBody;
}

float World::calculateVXStep(float vx, float vxGoal, float deltaTime)
{
    return lerp(vx, vxGoal, deltaTime);
}

float World::calculateXStep(float x, float vx, float deltaTime)
{
    float nextX = x + vx * deltaTime;
    return nextX;
}

void World::receiveEvent(Event* pEvent) {
    for (int i = 0; i < m_bodies.size(); i++)
    {   
        Body* pBody = m_bodies[i];
        if (strcmp(pBody->m_name.c_str(), pEvent->getTarget().c_str()) == 0) {
            if (strcmp(pEvent->getName().c_str(), "right_start") == 0) {
                pBody->m_vxGoal = PLAYER_RUNNING_SPEED;
            } else if (strcmp(pEvent->getName().c_str(), "right_stop") == 0) {
                pBody->m_vxGoal = 0;
            } else if (strcmp(pEvent->getName().c_str(), "left_start") == 0) {
                pBody->m_vxGoal = -PLAYER_RUNNING_SPEED;
            } else if (strcmp(pEvent->getName().c_str(), "left_stop") == 0) {
                pBody->m_vxGoal = 0;
            } else if (strcmp(pEvent->getName().c_str(), "space_start") == 0) {
                if (pBody->m_grounded) {
                    pBody->m_vy = PLAYER_JUMPING_SPEED;
                    pBody->m_grounded = false;
                } else if (!pBody->m_doubleJumping) {
                    pBody->m_vy = PLAYER_JUMPING_SPEED;
                    pBody->m_doubleJumping = true;
                }
            } else if (strcmp(pEvent->getName().c_str(), "space_stop") == 0) {
                if (pBody->m_vy > 0) {
                    pBody->m_vy = 0;
                }
            }
        }
    }
}