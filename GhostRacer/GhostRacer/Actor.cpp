#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include <cmath>

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp


//ACTOR CLASS
Actor::Actor(int imageID, double startX, double startY, int dir, double sz, int depth, StudentWorld* world)
: GraphObject(imageID, startX, startY, dir, sz, depth), m_world(world), m_isDead(false), m_speed(0), m_waterActive(false), m_score(0000)
{}


Actor::~Actor()
{}

bool Actor::isOverlap(Actor* racer)
{
    double delta_x = abs(getX() - racer->getX());
    double delta_y = abs(getY() - racer->getY());
    double radius_sum = getRadius() + racer->getRadius();
            
    return ( delta_x < (radius_sum * 0.25) && (delta_y < radius_sum * 0.6) );
}

Agent::Agent(int imageID, double startX, double startY, int dir, double sz, int hp, StudentWorld* world)
: Actor(imageID, startX, startY, dir, sz, 0, world), m_hp(hp)
{}

Agent::~Agent() {}


GhostRacer::GhostRacer(double startX, double startY, StudentWorld* world)
: Agent(IID_GHOST_RACER,128,32,90,4.0,100,world), m_waterSprays(10), m_souls(0)
{
    setVerticalSpeed(0);
}

GhostRacer::~GhostRacer() {}


void GhostRacer::doSomething()
{
    int ch;
    int direction = getDirection();
    double max_shift_per_tick = 4.0;
    if(getHP() <= 0)
        return;
    if(getX() <= ROAD_CENTER - ROAD_WIDTH/2) //left boundary
    {
        if(direction > 90)  //if facing left
            changeHP(-10);
        direction = 82;
        setDirection(direction);
        getWorld()->playSound(SOUND_VEHICLE_CRASH);
    }
    if(getX() >= ROAD_CENTER + ROAD_WIDTH/2)   //right boundary
    {
        if(direction < 90)  //if facing right
            changeHP(-10);
        direction = 98;
        setDirection(direction);
        getWorld()->playSound(SOUND_VEHICLE_CRASH);
    }
    if(getWorld()->getKey(ch))
    {
        switch(ch)
        {
            case KEY_PRESS_SPACE:
                if(getNumSprays() > 0)
                {
                    int spray_x;
                    if(direction > 90)
                        spray_x = getX() - SPRITE_HEIGHT;
                    if(direction < 90)
                        spray_x = getX() + SPRITE_HEIGHT;
                    else
                        spray_x = getX();
                    //Spray* new_spray = new Spray(spray_x,getY() + SPRITE_HEIGHT,direction,getWorld());
                    //TODO: make flag here to signal to StudentWorld that a new spray needs to be added
                    setWater(true);
                    getWorld()->playSound(SOUND_PLAYER_SPRAY);
                    increaseSprays(-1);
                }
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
                if(getVerticalSpeed() < 5)
                    setVerticalSpeed(getVerticalSpeed() + 1);
                break;
            case KEY_PRESS_DOWN:
                if(getVerticalSpeed() > -1)
                    setVerticalSpeed(getVerticalSpeed() - 1);
                break;
        }
    }
    setDirection(direction);
    double delta_x = cos( (direction) * M_PI / 180 ) * max_shift_per_tick;
    double cur_x = getX();
    double cur_y = getY();
    moveTo(cur_x + delta_x, cur_y);
}

void GhostRacer::spin()
{
    int randDir;
    int randClock = randInt(0,1);
    randDir = (randClock == 0) ? (randDir = randInt(5,20)) : (randDir = randInt(-20,-5));//turn randomly in CW or CCW direction
        //andDir = randInt(5,20);
   // else    //turn randomly in counter-clockwise direction
        //randDir = randInt(-20,-5);
    
    int new_dir = getDirection() + randDir;
    if(new_dir < 60)
        setDirection(60);
    if(new_dir > 120)
        setDirection(120);
    else
        setDirection(new_dir);
}


//BORDERLINE CLASS
BorderLine::BorderLine(int imageID, double startX, double startY, StudentWorld* world)
: Actor(imageID, startX, startY, 0, 2.0, 2, world)
{
    setVerticalSpeed(-4);
}

BorderLine::~BorderLine()
{}

void BorderLine::doSomething()
{
    GhostRacer* racer = getWorld()->getGhostRacer();
    int v_speed = getVerticalSpeed() - racer->getVerticalSpeed();
    double new_y = getY() + v_speed;
    double new_x = getX();
    moveTo(new_x,new_y);
    if(getY() < 0 || getY() > VIEW_HEIGHT)
        setDead();
}


//YELLOW CLASS
Yellow::Yellow(double x, double y, StudentWorld* world)
: BorderLine(IID_YELLOW_BORDER_LINE, x, y,world)
{}

Yellow::~Yellow() {}


//WHITE CLASS
White::White(double x, double y, StudentWorld* world)
: BorderLine(IID_WHITE_BORDER_LINE, x, y, world)
{}

White::~White() {};


Spray::Spray(double x, double y, int dir, StudentWorld* world)
: Actor(IID_HOLY_WATER_PROJECTILE, x, y, dir, 1.0, 1, world)
{}

Spray::~Spray() {}

void Spray::doSomething()
{
    //Actor* other;
    if(isDead())
        return;
    //TODO: IMPLEMENT P. 46 PSEUDOCODE
    //TODO: FIX STARTING POSITION IN STUDENTWORLD.CPP
    if(getWorld()->getGhostRacer()->hasActiveWater())
    {
        //if(
    }
}


GhostRacerActivatedObject::GhostRacerActivatedObject(int imageID, double x, double y, int dir, double size, StudentWorld* world)
: Actor(imageID, x, y, dir, size, 2, world)
{
    setVerticalSpeed(-4);
}

GhostRacerActivatedObject::~GhostRacerActivatedObject() {}

void GhostRacerActivatedObject::doSomething()
{
    double vert_speed = getVerticalSpeed() - getWorld()->getGhostRacer()->getVerticalSpeed();
    double new_y = getY() + vert_speed;
    double new_x = getX();
    moveTo(new_x,new_y);
    if( isOffScreen() )
    {
        setDead();
        return;
    }
    if(isOverlap(getWorld()->getGhostRacer()))
    {
        getWorld()->playSound(getSound());
        doActivity(getWorld()->getGhostRacer());
        setDead();
    }
}


OilSlick::OilSlick(double x, double y, StudentWorld* world)
: GhostRacerActivatedObject(IID_OIL_SLICK, x, y, 0, randInt(2,5),world)
{}

OilSlick::~OilSlick() {}

void OilSlick::doSomething()
{
    double vert_speed = getVerticalSpeed() - getWorld()->getGhostRacer()->getVerticalSpeed();
    double new_y = getY() + vert_speed;
    double new_x = getX();
    moveTo(new_x,new_y);
    if( (getX() < 0 || getX() > VIEW_WIDTH) || (getY() < 0 || getY() > VIEW_HEIGHT) )
    {
        setDead();
        return;
    }
    doActivity(getWorld()->getGhostRacer());
}

void OilSlick::doActivity(GhostRacer* racer)
{
    double delta_x = abs(getX() - racer->getX());
    double delta_y = abs(getY() - racer->getY());
    double radius_sum = getRadius() + racer->getRadius();
        
    if( delta_x < (radius_sum * 0.25) && (delta_y < radius_sum * 0.6) )
    {
        getWorld()->playSound(getSound());
        racer->spin();
    }
}

HealingGoodie::HealingGoodie(double x, double y, StudentWorld* world)
: GhostRacerActivatedObject(IID_HEAL_GOODIE, x, y, 0, 1.0, world)
{
    
}

HealingGoodie::~HealingGoodie() {}



void HealingGoodie::doActivity(GhostRacer *racer)
{
    racer->changeHP(10);
    racer->changeScore(250);
}

HolyWaterGoodie::HolyWaterGoodie(double x, double y, StudentWorld* world)
: GhostRacerActivatedObject(IID_HOLY_WATER_GOODIE, x, y, 90, 2.0, world)
{
}

HolyWaterGoodie::~HolyWaterGoodie() {}


void HolyWaterGoodie::doActivity(GhostRacer *racer)
{
    racer->increaseSprays(10);
    racer->changeScore(50);
}
