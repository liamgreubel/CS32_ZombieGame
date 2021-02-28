#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include <cmath>

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

const int LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH/2;
const int RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH/2;
const int left_middle = LEFT_EDGE + (ROAD_WIDTH / 3);
const int right_middle = RIGHT_EDGE - (ROAD_WIDTH / 3);

//ACTOR CLASS
Actor::Actor(int imageID, double startX, double startY, int dir, double sz, int depth, StudentWorld* world)
: GraphObject(imageID, startX, startY, dir, sz, depth), m_world(world), m_isDead(false), m_vertSpeed(0), m_horizSpeed(0), m_score(0)
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

void Actor::movement()
{
    GhostRacer* racer =getWorld()->getGhostRacer();
    int vert_speed = getVerticalSpeed() - racer->getVerticalSpeed();
    int horiz_speed = getHorizSpeed();
    double new_y = getY() + vert_speed;
    double new_x = getX() + horiz_speed;
    moveTo(new_x, new_y);
    if(isOffScreen())
        setDead();
}

int Actor::checkLane(Actor* a)  //returns lane with corresponding actors. if no lane nor are collisionAvoidance, then returns 0
{
    if(a->isCollisionAvoidanceWorthy())
    {
        return getWorld()->hasCollInLane(a);
    }
    return -1;
}

int Actor::getLane()//determine which lane an actor is in
{
    const int NO_LANE = 0;  //actors aren't in the same lane on the road
    const int LEFT_LANE = 1;
    const int MIDDLE_LANE = 2;
    const int RIGHT_LANE = 3;
    if(getX() > LEFT_EDGE && getX() < left_middle)
        return LEFT_LANE;
    else if(getX() > left_middle && getX() < right_middle)
        return MIDDLE_LANE;
    else if(getX() > right_middle && getX() < RIGHT_EDGE)
        return RIGHT_LANE;
    else
        return NO_LANE;
}



Agent::Agent(int imageID, double startX, double startY, int dir, double sz, StudentWorld* world)
: Actor(imageID, startX, startY, dir, sz, 0, world)
{}

Agent::~Agent() {}


GhostRacer::GhostRacer(double startX, double startY, StudentWorld* world)
: Agent(IID_GHOST_RACER,128,32,90,4.0,world), m_waterSprays(10), m_souls(0), m_waterActive(false), m_lives(3), m_lostLife(false), m_shotWater(false)
{
    setVerticalSpeed(0);
    setHorizSpeed(0);
    setHP(100);
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
                    //setShot(true);
                    int dir = getDirection();
                    double delta_x, delta_y;
                    if(getDirection() > 90)
                    {
                        delta_x = (SPRITE_HEIGHT) * cos( (getDirection() * M_PI / 180) );
                        delta_y = (SPRITE_HEIGHT) * sin( (getDirection() * M_PI / 180) );
                    }
                    if(getDirection() < 90)
                    {
                        delta_x = (SPRITE_HEIGHT) * cos( (getDirection() * M_PI / 180) );
                        delta_y = (SPRITE_HEIGHT) * sin( (getDirection() * M_PI / 180) );
                    }
                    else
                    {
                        delta_x = 0;
                        delta_y = 0;
                    }
                    dir = getDirection();
                    Spray* spray = new Spray(delta_x+getX(),delta_y+getY(),dir,getWorld());
                    getWorld()->addActor(spray);
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
    /*int randClock = randInt(0,1);
    randDir = randInt(5,20);
    if(randClock == 1)
        newDir = getDirection() + randDir;
    else
        newDir = getDirection() - randDir;*/
    
    randDir = randInt(5,20);
    int CW = getDirection() - randDir;
    int CCW = getDirection() + randDir;
    
    if(CW < 60)
        setDirection(CCW);
    else if(CCW > 120)
        setDirection(CW);
    else
        setDirection(CW);
}

/*bool GhostRacer::hasActiveWater()
{
    return getWorld()->sprayFirstAppropriateActor(Actor *a)
}*/


Pedestrian::Pedestrian(int imageID, double x, double y, double size, StudentWorld* world)
: Agent(imageID, x, y, 0, size, world), m_hSpeed(0), m_plan(0)
{
    setVerticalSpeed(-4);
    setHorizSpeed(0);
    setHP(2);
}

Pedestrian::~Pedestrian() {}

void Pedestrian::moveAndPossiblyPickPlan()
{
    double vert_speed = getVerticalSpeed() - getWorld()->getGhostRacer()->getVerticalSpeed();
    double horiz_speed = getHorizSpeed();
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
{
    setHorizSpeed(0);
    setVerticalSpeed(-4);
    setHP(2);
}

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
    //moveAndPossiblyPickPlan();
    double vert_speed = getVerticalSpeed() - getWorld()->getGhostRacer()->getVerticalSpeed();
    double horiz_speed = getHorizSpeed();
    double new_y = getY() + vert_speed;
    double new_x = getX() + horiz_speed;
    moveTo(new_x, new_y);
    if(isOffScreen())
    {
        setDead();
        return;
    }
    incPlan(-1);
    if(getPlan() > 0)
        return;
    else
    {
        int new_h = randInt(-3,3);
        if(new_h != 0)
        {
            setHorizSpeed(new_h);
        }
        setPlan(randInt(4,32));
        if(getHorizSpeed() < 0)
            setDirection(180);
        else
            setDirection(0);
        if(isDead())
            return;
        if(getPlan() > 0)
            return;
    }
}

bool HumanPedestrian::beSprayedIfAppropriate()
{
    setHorizSpeed(getHorizSpeed() * -1);
    int new_dir;
    if(getDirection() == 0)
        new_dir = 180;
    else
        new_dir = 0;
    setDirection(new_dir);
    return true;
}


ZombiePedestrian::ZombiePedestrian(double x, double y, StudentWorld* sw)
: Pedestrian(IID_ZOMBIE_PED, x, y, 3.0, sw), m_ticksGrunt(0)
{
    setHP(2);
}

ZombiePedestrian::~ZombiePedestrian() {}

void ZombiePedestrian::doSomething()
{
    if(isDead())
        return;
    GhostRacer* racer = getWorld()->getGhostRacer();
    if(isOverlap(racer))
    {
        racer->changeHP(-5);
        getWorld()->playSound(soundWhenHurt());
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

bool ZombiePedestrian::beSprayedIfAppropriate()
{
    changeHP(-1);
    if(getHP() <= 0)
        setDead();
    return true;
}

ZombieCab::ZombieCab(double x, double y, StudentWorld* sw)
: Agent(IID_ZOMBIE_CAB, x, y, 90, 4.0, sw), m_plan(0), m_hasDamaged(false)
{
    setVerticalSpeed(0);
    setHorizSpeed(0);
    setHP(3);
}

ZombieCab::~ZombieCab() {}

void ZombieCab::doSomething()
{
    if(isDead())
        return;
    GhostRacer* racer = getWorld()->getGhostRacer();
    if(isOverlap(racer))
    {
        if(!m_hasDamaged)   //step 2.b
        {
            getWorld()->playSound(SOUND_VEHICLE_CRASH);
            racer->changeHP(-20);
            if(getX() <= racer->getX()) //step 2.e
            {
                setHorizSpeed(-5);
                int new_dir = 120 + randInt(0, 19);
                setDirection(new_dir);
            }
            if(getX() > racer->getX())  //step 2.f
            {
                setHorizSpeed(-5);
                int new_dir = 60 - randInt(0, 19);
                setDirection(new_dir);
            }
            m_hasDamaged = true;    //2.g
        }
    }
    else
    {
        movement();    //step 3
        if(isDead())
            return;
        
    }
    /*if(getVerticalSpeed() > racer->getVerticalSpeed() && collision-worthy) //step 4
    {
        
    }*/
    
}

//BORDERLINE CLASS
BorderLine::BorderLine(int imageID, double startX, double startY, StudentWorld* world)
: Actor(imageID, startX, startY, 0, 2.0, 2, world)
{
    setVerticalSpeed(-4);
    setHorizSpeed(0);
}

BorderLine::~BorderLine()
{}

void BorderLine::doSomething()
{
    /*GhostRacer* racer = getWorld()->getGhostRacer();
    int v_speed = getVerticalSpeed() - racer->getVerticalSpeed();
    double new_y = getY() + v_speed;
    double new_x = getX();
    moveTo(new_x,new_y);
    if(getY() < 0 || getY() > VIEW_HEIGHT)
        setDead();*/
    movement();
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
    if(isDead())
        return;
    if(getWorld()->sprayFirstAppropriateActor(this) == true)
    {
        setDead();
        return;
    }
    else
    {
        moveForward(SPRITE_HEIGHT);
        m_pixelsMoved += SPRITE_HEIGHT;
        if(isOffScreen() || m_pixelsMoved >= 160)
        {
            setDead();
            m_pixelsMoved = 0;
        }
    }
}

/*void Spray::hasOverlapped(Actor* other)
{
    if(isOverlap(other) && other->beSprayedIfAppropriate())
    {
        if(other->isHuman())
            //REVERSE DIRECTION
        else
            other->changeHP(-1);
         setDead();
         return;
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
}*/


GhostRacerActivatedObject::GhostRacerActivatedObject(int imageID, double x, double y, int dir, double size, StudentWorld* world)
: Actor(imageID, x, y, dir, size, 2, world)
{
    setVerticalSpeed(-4);
    if(!isProjectile())
        setHorizSpeed(0);
}

GhostRacerActivatedObject::~GhostRacerActivatedObject() {}

void GhostRacerActivatedObject::doSomething()
{
    movement();
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
    /*double vert_speed = getVerticalSpeed() - getWorld()->getGhostRacer()->getVerticalSpeed();
    double new_y = getY() + vert_speed;
    double new_x = getX();
    moveTo(new_x,new_y);
    if( isOffScreen() )
    {
        setDead();
        return;
     }*/movement();
    if(isOverlap(getWorld()->getGhostRacer()))
    {
        getWorld()->playSound(getSound());
        doActivity(getWorld()->getGhostRacer());
        setDead();
    }
    setDirection(getDirection() + 10);
}
