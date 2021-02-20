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
{}

int StudentWorld::init()
{
    GhostRacer* liam = new GhostRacer(this);
    temp = liam;
    m_vector.push_back(liam);
    int LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH/2;
    int RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH/2;
    int N = VIEW_HEIGHT / SPRITE_HEIGHT;
    int M = VIEW_HEIGHT / (4 * SPRITE_HEIGHT);
    int left_middle = LEFT_EDGE + (ROAD_WIDTH / 3);
    int right_middle = RIGHT_EDGE - (ROAD_WIDTH / 3);
    for(int i = 0; i < N; i++)
    {
        Yellow* leftLine = new Yellow(LEFT_EDGE,VIEW_HEIGHT-(i*SPRITE_HEIGHT),this,liam);
        m_vector.push_back(leftLine);
        Yellow* rightLine = new Yellow(RIGHT_EDGE,VIEW_HEIGHT-(i*SPRITE_HEIGHT),this,liam);
        m_vector.push_back(rightLine);
    }
    for(int j = 0; j < M; j++)
    {
        White* left = new White(left_middle,VIEW_HEIGHT-((10*j)+SPRITE_HEIGHT),this,liam);
        m_vector.push_back(left);
        White* right = new White(right_middle,VIEW_HEIGHT-((10*j)+SPRITE_HEIGHT),this,liam);
        m_vector.push_back(right);
    }
    /*
    Yellow* leftLine = new Yellow(LEFT_EDGE,VIEW_HEIGHT-(1*6),this,liam);
    m_vector.push_back(leftLine);
    Yellow* rightLine = new Yellow(RIGHT_EDGE,VIEW_HEIGHT-(1*6),this,liam);
    m_vector.push_back(rightLine);*/
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    int LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH/2;
    int RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH/2;
    int N = VIEW_HEIGHT / SPRITE_HEIGHT;
    int M = VIEW_HEIGHT / (4 * SPRITE_HEIGHT);
    int left_middle = LEFT_EDGE + (ROAD_WIDTH / 3);
    int right_middle = RIGHT_EDGE - (ROAD_WIDTH / 3);
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    for(int i = 0 ; i < m_vector.size(); i++)
    {
        m_vector.at(i)->doSomething();
        if(m_vector.at(i)->getY() < 0 && m_vector.at(i)->getX() < ROAD_CENTER)
        {
            delete m_vector.at(i);
            m_vector.erase(m_vector.begin()+i);
            Yellow* leftLine = new Yellow(ROAD_CENTER - ROAD_WIDTH/2,VIEW_HEIGHT,this,temp);
            m_vector.push_back(leftLine);
            //m_vector.back()->doSomething();
            White* left = new White(left_middle, VIEW_HEIGHT,this,temp);
            m_vector.push_back(left);
            //m_vector.back()->doSomething();
        }
        if(m_vector.at(i)->getY() < 0 && m_vector.at(i)->getX() > ROAD_CENTER)
        {
            delete m_vector.at(i);
            m_vector.erase(m_vector.begin()+i);
            Yellow* rightLine = new Yellow(ROAD_CENTER + ROAD_WIDTH/2,VIEW_HEIGHT,this,temp);
            m_vector.push_back(rightLine);
            //m_vector.back()->doSomething();
            White* right = new White(right_middle, VIEW_HEIGHT,this,temp);
            m_vector.push_back(right);
            //m_vector.back()->doSomething();
        }
    }

    decLives();
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp() //use loop w std::iterator to delete all actors at the end
{
    for(int i = m_vector.size(); i >= 0; i++)
    {
        delete m_vector.at(i);
        m_vector.pop_back();
    }
    //delete *it;
}
/*
void StudentWorld::remove()
{
    m_vector.
}*/
