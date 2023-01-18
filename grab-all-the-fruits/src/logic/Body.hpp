#pragma once
#ifndef Body_hpp 
#define Body_hpp

#include <string>

using namespace std;

struct Body
{
    string m_name;

    float m_y;
    float m_vy;
    float m_dropHeight;
    
    float m_x;
    float m_vx;
    float m_vxGoal;
    float m_vxAir;
    float m_adjustX;
    bool m_adjustingX;
    
    float m_width;
    float m_height;
    
    bool m_grounded;
    bool m_decelerating;

    bool m_jumping;
    bool m_doubleJumping;

    bool m_facingRight;
};

#endif /* Body_hpp */
