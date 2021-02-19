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
    void remove();
    StudentWorld* getStudentWorld() {return this;}
private:
    std::vector<Actor*> m_vector;
};

#endif // STUDENTWORLD_H_
