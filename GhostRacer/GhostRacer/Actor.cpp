#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include <cmath>

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp


//ACTOR CLASS
Actor::Actor(int imageID, double startX, double startY, int dir, double sz, int depth, StudentWorld* world)
: GraphObject(imageID, startX, startY, dir, sz, depth), m_world(world), m_isDead(false), /*m_vertSpeed(0), m_horizSpeed(0), */m_score(0), m_speed(0)
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
: Agent(IID_GHOST_RACER,128,32,90,4.0,100,world), m_waterSprays(10), m_souls(0), m_waterActive(false), m_lives(3), m_lostLife(false)
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
    randDir = (randClock == 0) ? (randDir = randInt(5,20)) : (randDir = randInt(-20,-5));   //turn randomly in CW or CCW direction
    
    int new_dir = getDirection() + randDir;
    if(new_dir < 60)
        setDirection(60);
    if(new_dir > 120)
        setDirection(120);
    else
        setDirection(new_dir);
}


Pedestrian::Pedestrian(int imageID, double x, double y, double size, StudentWorld* world)
: Agent(imageID, x, y, 0, size, 2, world), m_hSpeed(0), m_plan(0)
{
    setVerticalSpeed(-4);
}

Pedestrian::~Pedestrian() {}

void Pedestrian::moveAndPossiblyPickPlan()
{
    int vert_speed = getVerticalSpeed() - getWorld()->getGhostRacer()->getVerticalSpeed();
    int horiz_speed = getHorizSpeed();
    double new_y = getY() + vert_speed;
    double new_x = getX() + horiz_speed;
    moveTo(new_x, new_y);
    if(isOffScreen())
    {
        setDead();
        return;
    }
    if(isHuman())
    {
        incPlan(-1);
        if(getPlan() > 0)
            return;
    }
    else
    {
        if(getPlan()  > 0)
        {
            incPlan(-1);
            return;
        }
    }
    while(getHorizSpeed() == 0)
    {
        setHorizSpeed(randInt(-3,3));
    }
    setPlan(randInt(4,32));
    if(getHorizSpeed() < 0)
        setDirection(180);
    else
        setDirection(0);
}


HumanPedestrian::HumanPedestrian(double x, double y, StudentWorld* sw)
: Pedestrian(IID_HUMAN_PED, x, y, 2.0, sw)
{}

HumanPedestrian::~HumanPedestrian() {}

void HumanPedestrian::doSomething()
{
    if(isDead())
        return;
    GhostRacer* racer = getWorld()->getGhostRacer();
    if(isOverlap(racer))
    {
        racer->setHP(0);
        return;
    }
    moveAndPossiblyPickPlan();
    if(isDead())
        return;
    if(getPlan() > 0)
        return;
    
}


ZombiePedestrian::ZombiePedestrian(double x, double y, StudentWorld* sw)
: Pedestrian(IID_ZOMBIE_PED, x, y, 3.0, sw), m_ticksGrunt(0)
{}

ZombiePedestrian::~ZombiePedestrian() {}

void ZombiePedestrian::doSomething()
{
    if(isDead())
        return;
    GhostRacer* racer = getWorld()->getGhostRacer();
    if(isOverlap(racer))
    {
        racer->changeHP(-5);
        changeHP(-2);
        setDead();
        return;
    }
    if(abs(getX() - racer->getX()) <= 30.0 && getY() > racer->getY())
    {
        setDirection(270);
        if(getX() < racer->getX())
            setHorizSpeed(1);
        else if (getX() > racer->getX())
            setHorizSpeed(-1);
        else
            setHorizSpeed(0);
        m_ticksGrunt--;
        if(m_ticksGrunt <= 0)
        {
            getWorld()->playSound(SOUND_ZOMBIE_ATTACK);
            m_ticksGrunt = 20;
        }
    }
    moveAndPossiblyPickPlan();
    
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
: Actor(IID_HOLY_WATER_PROJECTILE, x, y, dir, 1.0, 1, world), m_pixelsMoved(0)
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
        //hasOverlapped();
        moveForward(SPRITE_HEIGHT);
        m_pixelsMoved += SPRITE_HEIGHT;
        if(isOffScreen())
        {
            setDead();
            return;
        }
        if(m_pixelsMoved == 160)
        {
            setDead();
            getWorld()->getGhostRacer()->setWater(false);
        }
    }
}

void Spray::hasOverlapped(/*Actor* other*/)
{
    if(/*isOverlap(other) && other->beSprayedIfAppropriate()*/1 == 0)
    {
        /*if(other->isHuman())
            //REVERSE DIRECTION
        else
            other->changeHP(-1);
         setDead();
         return;
         */
        return;
    }
    else
    {
        moveForward(SPRITE_HEIGHT);
        m_pixelsMoved++;
    }
    if(isOffScreen())
    {
        setDead();
        return;
    }
    if(m_pixelsMoved == 160)
    {
        setDead();
        getWorld()->getGhostRacer()->setWater(false);
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
    double delta_x = fabs(getX() - racer->getX());
    double delta_y = fabs(getY() - racer->getY());
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
    if(racer->getHP() > 100)
        racer->setHP(100);
    racer->setScore(250);
}

HolyWaterGoodie::HolyWaterGoodie(double x, double y, StudentWorld* world)
: GhostRacerActivatedObject(IID_HOLY_WATER_GOODIE, x, y, 90, 2.0, world)
{
}

HolyWaterGoodie::~HolyWaterGoodie() {}


void HolyWaterGoodie::doActivity(GhostRacer *racer)
{
    racer->increaseSprays(10);
    racer->setScore(50);
}


SoulGoodie::SoulGoodie(double x, double y, StudentWorld* world)
: GhostRacerActivatedObject(IID_SOUL_GOODIE, x, y, 0, 4.0, world)
{
}

SoulGoodie::~SoulGoodie() {}

void SoulGoodie::doActivity(GhostRacer* racer)
{
    racer->incSouls();
    racer->setScore(100);
}

void SoulGoodie::doSomething()
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
    setDirection(getDirection() + 10);
}
