#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"

class GameWorld;
class StudentWorld;

class Actor : public GraphObject
{
public:
    Actor(int imageID, int startX, int startY, int dir, double sz, int depth = 0);
    virtual void doSomething() = 0;
    virtual bool isAlive() = 0;
    ~Actor();
private:
    bool m_isActive;
};

class GhostRacer : public Actor
{
public:
    GhostRacer(StudentWorld* world);
    virtual void doSomething();
    virtual bool isAlive();
    ~GhostRacer();
    StudentWorld* getWorld();
private:
    int m_speed;
    int m_holyWater;
    int m_hp;
    bool avoidCollision;
    StudentWorld* m_world;

};

#endif // ACTOR_H_
