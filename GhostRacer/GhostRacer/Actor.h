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
    StudentWorld* getWorld() {return m_world;}
    virtual ~Actor();
    bool isOverlap(Actor* other);
private:
    bool m_isActive;
    int m_speed;
    int m_hp;
    bool m_alive;
    StudentWorld* m_world;
    //Actor* other;
};

class GhostRacer : public Actor
{
public:
    GhostRacer(StudentWorld* world);
    virtual void doSomething();
    //virtual bool isAlive();
    virtual ~GhostRacer();
    int getSpeed() {return m_speed;}
    //GhostRacer* getRacer();
private:
    int m_speed;
    int m_holyWater;
    int m_hp;
    bool avoidCollision;
    StudentWorld* m_world;
    GhostRacer* m_racer;
};

class BorderLine : public Actor
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
    int x,y;
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
