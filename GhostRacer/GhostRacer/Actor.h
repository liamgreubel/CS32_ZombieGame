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
    //virtual bool beSprayedIfAppropriate();

      // Does this object affect zombie cab placement and speed?
    virtual bool isCollisionAvoidanceWorthy() const {return false;}

      // Adjust the x coordinate by dx to move to a position with a y coordinate
      // determined by this actor's vertical speed relative to GhostRacser's
      // vertical speed.  Return true if the new position is within the view;
      // otherwise, return false, with the actor dead.
    //virtual bool moveRelativeToGhostRacerVerticalSpeed(double dx);
    
    bool isOverlap(Actor* other);
    
private:
    StudentWorld* m_world;
    bool m_isDead;
    double m_speed;
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
    //virtual bool isCollisionAvoidanceWorthy() const;
    virtual ~Agent();

    bool isCollisionAvoidanceWorthy() const {return true;}
      // Get hit points.
    int getHP() const {return m_hp;}

      // Increase hit points by hp.
    void changeHP(int hp) {m_hp += hp;}

      // Do what the spec says happens when hp units of damage is inflicted.
      // Return true if this agent dies as a result, otherwise false.
//    virtual bool takeDamageAndPossiblyDie(int hp);

      // What sound should play when this agent is damaged but does not die?
//    virtual int soundWhenHurt();

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

      // Increase the number of holy water projectiles the object has.
    void increaseSprays(int amt) {m_waterSprays += amt;}

      // Spin as a result of hitting an oil slick.
    //void spin();
    
private:
    int m_waterSprays;
    bool m_waterActive;
};
/*
class Pedestrian : public Agent
{
public:
    Pedestrian(StudentWorld* sw, int imageID, double x, double y, double size);
    virtual int soundWhenHurt() const;
    virtual int soundWhenDie() const {return SOUND_PED_DIE;}
    virtual ~Pedestrian();

      // Get the pedestrian's horizontal speed
    //int getHorizSpeed() const;

      // Set the pedestrian's horizontal speed
    //void setHorizSpeed(int s);

      // Move the pedestrian.  If the pedestrian doesn't go off screen and
      // should pick a new movement plan, pick a new plan.
    //void moveAndPossiblyPickPlan();
};

class HumanPedestrian : public Pedestrian
{
public:
    HumanPedestrian(StudentWorld* sw, double x, double y);
    virtual void doSomething();
    virtual bool beSprayedIfAppropriate();
    virtual bool takeDamageAndPossiblyDie(int hp);
    virtual ~HumanPedestrian();
};

class ZombiePedestrian : public Pedestrian
{
public:
    ZombiePedestrian(StudentWorld* sw, double x, double y);
    virtual void doSomething();
    virtual bool beSprayedIfAppropriate();
    virtual ~ZombiePedestrian();
};

class ZombieCab : public Agent
{
public:
    ZombieCab(StudentWorld* sw, double x, double y);
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
    virtual ~Spray();
};

class GhostRacerActivatedObject : public Actor
{
public:
    GhostRacerActivatedObject(int imageID, double x, double y, double size, int dir, StudentWorld* sw);
    virtual bool beSprayedIfAppropriate();

      // Do the object's special activity (increase health, spin Ghostracer, etc.)
    virtual void doActivity(GhostRacer* gr) = 0;

      // Return the object's increase to the score when activated.
    virtual int getScoreIncrease() const = 0;

      // Return the sound to be played when the object is activated.
    virtual int getSound() const;

      // Return whether the object dies after activation.
    virtual bool selfDestructs() const = 0;

      // Return whether the object is affected by a holy water projectile.
    virtual bool isSprayable() const = 0;
    
    virtual ~GhostRacerActivatedObject();
};

class OilSlick : public GhostRacerActivatedObject
{
public:
    OilSlick(double x, double y, StudentWorld* sw);
    virtual void doSomething();
    virtual void doActivity(GhostRacer* gr);
    virtual int getScoreIncrease() const;
    virtual int getSound() const;
    virtual bool selfDestructs() const;
    virtual bool isSprayable() const;
    virtual ~OilSlick();
};

class HealingGoodie : public GhostRacerActivatedObject
{
public:
    HealingGoodie(double x, double y, StudentWorld* sw);
    virtual void doSomething();
    virtual void doActivity(GhostRacer* gr);
    virtual int getScoreIncrease() const;
    virtual bool selfDestructs() const;
    virtual bool isSprayable() const;
    virtual ~HealingGoodie();
};

class HolyWaterGoodie : public GhostRacerActivatedObject
{
public:
    HolyWaterGoodie(double x, double y, StudentWorld* sw);
    virtual void doSomething();
    virtual void doActivity(GhostRacer* gr);
    virtual int getScoreIncrease() const;
    virtual bool selfDestructs() const;
    virtual bool isSprayable() const;
    virtual ~HolyWaterGoodie();
};

class SoulGoodie : public GhostRacerActivatedObject
{
public:
    SoulGoodie(double x, double y, StudentWorld* sw);
    virtual void doSomething();
    virtual void doActivity(GhostRacer* gr);
    virtual int getScoreIncrease() const;
    virtual int getSound() const;
    virtual bool selfDestructs() const;
    virtual bool isSprayable() const;
    virtual ~SoulGoodie();
};
*/
#endif // ACTOR_INCLUDED

/*
class StudentWorld;

class Actor : public GraphObject
{
public:
    Actor(int imageID, int startX, int startY, int dir, double sz, int depth, StudentWorld* world);
    virtual void doSomething() = 0;
    int getHP() {return m_hp;}
    bool isAlive() {return m_hp > 0;}
    bool getAlive() {return m_alive;}
    void setDead() {m_alive = false;}
    StudentWorld* getWorld() {return m_world;}
    virtual ~Actor();
    void setCollision(bool col) {m_isCollision = col;}
    bool getCollision() {return m_isCollision;}
    bool isOverlap(Actor* other);
    virtual int getSpeed() {return m_speed;}
private:
    bool m_isCollision;
    bool m_isActive;
    int m_speed;
    int m_hp;
    bool m_alive;
    StudentWorld* m_world;
    //Actor* other;
};

class hasHP : public Actor
{
public:
    hasHP(int imageID, int startX, int startY, int dir, double sz, int depth, StudentWorld* world);
    virtual ~hasHP();
    int getHP() {return m_hp;}
    void setHP(int hpToSet) {m_hp = hpToSet;}
    int getSpeed() {return m_speed;}
    void setSpeed(int newSpeed) {m_speed = newSpeed;}
private:
    int m_hp;
    int m_speed;
};

class noHP : public Actor
{
public:
    noHP(int imageID, int startX, int startY, int dir, double sz, int depth, StudentWorld* world);
    virtual ~noHP();
private:
};



class GhostRacer : public hasHP
{
public:
    GhostRacer(StudentWorld* world);
    virtual void doSomething();
    virtual ~GhostRacer();
    int getSpeed() {return m_speed;}
private:
    int m_speed;
    int m_holyWater;
    int m_hp;
    bool avoidCollision;
    StudentWorld* m_world;
    GhostRacer* m_racer;
};

class BorderLine : public noHP
{
public:
    BorderLine(int imageID, int startX, int startY, int dir, double sz, int depth, StudentWorld* world, GhostRacer* m_racer);
    virtual ~BorderLine();
    virtual void doSomething();
    GhostRacer* getRacer() {return m_racer;}
    bool hp() {return m_hp;}
private:
    int m_speed;
    int m_racerSpeed;
    int x,y,lastY;
    StudentWorld* m_world;
    GhostRacer* m_racer;
    int m_hp;
};

class Yellow : public BorderLine
{
public:
    Yellow(int x, int y, StudentWorld* world, GhostRacer* racer);
    virtual ~Yellow();
private:
    int m_speed;
    int m_hp;
    StudentWorld* m_world;
    GhostRacer* racer;
};

class White : public BorderLine
{
public:
    White(int x, int y, StudentWorld* world, GhostRacer* racer);
    virtual ~White();
private:
    int m_speed;
    int m_hp;
    StudentWorld* m_world;
    GhostRacer* racer;
};

#endif // ACTOR_H_

*/
