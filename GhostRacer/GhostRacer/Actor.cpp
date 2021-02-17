#include "Actor.h"
#include "GameWorld.h"
#include "StudentWorld.h"
#include <cmath>
//#include "GameWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

Actor::Actor(int imageID, int startX, int startY, int dir, double sz, int depth)
: GraphObject(imageID, startX, startY, dir, sz, depth)
{
    //m_world = world;
}

Actor::~Actor()
{}

GhostRacer::GhostRacer(StudentWorld* world)
: Actor(IID_GHOST_RACER,128,32,90,4.0,0)
{
    m_speed = 0;
    m_holyWater = 10;
    m_hp = 100;
    avoidCollision = true;
    m_world = world;
}

GhostRacer::~GhostRacer()
{}

StudentWorld* GhostRacer::getWorld()
{
    return m_world;
}

void GhostRacer::doSomething()
{
    int ch;
    int direction = getDirection();
    double max_shift_per_tick = 4.0;
    
    //1. If the Ghost Racer is not currently alive (i.e., its hit points are zero or less), its doSomething() method must return immediately – none of the following steps should be performed.
    if(m_hp <= 0)
        return;
    //2. If the Ghost Racer's center X coordinate is less than or equal to the left road boundary (meaning it’s swerving off the road):
    if(getX() <= 0) //left boundary
    {
        if(direction > 90)  //if facing left
            m_hp -= 10;
        direction = 82;
        setDirection(direction);
        m_world->playSound(SOUND_VEHICLE_CRASH);
    }
    //3. If the Ghost Racer's center X coordinate is greater than or equal to the right road boundary
    if(getX() >= 255)   //right boundary
    {
        if(direction < 90)  //if facing right
            m_hp -= 10;
        direction = 98;
        setDirection(direction);
        m_world->playSound(SOUND_VEHICLE_CRASH);
    }
        
    //4. The Ghost Racer must check to see if the player pressed a key (the section below shows how to check this). If the player pressed a key:
    if(getWorld()->getKey(ch))
    {   //TODO: IMPLEMENT 4.A.I & 4.A.II
        switch(ch)
        {
            //a. Player pressed space key
            case KEY_PRESS_SPACE:
                break;
            //b. Player pressed left key
            case KEY_PRESS_LEFT:
                if(direction < 114)
                {
                    direction += 8;
                    setDirection(direction);
                }
                break;
            //c. player pressed right key
            case KEY_PRESS_RIGHT:
                if(direction > 66)
            {
                direction -= 8;
                setDirection(direction);
            }
                break;
            //d. player pressed up key
            case KEY_PRESS_UP:
                if(m_speed < 5)
                    m_speed++;
                break;
            //e. player pressed down key
            case KEY_PRESS_DOWN:
                if(m_speed > -1)
                    m_speed++;
                break;
        }
    }
        
    //5. Ghost Racer must attempt to move using the Ghost Racer Movement Algorithm described in the section below.
    int delta_x = cos( (direction) * M_PI / 180 ) * max_shift_per_tick;
    int cur_x = getX();
    int cur_y = getY();
    moveTo(cur_x + delta_x, cur_y);
}

bool GhostRacer::isAlive()
{ return true; }


