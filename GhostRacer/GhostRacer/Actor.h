#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"

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


//*/
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
