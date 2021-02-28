#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

const int LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH/2;
const int RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH/2;
const int left_middle = LEFT_EDGE + (ROAD_WIDTH / 3);
const int right_middle = RIGHT_EDGE - (ROAD_WIDTH / 3);
const int N = VIEW_HEIGHT / SPRITE_HEIGHT;
const int M = VIEW_HEIGHT / (4 * SPRITE_HEIGHT);


class Actor : public GraphObject
{
public:
    Actor(int imageID, double x, double y, int dir, double sz, int depth, StudentWorld* world);
    
    virtual ~Actor();

    virtual void doSomething() = 0;

    bool isDead() const {return m_isDead;}
    
      // Mark this actor as dead.
    void setDead() {m_isDead = true;}

      // Get this actor's world
    StudentWorld* getWorld() const {return m_world;}

      // Get this actor's vertical speed.
    double getVerticalSpeed() const {return m_vertSpeed;}

      // Set this actor's vertical speed.
    void setVerticalSpeed(double speed) {m_vertSpeed = speed;}
    
    //get this actor's horizontal speed
    double getHorizSpeed() const {return m_horizSpeed;}
    
    // Set this actor's horizontal speed.
    void setHorizSpeed(double speed) {m_horizSpeed = speed;}

      // If this actor is affected by holy water projectiles, then inflict that
      // affect on it and return true; otherwise, return false.
    virtual bool beSprayedIfAppropriate() const {return false;}

      // Does this object affect zombie cab placement and speed?
    virtual bool isCollisionAvoidanceWorthy() const {return false;}

      // Adjust the x coordinate by dx to move to a position with a y coordinate
      // determined by this actor's vertical speed relative to GhostRacser's
      // vertical speed.  Return true if the new position is within the view;
      // otherwise, return false, with the actor dead.
    //virtual bool moveRelativeToGhostRacerVerticalSpeed(double dx);
    
    //performs movement pattern in common with most actors
    virtual void movement();
    
    //checks if an actor is off the screen
    bool isOffScreen() {return (getX() < 0 || getX() > VIEW_WIDTH) || (getY() < 0 || getY() > VIEW_HEIGHT);}
    
    //checks if an actor overlaps with another actor
    bool isOverlap(Actor* racer);
    
    //returns score
    int getScore() {return m_score;}
    
    //sets score
    void setScore(int s) {m_score = s;}
    
    //increase/decrease score by a set amount
    void changeScore(int n) {m_score += n;}
    
    //checks if an actor is human. default behavior is false
    virtual bool isHuman() const {return false;}
    
    //checks if an actor has the ability to drive. default behavior is false
    virtual bool canDrive() const {return false;}
    
    //returns lane with corresponding actors. if no lane nor are collisionAvoidance, then returns 0. Coudln't finish in time
    virtual int checkLane(Actor* a);
    
    //not fully implemented, but should check to see which lane a single actor is in
    int getLane();

    
private:
    StudentWorld* m_world;
    bool m_isDead;
    double m_vertSpeed;
    double m_horizSpeed;
    int m_score;
};

class BorderLine : public Actor
{
public:
    BorderLine(int imageID, double x, double y, StudentWorld* world);
    virtual ~BorderLine();
    virtual void doSomething();
};

class Yellow : public BorderLine
{
public:
    Yellow(double startX, double startY, StudentWorld* world);
    virtual ~Yellow();
private:
};

class White : public BorderLine
{
public:
    White(double startX, double startY, StudentWorld* world);
    virtual ~White();
private:
};

class Agent : public Actor
{
public:
    Agent(int imageID, double x, double y, int dir, double sz, StudentWorld* sw);

    virtual ~Agent();

    bool isCollisionAvoidanceWorthy() const {return true;}
      // Get hit points.
    int getHP() const {return m_hp;}

      // Increase/decrease hit points by hp.
    void changeHP(int hp) {m_hp += hp;}
    
    //set hp
    void setHP(int hp) {m_hp = hp;}

      // What sound should play when this agent is damaged and dies?
    virtual int soundWhenDie() const = 0;
    
    
private:
    int m_hp;
    
};

class GhostRacer : public Agent
{
public:
    GhostRacer(double x, double y, StudentWorld* sw);
    virtual void doSomething();
    virtual ~GhostRacer();
    virtual int soundWhenDie() const {return SOUND_PLAYER_DIE;}

      // How many holy water projectiles does the object have?
    int getNumSprays() const {return m_waterSprays;}
    
    void setSprays(int n) {m_waterSprays = n;}

      // Increase the number of holy water projectiles the object has.
    void increaseSprays(int amt) {m_waterSprays += amt;}

      // Spin as a result of hitting an oil slick.
    void spin();
    
    //return number of souls captured
    int getSouls() {return m_souls;}
    
    //increase souls captured by 1
    void incSouls() {m_souls++;}
    
    //set number of souls captured
    void setSouls(int n) {m_souls = n;}
    
    //getter/setter for number of lives remaining
    int getLife() {return m_lives;}
    void setLife(int val) {m_lives = val;}
    
    //getter/setter for whether GhostRacer's lives (used for starting conditions)
    bool lostLife() {return m_lostLife;}
    void setState(bool val)  {m_lostLife = val;}
    
    //set number of Holy Water shots a GhostRacer has
    void setShot(bool val) {m_shotWater = val; }
    
    //get number of remaining Holy Water shots a GhostRacer has
    bool hasShot() {return m_shotWater;}
private:
    int m_waterSprays;
    int m_souls;
    int m_lives;
    bool m_lostLife;
    bool m_shotWater;
};

class Pedestrian : public Agent
{
public:
    Pedestrian(int imageID, double x, double y, double size, StudentWorld* sw);
    virtual int soundWhenHurt() const {return SOUND_PED_HURT;}
    virtual int soundWhenDie() const {return SOUND_PED_DIE;}
    virtual ~Pedestrian();

      // Get the pedestrian's horizontal speed
    double getHorizSpeed() const {return m_hSpeed;}

      // Set the pedestrian's horizontal speed
    void setHorizSpeed(int s) {m_hSpeed = s;}

      // Move the pedestrian.  If the pedestrian doesn't go off screen and
      // should pick a new movement plan, pick a new plan.
    void moveAndPossiblyPickPlan();
    //increment plan variable by n
    void incPlan(int n) {m_plan += n;}
    
    //getter/setter for plan variable used for determining position
    void setPlan(int n) {m_plan = n;}
    int  getPlan() {return m_plan;}
private:
    double m_hSpeed;
    int m_plan;
};

class HumanPedestrian : public Pedestrian
{
public:
    HumanPedestrian(double x, double y, StudentWorld* sw);
    virtual void doSomething();
    virtual bool beSprayedIfAppropriate();
    virtual ~HumanPedestrian();
    //specific for humans
    virtual bool isHuman() const {return true;}
private:
    
};

class ZombiePedestrian : public Pedestrian
{
public:
    ZombiePedestrian(double x, double y, StudentWorld* sw);
    virtual void doSomething();
    virtual bool beSprayedIfAppropriate();
    virtual ~ZombiePedestrian();
private:
    int m_ticksGrunt;
};

class ZombieCab : public Agent
{
public:
    ZombieCab(double x, double y, StudentWorld* sw);
    virtual void doSomething();
    virtual ~ZombieCab();
    virtual int soundWhenDie() const {return SOUND_VEHICLE_DIE;}
    //specific for actors that can drive
    virtual bool canDrive() const {return true;}
private:
    int m_plan;
    bool m_hasDamaged;
};

class Spray : public Actor
{
public:
    Spray(double x, double y, int dir, StudentWorld* sw);
    virtual void doSomething();
    virtual ~Spray();
private:
    int m_pixelsMoved;
};

class GhostRacerActivatedObject : public Actor
{
public:
    GhostRacerActivatedObject(int imageID, double x, double y, int dir, double size, StudentWorld* sw);
    virtual void doSomething();
      // Do the object's special activity (increase health, spin Ghostracer, etc.)
    virtual void doActivity(GhostRacer* gr) = 0;

      // Return the sound to be played when the object is activated.
    virtual int getSound() const {return SOUND_GOT_GOODIE;}
    
    virtual bool isProjectile() {return false;}
    
    virtual ~GhostRacerActivatedObject();
};

class OilSlick : public GhostRacerActivatedObject
{
public:
    OilSlick(double x, double y, StudentWorld* sw);
    virtual void doSomething();
    virtual void doActivity(GhostRacer* gr);
    virtual int getSound() const {return SOUND_OIL_SLICK;}

    virtual ~OilSlick();
};

class HealingGoodie : public GhostRacerActivatedObject
{
public:
    HealingGoodie(double x, double y, StudentWorld* sw);
    virtual void doActivity(GhostRacer* gr);
    virtual ~HealingGoodie();
};

class HolyWaterGoodie : public GhostRacerActivatedObject
{
public:
    HolyWaterGoodie(double x, double y, StudentWorld* sw);
    virtual void doActivity(GhostRacer* gr);
    //only for projectile type objects
    virtual bool isProjectile() {return true;}
    virtual ~HolyWaterGoodie();
};

class SoulGoodie : public GhostRacerActivatedObject
{
public:
    SoulGoodie(double x, double y, StudentWorld* sw);
    virtual void doSomething();
    virtual void doActivity(GhostRacer* gr);
    virtual int getSound() const {return SOUND_GOT_SOUL;}
    virtual ~SoulGoodie();
};

#endif // ACTOR_INCLUDED
