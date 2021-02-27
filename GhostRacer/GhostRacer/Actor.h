#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

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
    double getVerticalSpeed() const {return m_speed;}

      // Set this actor's vertical speed.
    void setVerticalSpeed(double speed) {m_speed = speed;}

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
    
    bool isOffScreen() {return (getX() < 0 || getX() > VIEW_WIDTH) || (getY() < 0 || getY() > VIEW_HEIGHT);}
    
    bool isOverlap(Actor* racer);
    bool isOverlap(Actor* a, Actor* b);
    
    int getScore() {return m_score;}
    
    void setScore(int s) {m_score = s;}
    
    void changeScore(int n) {m_score += n;}
    
    virtual bool isHuman() const {return false;}

    
    
private:
    StudentWorld* m_world;
    bool m_isDead;
    double m_speed;
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
    Agent(int imageID, double x, double y, int dir, double sz, int hp, StudentWorld* sw);

    virtual ~Agent();

    bool isCollisionAvoidanceWorthy() const {return true;}
      // Get hit points.
    int getHP() const {return m_hp;}

      // Increase hit points by hp.
    void changeHP(int hp) {m_hp += hp;}
    
    void setHP(int hp) {m_hp = hp;}

      // Do what the spec says happens when hp units of damage is inflicted.
      // Return true if this agent dies as a result, otherwise false.
//    virtual bool takeDamageAndPossiblyDie(int hp);

      // What sound should play when this agent is damaged but does not die?
//    virtual int soundWhenHurt();

      // What sound should play when this agent is damaged and dies?
    virtual int soundWhenDie() const = 0;
    
    virtual bool beSprayedIfAppropriate() {return true;}
    
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
    
    int getSouls() {return m_souls;}
    
    void incSouls() {m_souls++;}
    
    void setSouls(int n) {m_souls = n;}
    
    void setWater(bool val) {m_waterActive = val; }
    
    bool hasActiveWater() {return m_waterActive;}
    
    int getLife() {return m_lives;}
    void setLife(int val) {m_lives = val;}
    
    bool lostLife() {return m_lostLife;}
    void setState(bool val)  {m_lostLife = val;}
private:
    int m_waterSprays;
    int m_souls;
    bool m_waterActive;
    int m_lives;
    bool m_lostLife;
};

class Pedestrian : public Agent
{
public:
    Pedestrian(int imageID, double x, double y, double size, StudentWorld* sw);
    virtual int soundWhenHurt() const {return SOUND_PED_HURT;}
    virtual int soundWhenDie() const {return SOUND_PED_DIE;}
    virtual ~Pedestrian();

      // Get the pedestrian's horizontal speed
    int getHorizSpeed() const {return m_hSpeed;}

      // Set the pedestrian's horizontal speed
    void setHorizSpeed(int s) {m_hSpeed = s;}

      // Move the pedestrian.  If the pedestrian doesn't go off screen and
      // should pick a new movement plan, pick a new plan.
    void moveAndPossiblyPickPlan();
    void incPlan(int n) {m_plan += n;}
    void setPlan(int n) {m_plan = n;}
    int  getPlan() {return m_plan;}
private:
    int m_hSpeed;
    int m_plan;
};

class HumanPedestrian : public Pedestrian
{
public:
    HumanPedestrian(double x, double y, StudentWorld* sw);
    virtual void doSomething();
    virtual bool beSprayedIfAppropriate();
    //virtual bool takeDamageAndPossiblyDie(int hp);
    virtual ~HumanPedestrian();
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
    virtual bool beSprayedIfAppropriate();
    virtual ~ZombieCab();
    virtual int soundWhenDie() const {return SOUND_VEHICLE_DIE;}
};

class Spray : public Actor
{
public:
    Spray(double x, double y, int dir, StudentWorld* sw);
    virtual void doSomething();
    void hasOverlapped(/*Actor* other*/);
    virtual ~Spray();
private:
    int m_pixelsMoved;
};

class GhostRacerActivatedObject : public Actor
{
public:
    GhostRacerActivatedObject(int imageID, double x, double y, int dir, double size, StudentWorld* sw);
    virtual void doSomething();
    //virtual bool beSprayedIfAppropriate();
    
      // Do the object's special activity (increase health, spin Ghostracer, etc.)
    virtual void doActivity(GhostRacer* gr) = 0;

      // Return the object's increase to the score when activated.
    //virtual int getScoreIncrease() const = 0;

      // Return the sound to be played when the object is activated.
    virtual int getSound() const {return SOUND_GOT_GOODIE;}

      // Return whether the object dies after activation.
    //virtual bool selfDestructs() const = 0;

      // Return whether the object is affected by a holy water projectile.
    //virtual bool isSprayable() const = 0;
    
    virtual ~GhostRacerActivatedObject();
};

class OilSlick : public GhostRacerActivatedObject
{
public:
    OilSlick(double x, double y, StudentWorld* sw);
    virtual void doSomething();
    virtual void doActivity(GhostRacer* gr);
    //virtual int getScoreIncrease() const;
    virtual int getSound() const {return SOUND_OIL_SLICK;}
    //virtual bool selfDestructs() const;
    //virtual bool isSprayable() const;
    virtual ~OilSlick();
};

class HealingGoodie : public GhostRacerActivatedObject
{
public:
    HealingGoodie(double x, double y, StudentWorld* sw);
    //virtual void doSomething();
    virtual void doActivity(GhostRacer* gr);
    //virtual int getScoreIncrease() const {return 250;}
    //virtual bool selfDestructs() const;
    //virtual bool isSprayable() const;
    virtual ~HealingGoodie();
};

class HolyWaterGoodie : public GhostRacerActivatedObject
{
public:
    HolyWaterGoodie(double x, double y, StudentWorld* sw);
    //virtual void doSomething();
    virtual void doActivity(GhostRacer* gr);
    //virtual int getScoreIncrease() const {return 50;}
    //virtual bool selfDestructs() const;
    //virtual bool isSprayable() const;
    virtual ~HolyWaterGoodie();
};

class SoulGoodie : public GhostRacerActivatedObject
{
public:
    SoulGoodie(double x, double y, StudentWorld* sw);
    virtual void doSomething();
    virtual void doActivity(GhostRacer* gr);
    //virtual int getScoreIncrease() const {return 100;}
    virtual int getSound() const {return SOUND_GOT_SOUL;}
    //virtual bool selfDestructs() const;
    //virtual bool isSprayable() const;
    virtual ~SoulGoodie();
};

#endif // ACTOR_INCLUDED
