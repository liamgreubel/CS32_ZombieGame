#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <vector>

class GhostRacer;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Actor;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    void remove();
    void insert();
    StudentWorld* getStudentWorld() {return this;}
private:
    std::vector<Actor*> m_vector;
    double y;
    GhostRacer* m_racer;
};

#endif // STUDENTWORLD_H_
