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
    m_racer = liam;
    m_vector.push_back(liam);
    int LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH/2;
    int RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH/2;
    int N = VIEW_HEIGHT / SPRITE_HEIGHT;
    int M = VIEW_HEIGHT / (4 * SPRITE_HEIGHT);
    int left_middle = LEFT_EDGE + (ROAD_WIDTH / 3);
    int right_middle = RIGHT_EDGE - (ROAD_WIDTH / 3);
    for(int i = 0; i < N; i++)
    {
        Yellow* leftLine = new Yellow(LEFT_EDGE,i*SPRITE_HEIGHT,this,liam);
        m_vector.push_back(leftLine);
        Yellow* rightLine = new Yellow(RIGHT_EDGE,i*SPRITE_HEIGHT,this,liam);
        m_vector.push_back(rightLine);
    }
    for(int j = 0; j < M; j++)
    {
        White* left = new White(left_middle,4*j*SPRITE_HEIGHT,this,liam);
        m_vector.push_back(left);
        White* right = new White(right_middle,4*j*SPRITE_HEIGHT,this,liam);
        m_vector.push_back(right);
    }
    y = m_vector.back()->getY();

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

    for(int i = 0 ; i < m_vector.size(); i++)
    {
        if(m_vector.at(i)->getAlive())
        {
            m_vector.at(i)->doSomething();

        }
    }//end for loop
    remove();
    insert();

    return GWSTATUS_CONTINUE_GAME;
} //end move()

void StudentWorld::cleanUp() //use loop w std::iterator to delete all actors at the end
{
    vector<Actor*>::iterator it = m_vector.end()-1;
    for(; it != m_vector.begin();)
    {
        delete *it;
        m_vector.pop_back();
        it = m_vector.end()-1;
    }
}

void StudentWorld::remove()
{
    vector<Actor*>::iterator it;
    for(it = m_vector.begin(); it != m_vector.end();)
    {
        if( !((*it)->getAlive()) )
        {
            delete *it;
            it = m_vector.erase(it);
        }
        else
            it++;
    }
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

void StudentWorld::insert()
{
    int LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH/2;
    int RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH/2;
    int N = VIEW_HEIGHT / SPRITE_HEIGHT;
    int M = VIEW_HEIGHT / (4 * SPRITE_HEIGHT);
    int left_middle = LEFT_EDGE + (ROAD_WIDTH / 3);
    int right_middle = RIGHT_EDGE - (ROAD_WIDTH / 3);
    
    
    double new_border_y = (double)VIEW_HEIGHT - (double)SPRITE_HEIGHT;
    y = y - 4 - m_racer->getSpeed();
    double last_y = y;
    double delta_y = new_border_y - last_y;
    if(delta_y >= SPRITE_HEIGHT)
    {
        Yellow* leftLine = new Yellow(LEFT_EDGE,VIEW_HEIGHT/*new_border_y*/,this,m_racer);
        m_vector.push_back(leftLine);
        Yellow* rightLine = new Yellow(RIGHT_EDGE,VIEW_HEIGHT/*new_border_y*/,this,m_racer);
        m_vector.push_back(rightLine);
    }
    if(delta_y >= (4*SPRITE_HEIGHT))
    {

        White* leftLine = new White(left_middle,new_border_y,this,m_racer);
        m_vector.push_back(leftLine);
        White* rightLine = new White(right_middle,new_border_y,this,m_racer);
        m_vector.push_back(rightLine);
        
        y = (m_vector.back())->getY();
    }
    

}
