#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Actor;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    StudentWorld* getStudentWorld();
private:
    std::vector<Actor*> m_vector;
    StudentWorld* m_this;
};

#endif // STUDENTWORLD_H_
