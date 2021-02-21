#include "Actor.h"
#include "StudentWorld.h"
#include <cmath>

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp


//ACTOR CLASS
Actor::Actor(int imageID, int startX, int startY, int dir, double sz, int depth,StudentWorld* world)
: GraphObject(imageID, startX, startY, dir, sz, depth)
{
    m_world = world;
    m_alive = true;
}

Actor::~Actor()
{}

GhostRacer::GhostRacer(StudentWorld* wrld)
: Actor(IID_GHOST_RACER,128,32,90,4.0,0,wrld)
{
    m_speed = 0;
    m_holyWater = 10;
    m_hp = 100;
    avoidCollision = true;
}

bool Actor::isOverlap(Actor* other)
{
    int delta_x = abs(getX() - other->getX());
    int delta_y = abs(getY() - other->getY());
    int radius_sum = getRadius() + other->getRadius();
    
    return ( delta_x < (radius_sum * 0.25) && (delta_y < radius_sum * 0.6) );
}

//GHOSTRACER CLASS
GhostRacer::~GhostRacer()
{}

void GhostRacer::doSomething()
{
    int ch;
    int direction = getDirection();
    double max_shift_per_tick = 4.0;
    //1. If the Ghost Racer is not currently alive (i.e., its hit points are zero or less), its doSomething() method must return immediately – none of the following steps should be performed.
    if(m_hp <= 0)
        return;
    //2. If the Ghost Racer's center X coordinate is less than or equal to the left road boundary (meaning it’s swerving off the road):
    if(getX() <= ROAD_CENTER - ROAD_WIDTH/2) //left boundary
    {
        if(direction > 90)  //if facing left
            m_hp -= 10;
        direction = 82;
        setDirection(direction);
        getWorld()->playSound(SOUND_VEHICLE_CRASH);
    }
    //3. If the Ghost Racer's center X coordinate is greater than or equal to the right road boundary
    if(getX() >= ROAD_CENTER + ROAD_WIDTH/2)   //right boundary
    {
        if(direction < 90)  //if facing right
            m_hp -= 10;
        direction = 98;
        setDirection(direction);
        getWorld()->playSound(SOUND_VEHICLE_CRASH);
    }
    //4. The Ghost Racer must check to see if the player pressed a key (the section below shows how to check this). If the player pressed a key:
    if(getWorld()->getKey(ch))
    {   //TODO: IMPLEMENT 4.A.I & 4.A.II
        switch(ch)
        {
            case KEY_PRESS_SPACE:
                break;
            case KEY_PRESS_LEFT:
                if(direction < 114)
                    direction += 8;
                break;
            case KEY_PRESS_RIGHT:
                if(direction > 66)
                    direction -= 8;
                break;
            case KEY_PRESS_UP:
                if(m_speed < 5)
                    m_speed++;
                break;
            case KEY_PRESS_DOWN:
                if(m_speed > 0)
                    m_speed--;
                break;
        }
    }
    //5. Ghost Racer must attempt to move using the Ghost Racer Movement Algorithm described in the section below.
    setDirection(direction);
    double delta_x = cos( (direction) * M_PI / 180 ) * max_shift_per_tick;
    double cur_x = getX();
    double cur_y = getY();
    moveTo(cur_x + delta_x, cur_y);
}
//TODO: implement method without trivial bool return type. instead return unique string? (see groupme screenshots)



//BORDERLINE CLASS
BorderLine::BorderLine(int imageID, int startX, int startY, int dir, double sz, int depth, StudentWorld* world, GhostRacer* racer)
: Actor(imageID, startX, startY, 0, 2.0,1,world)
{
    m_world = world;
    m_racer = racer;
    m_speed = -4;
    x = startX; y = startY;
    m_hp = 1;
    //m_alive = true;
    m_racerSpeed = 0;
}

BorderLine::~BorderLine()
{}

void BorderLine::doSomething()
{
    int v_speed = m_speed - m_racer->getSpeed();
    int h_speed = 0;
    double new_y = getY() + v_speed;
    double new_x = getX() + h_speed;
    moveTo(new_x,new_y);
    if(getY() < 0 || getY() > VIEW_HEIGHT)
        setDead();
}


//YELLOW CLASS
Yellow::Yellow(int x, int y, StudentWorld* world, GhostRacer* racer)
: BorderLine(IID_YELLOW_BORDER_LINE,x,y,0,2.0,1,world,racer)
{
    m_hp = 1;
    m_speed = -4;
}

Yellow::~Yellow() {}


//WHITE CLASS
White::White(int x, int y, StudentWorld* world, GhostRacer* racer)
: BorderLine(IID_WHITE_BORDER_LINE, x, y, 0, 2.0, 1, world, racer)
{
    m_hp = 1;
    m_speed = -4;
}

White::~White() {};

