#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include <cmath>


//class for pure virtual Actor class inheriting from GraphObject class
Actor::Actor(int imageID, double startX, double startY, int dir, double sz, int depth, StudentWorld* world)
: GraphObject(imageID, startX, startY, dir, sz, depth), m_world(world), m_isDead(false), m_vertSpeed(0), m_horizSpeed(0), m_score(0)
{}


Actor::~Actor()
{}

bool Actor::isOverlap(Actor* racer) //checks if an actor overlaps with another actor
{
    double delta_x = abs(getX() - racer->getX());
    double delta_y = abs(getY() - racer->getY());
    double radius_sum = getRadius() + racer->getRadius();
            
    return ( delta_x < (radius_sum * 0.25) && (delta_y < radius_sum * 0.6) );
}

void Actor::movement()  //generic movement pattern used my most Actors
{
    GhostRacer* racer =getWorld()->getGhostRacer();
    int vert_speed = getVerticalSpeed() - racer->getVerticalSpeed();
    int horiz_speed = getHorizSpeed();
    double new_y = getY() + vert_speed;
    double new_x = getX() + horiz_speed;
    moveTo(new_x, new_y);
    if(isOffScreen())   //call to another generic function that checks if an Actor is off screen, and sets it dead if it is
        setDead();
}

int Actor::checkLane(Actor* a)  //returns lane with corresponding actors. if no lane nor are collisionAvoidance, then returns 0. Coudln't finish in time
{
    if(a->isCollisionAvoidanceWorthy())
    {
        return getWorld()->hasCollInLane(a);
    }
    return -1;
}

int Actor::getLane()//determine which lane an actor is in. Not fully implemented yet
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


//pure virtual class inheriting directly from Actor class
Agent::Agent(int imageID, double startX, double startY, int dir, double sz, StudentWorld* world)
: Actor(imageID, startX, startY, dir, sz, 0, world)
{}

Agent::~Agent() {}

//class inherits directly from Agent class with starting position parameters and a pointer to the world
GhostRacer::GhostRacer(double startX, double startY, StudentWorld* world)
: Agent(IID_GHOST_RACER,128,32,90,4.0,world), m_waterSprays(10), m_souls(0), m_lives(3), m_shotWater(false)
{
    setVerticalSpeed(0);    //default speed, HP for the GhostRacer
    setHorizSpeed(0);
    setHP(100);
}

GhostRacer::~GhostRacer() {}

//during each tick:
void GhostRacer::doSomething()
{
    int ch;
    int direction = getDirection();
    double max_shift_per_tick = 4.0;
    //if not alive
    if(getHP() <= 0)
        return;
    //if swerving off the road to the left, bounce off and take damage
    if(getX() <= ROAD_CENTER - ROAD_WIDTH/2) //left boundary
    {
        if(direction > 90)  //if facing left
            changeHP(-10);
        direction = 82;
        setDirection(direction);
        getWorld()->playSound(SOUND_VEHICLE_CRASH);
    }
    //if swerving off the road to the right, bounce off and take damagee
    if(getX() >= ROAD_CENTER + ROAD_WIDTH/2)   //right boundary
    {
        if(direction < 90)  //if facing right
            changeHP(-10);
        direction = 98;
        setDirection(direction);
        getWorld()->playSound(SOUND_VEHICLE_CRASH);
    }
    //get input from user
    if(getWorld()->getKey(ch))
    {
        switch(ch)
        {//if space bar is pressed
            case KEY_PRESS_SPACE:
                //if ghostRacer has sprays left
                if(getNumSprays() > 0)
                {
                    int dir = getDirection();
                    double delta_x, delta_y;
                    //point spray in direction that GhostRacer is already facing
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
                    //add new spray object to the world
                    Spray* spray = new Spray(delta_x+getX(),delta_y+getY(),dir,getWorld());
                    getWorld()->addActor(spray);
                    getWorld()->playSound(SOUND_PLAYER_SPRAY);
                    //decrement spray
                    increaseSprays(-1);
                }
                break;
            case KEY_PRESS_LEFT:
                //turn to the left
                if(direction < 114)
                    direction += 8;
                break;
            case KEY_PRESS_RIGHT:
                //turn to the right
                if(direction > 66)
                    direction -= 8;
                break;
            case KEY_PRESS_UP:
                //increase speed up to value of 5
                if(getVerticalSpeed() < 5)
                    setVerticalSpeed(getVerticalSpeed() + 1);
                break;
            case KEY_PRESS_DOWN:
                //decrease speed down to value of -1
                if(getVerticalSpeed() > -1)
                    setVerticalSpeed(getVerticalSpeed() - 1);
                break;
        }
    }
    //update direction and move
    setDirection(direction);
    double delta_x = cos( (direction) * M_PI / 180 ) * max_shift_per_tick;
    double cur_x = getX();
    double cur_y = getY();
    moveTo(cur_x + delta_x, cur_y);
}

//randomly spin in either the clockwisee(CW) or counter-clockwise (CCW) direction
void GhostRacer::spin()
{
    int randDir;
    
    randDir = randInt(5,20);
    //clockwise spin
    int CW = getDirection() - randDir;
    //counter-clockwise spin
    int CCW = getDirection() + randDir;
    
    //set limit on how far GhostRacer can spin
    if(CW < 60)
        setDirection(CCW);
    else if(CCW > 120)
        setDirection(CW);
    else
        setDirection(CW);
}

//class directly inheriting from Agenet class
Pedestrian::Pedestrian(int imageID, double x, double y, double size, StudentWorld* world)
: Agent(imageID, x, y, 0, size, world), m_hSpeed(0), m_plan(0)
{
    //sets default hp and speeds for both human PEDs and zombie PEDs
    setVerticalSpeed(-4);
    setHorizSpeed(0);
    setHP(2);
}

Pedestrian::~Pedestrian() {}

//function dictates how PEDs randomly move
void Pedestrian::moveAndPossiblyPickPlan()
{
    //update position and move, provided on screen
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
    //assuming the pedestrian is a human, update plan specifically for a human PED
    if(isHuman())
    {
        incPlan(-1);
        if(getPlan() > 0)
            return;
    }
    else    //otherwise, update plan for the zombie PED
    {
        if(getPlan()  > 0)
        {
            incPlan(-1);
            return;
        }
    }
    //horiz_speed cannot be 0
    while(getHorizSpeed() == 0)
    {
        setHorizSpeed(randInt(-3,3));
    }
    //update plan and set direction
    setPlan(randInt(4,32));
    if(getHorizSpeed() < 0)
        setDirection(180);
    else
        setDirection(0);
}

//class directly inheriting from Pedestrian class
HumanPedestrian::HumanPedestrian(double x, double y, StudentWorld* sw)
: Pedestrian(IID_HUMAN_PED, x, y, 2.0, sw)
{
}

HumanPedestrian::~HumanPedestrian() {}

//during each tick:
void HumanPedestrian::doSomething()
{
    if(isDead())
        return;
    GhostRacer* racer = getWorld()->getGhostRacer();
    if(isOverlap(racer))
    {
        //instantly kill the user if they run into a human PED
        racer->setHP(0);
        return;
    }
    //function call inherited from Pedestrian class
    moveAndPossiblyPickPlan();
    incPlan(-1);
    if(getPlan() > 0)
        return;
    else
    {
        //changing horizontal speed randomly
        int new_h = randInt(-3,3);
        if(new_h != 0)
        {
            setHorizSpeed(new_h);
        }
        //updates plan to random number
        setPlan(randInt(4,32));
        //flips horizontal direction and sets direction
        if(getHorizSpeed() < 0)
            setDirection(180);
        else
            setDirection(0);
    }
}

//function doesn't work yet, but is intended to flip the direction of
//a human PED when sprayed with water, returning true for successful change
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

//class directly inheriting from Pedestrian class
ZombiePedestrian::ZombiePedestrian(double x, double y, StudentWorld* sw)
: Pedestrian(IID_ZOMBIE_PED, x, y, 3.0, sw), m_ticksGrunt(0)
{
    setHP(2);
}

ZombiePedestrian::~ZombiePedestrian() {}

//during each tick:
void ZombiePedestrian::doSomething()
{
    if(isDead())
        return;
    GhostRacer* racer = getWorld()->getGhostRacer();
    //if a zombie PED overlaps with the user
    if(isOverlap(racer))
    {
        //reduce user health by 5
        racer->changeHP(-5);
        getWorld()->playSound(soundWhenHurt());
        //zombie PED loses 2 hp (equivalent to setting dead, since they have 2 hp)
        setDead();
        //increase score for killing zombie PED
        racer->setScore(150);
        return;
    }
    //if the zombie PED is close to the user (within 30 pixels) and is above
    if(abs(getX() - racer->getX()) <= 30.0 && getY() > racer->getY())
    {
        //face user (downwards on the screen)
        setDirection(270);
        //start moving horizontally towards the user depending on which
        //side of the user the zombie PED is on
        
        //if on left side, move right
        if(getX() < racer->getX())
            setHorizSpeed(1);
        //if on right side, move left
        else if (getX() > racer->getX())
            setHorizSpeed(-1);
        //otherwise, do nothing
        else
            setHorizSpeed(0);
        //update ticks until next grunt, and grunt if applicable
        m_ticksGrunt--;
        if(m_ticksGrunt <= 0)
        {
            getWorld()->playSound(SOUND_ZOMBIE_ATTACK);
            m_ticksGrunt = 20;
        }
    }
    //function call inherited from Pedestrian class
    moveAndPossiblyPickPlan();
}

//when fully implemented, will decrement the Zombie Ped's health
//and set dead if applicable
bool ZombiePedestrian::beSprayedIfAppropriate()
{
    changeHP(-1);
    if(getHP() <= 0)
        setDead();
    //increase score for killing zombie PED
    getWorld()->getGhostRacer()->setScore(150);
    return true;
}

//class directly inherited from Agent class - NOT FINISHED
ZombieCab::ZombieCab(double x, double y, StudentWorld* sw)
: Agent(IID_ZOMBIE_CAB, x, y, 90, 4.0, sw), m_plan(0), m_hasDamaged(false)
{
    setVerticalSpeed(0);
    setHorizSpeed(0);
    setHP(3);   //default HP set to 3
}

ZombieCab::~ZombieCab() {}

//for every tick (NOT FINISHED):
void ZombieCab::doSomething()
{
    if(isDead())
        return;
    GhostRacer* racer = getWorld()->getGhostRacer();
    //if the user runs into the cab
    if(isOverlap(racer))
    {
        //if cab has already damaged the user, skip to step 3
        if(!m_hasDamaged)   //step 2.b
        {
            //adjust user health
            getWorld()->playSound(SOUND_VEHICLE_CRASH);
            racer->changeHP(-20);
            //if on the left side of the user, set new horiz speed
            //and a new random direction towards the right
            if(getX() <= racer->getX()) //step 2.e
            {
                setHorizSpeed(-5);
                int new_dir = 120 + randInt(0, 19);
                setDirection(new_dir);
            }
            //if on the left side of the user, set new horiz speed
            //and a new random direction towards the left
            if(getX() > racer->getX())  //step 2.f
            {
                setHorizSpeed(-5);
                int new_dir = 60 - randInt(0, 19);
                setDirection(new_dir);
            }
            //updates condition that cab has already damaged user
            m_hasDamaged = true;    //2.g
        }
    }
    //otherwise, perform standard movement pattern inherited from Actor class
    else
    {
        movement();    //step 3
        if(isDead())
            return;
        
    }
    
}

//class inheriting directly from Actor class
BorderLine::BorderLine(int imageID, double startX, double startY, StudentWorld* world)
: Actor(imageID, startX, startY, 0, 2.0, 2, world)
{
    //set speeds for both white and yellow lines
    setVerticalSpeed(-4);
    setHorizSpeed(0);
}

BorderLine::~BorderLine()
{}

void BorderLine::doSomething()
{
    //perform standard movement pattern inherited from Actor class
    movement();
}


//class directly inheeriting from BorderLine class
Yellow::Yellow(double x, double y, StudentWorld* world)
: BorderLine(IID_YELLOW_BORDER_LINE, x, y,world)
{}

Yellow::~Yellow() {}


//class directly inheeriting from BorderLine class
White::White(double x, double y, StudentWorld* world)
: BorderLine(IID_WHITE_BORDER_LINE, x, y, world)
{}

White::~White() {};


//class directly inheriting from Actor class
Spray::Spray(double x, double y, int dir, StudentWorld* world)
: Actor(IID_HOLY_WATER_PROJECTILE, x, y, dir, 1.0, 1, world), m_pixelsMoved(0)
{}

Spray::~Spray() {}

//for every tick (NOT FINISHED - BUGGED):
void Spray::doSomething()
{
    if(isDead())
        return;
    //check to see if active by calling function from StudentWorld.cpp
    if(getWorld()->sprayFirstAppropriateActor(this) == true)
    {
        //if the spray overlaps with a sprayable actor,
        //and if so, remove from game by setting dead
        setDead();
        
        return;
    }
    //otherwise, continue movement pattern in same direction until
    //a sprayable actor is reached or it travels 160 pixels
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

//class for pure virtual class inheriting directly from Actor class
GhostRacerActivatedObject::GhostRacerActivatedObject(int imageID, double x, double y, int dir, double size, StudentWorld* world)
: Actor(imageID, x, y, dir, size, 2, world)
{
    //sets all inherited speeds to be -4
    setVerticalSpeed(-4);
}

GhostRacerActivatedObject::~GhostRacerActivatedObject() {}

//for every tick:
void GhostRacerActivatedObject::doSomething()
{
    //perform standard movement pattern
    movement();
    //if activated by overlapping, perform specific
    //functionality by calling doActivity()
    if(isOverlap(getWorld()->getGhostRacer()))
    {
        getWorld()->playSound(getSound());
        doActivity(getWorld()->getGhostRacer());
        setDead();
    }
}


//class inheriting directly from GhostRacerActivatedObject class
OilSlick::OilSlick(double x, double y, StudentWorld* world)
: GhostRacerActivatedObject(IID_OIL_SLICK, x, y, 0, randInt(2,5),world)
{}

OilSlick::~OilSlick() {}

//for every tick:
void OilSlick::doSomething()
{
    //perform standard movement pattern for oil slick
    movement();
    
    doActivity(getWorld()->getGhostRacer());
}

//performs spinout of the user
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

//class inheriting directly from GhostRacerActivatedObject class
HealingGoodie::HealingGoodie(double x, double y, StudentWorld* world)
: GhostRacerActivatedObject(IID_HEAL_GOODIE, x, y, 0, 1.0, world)
{
    
}

HealingGoodie::~HealingGoodie() {}


//heals user by 10 hp up to 100 max while increasing the score
void HealingGoodie::doActivity(GhostRacer *racer)
{
    racer->changeHP(10);
    if(racer->getHP() > 100)
        racer->setHP(100);
    racer->setScore(250);
}

//class inherits directly from GhostRacerActivatedObject class
HolyWaterGoodie::HolyWaterGoodie(double x, double y, StudentWorld* world)
: GhostRacerActivatedObject(IID_HOLY_WATER_GOODIE, x, y, 90, 2.0, world)
{
}

HolyWaterGoodie::~HolyWaterGoodie() {}


//when in contact with the user, increase score and # sprays
void HolyWaterGoodie::doActivity(GhostRacer *racer)
{
    racer->increaseSprays(10);
    racer->setScore(50);
}


//class inherits directly from GhostRacerActivatedObject class
SoulGoodie::SoulGoodie(double x, double y, StudentWorld* world)
: GhostRacerActivatedObject(IID_SOUL_GOODIE, x, y, 0, 4.0, world)
{
}

SoulGoodie::~SoulGoodie() {}


//when in contact with the user, increase score and # sprays
void SoulGoodie::doActivity(GhostRacer* racer)
{
    racer->incSouls();
    racer->setScore(100);
}

void SoulGoodie::doSomething()
{
    movement();
    //perform standard movement pattern and rotate every tick
    if(isOverlap(getWorld()->getGhostRacer()))
    {
        getWorld()->playSound(getSound());
        doActivity(getWorld()->getGhostRacer());
        setDead();
    }
    setDirection(getDirection() + 10);
}
