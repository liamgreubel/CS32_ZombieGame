#include "StudentWorld.h"
#include "GameConstants.h"
#include "GraphObject.h"
#include "Actor.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    m_this = this;
}

int StudentWorld::init()
{
    GhostRacer* liam = new GhostRacer(this);
    m_vector.push_back(liam);
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    m_vector.at(0)->doSomething();
    decLives();
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp() //use loop w std::iterator to delete all actors at the end
{
    delete m_vector.at(0);
    m_vector.pop_back();
}

StudentWorld* StudentWorld::getStudentWorld()
{ return m_this; }

