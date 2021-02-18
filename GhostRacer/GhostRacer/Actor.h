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
    virtual bool isAlive() = 0;
    virtual int getSpeed() {return m_speed;}
    StudentWorld* getWorld() {return m_world;}
    //virtual StudentWorld* getWorld() {return m_world;}
    virtual ~Actor();
private:
    bool m_isActive;
    int m_speed;
    StudentWorld* m_world;
};

class GhostRacer : public Actor
{
public:
    GhostRacer(StudentWorld* world);
    virtual void doSomething();
    virtual bool isAlive();
    virtual ~GhostRacer();
    //StudentWorld* getWorld() {return world;}
    GhostRacer* getRacer();
private:
    int m_speed;
    int m_holyWater;
    int m_hp;
    bool avoidCollision;
    //StudentWorld* m_world;
    GhostRacer* m_racer;
};

class BorderLine : public Actor
{
public:
    BorderLine(int imageID, int startX, int startY, int dir, double sz, int depth, StudentWorld* world, GhostRacer* m_racer);
    virtual ~BorderLine();
    virtual void doSomething();
    virtual bool isAlive() {return false;}
    virtual GhostRacer* getRacer() {return m_racer;}
    virtual StudentWorld* getWorld() {return m_world;}
private:
    int m_speed;
    bool m_Alive;
    StudentWorld* m_world;
    GhostRacer* m_racer;
};

class Yellow : public BorderLine
{
public:
    Yellow(int x, int y, StudentWorld* world, GhostRacer* racer);
    virtual ~Yellow();
    virtual void doSomething();
private:
    int m_speed;
    StudentWorld* m_world;
    GhostRacer* racer;
};

#endif // ACTOR_H_
