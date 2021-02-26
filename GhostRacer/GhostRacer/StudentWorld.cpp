#include "StudentWorld.h"
#include "GameConstants.h"
#include "GraphObject.h"
#include "Actor.h"
#include <string>
using namespace std;

const int LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH/2;
const int RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH/2;
const int N = VIEW_HEIGHT / SPRITE_HEIGHT;
const int M = VIEW_HEIGHT / (4 * SPRITE_HEIGHT);
const int left_middle = LEFT_EDGE + (ROAD_WIDTH / 3);
const int right_middle = RIGHT_EDGE - (ROAD_WIDTH / 3);

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
    GhostRacer* liam = new GhostRacer(128, 32, this);
    m_racer = liam;
    m_vector.push_back(liam);

    for(int i = 0; i < N; i++)
    {
        Yellow* leftLine = new Yellow(LEFT_EDGE,i*SPRITE_HEIGHT,this);
        m_vector.push_back(leftLine);
        Yellow* rightLine = new Yellow(RIGHT_EDGE,i*SPRITE_HEIGHT,this);
        m_vector.push_back(rightLine);
    }
    for(int j = 0; j < M; j++)
    {
        White* left = new White(left_middle,4*j*SPRITE_HEIGHT,this);
        m_vector.push_back(left);
        White* right = new White(right_middle,4*j*SPRITE_HEIGHT,this);
        m_vector.push_back(right);
    }
    y = m_vector.back()->getY();

    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    setGameStatText("Score: 0000  Lvl: 1  Souls2Save: 5  Lives: 3  Health: " + to_string(m_racer->getHP()) + "  Sprays: " + to_string(m_racer->getNumSprays()) + "  Bonus: 4321");
    if(oil() == 0)
    {
        OilSlick* rand_oil = new OilSlick(randInt(LEFT_EDGE,RIGHT_EDGE), randInt(0, VIEW_HEIGHT), this);
        m_vector.push_back(rand_oil);
    }
    for(int i = 0 ; i < m_vector.size(); i++)
    {
        if(!m_vector.at(i)->isDead())
        {
            m_vector.at(i)->doSomething();
            if(m_racer->hasActiveWater())
                setSpray();//TODO: ONLY INITIALIZATION IMPLEMENTED - ADD MOVEMENT
            
        }
    }//end for loop
    remove();
    insert();
    
    //if(m_racer->isDead())

    return GWSTATUS_CONTINUE_GAME;
}

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
        if( (*it)->isDead() )
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

bool StudentWorld::overlaps(const Actor* a1, const Actor* a2) const
{
    double delta_x = abs(a1->getX() - a2->getX());
    double delta_y = abs(a1->getY() - a2->getY());
    double radius_sum = a1->getRadius() + a2->getRadius();
        
    return ( delta_x < (radius_sum * 0.25) && (delta_y < radius_sum * 0.6) );
}

void StudentWorld::insert()
{
    double new_border_y = (double)VIEW_HEIGHT - (double)SPRITE_HEIGHT;
    y = y - 4 - m_racer->getVerticalSpeed();
    double last_y = y;
    double delta_y = new_border_y - last_y;
    if(delta_y >= SPRITE_HEIGHT)
    {
        Yellow* leftLine = new Yellow(LEFT_EDGE,VIEW_HEIGHT,this);
        m_vector.push_back(leftLine);
        Yellow* rightLine = new Yellow(RIGHT_EDGE,VIEW_HEIGHT,this);
        m_vector.push_back(rightLine);
    }
    if(delta_y >= (4*SPRITE_HEIGHT))
    {
        White* leftLine = new White(left_middle,new_border_y,this);
        m_vector.push_back(leftLine);
        White* rightLine = new White(right_middle,new_border_y,this);
        m_vector.push_back(rightLine);
        
        y = (m_vector.back())->getY();
    }
}

void StudentWorld::setSpray()
{
    if(m_racer->hasActiveWater())
    {
        int direction = m_racer->getDirection();
        double spray_x, spray_y;
        if(direction > 90)
        {
            spray_x = (m_racer->getX() + m_racer->getRadius()) * cos( (m_racer->getDirection() * M_PI / 180) );
            spray_y = (m_racer->getY() + m_racer->getRadius()) * sin( (m_racer->getDirection() * M_PI / 180) );
        }
        if(direction < 90)
        {
            
            spray_x = m_racer->getX() + m_racer->getRadius() * cos( ((m_racer->getDirection()) * M_PI / 180) );
            spray_y = m_racer->getY() + m_racer->getRadius() * sin( (m_racer->getDirection() * M_PI / 180) );
        }
        else
        {
            spray_x = m_racer->getX();
            spray_y =m_racer->getY() + m_racer->getRadius() + SPRITE_HEIGHT;
        }
        Spray* new_spray = new Spray(spray_x,spray_y,direction,this);
        m_vector.push_back(new_spray);
    }
    m_racer->setWater(false);
}
