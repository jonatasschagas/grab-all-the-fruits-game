#pragma once
#ifndef Body_hpp 
#define Body_hpp

#include <string>
#include "GameConfiguration.h"
#include <box2d/box2d.h>

using namespace std;

class Body
{
public:
    
    string m_name;
    b2Body* m_pb2Body;

    Body(){}
    Body(const string& name, b2Body* pb2Body) : m_name(name), m_pb2Body(pb2Body) {}

    b2Vec2 getPositionInPixels() const { 
        const b2Vec2& position = m_pb2Body->GetPosition();
        return b2Vec2(position.x * PIXELS_PER_METER, position.y * PIXELS_PER_METER); 
    }

    b2Vec2 getSizeInPixels() const { 
        const b2Shape* pShape = m_pb2Body->GetFixtureList()->GetShape();
        if (pShape->GetType() == b2Shape::Type::e_circle)
        {
            return b2Vec2(pShape->m_radius * 2 * PIXELS_PER_METER, pShape->m_radius * 2 * PIXELS_PER_METER); 
        } 
        else 
        {
            b2AABB shapeAABB;
            b2Transform t;
            pShape->ComputeAABB(&shapeAABB, t, 0);
            float bodyWidth = shapeAABB.upperBound.x - shapeAABB.lowerBound.x;
            float bodyHeight = shapeAABB.upperBound.y - shapeAABB.lowerBound.y;

            return b2Vec2(bodyWidth * PIXELS_PER_METER, bodyHeight * PIXELS_PER_METER); 
        }
    }

};

#endif /* Body_hpp */
