#include "StudentWorld.h"
#include "GameConstants.h"
#include "GraphObject.h"
#include "Actor.h"
#include <string>
using namespace std;

const int LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH/2;
const int RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH/2;
const int left_middle = LEFT_EDGE + (ROAD_WIDTH / 3);
const int right_middle = RIGHT_EDGE - (ROAD_WIDTH / 3);
const int N = VIEW_HEIGHT / SPRITE_HEIGHT;
const int M = VIEW_HEIGHT / (4 * SPRITE_HEIGHT);


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
    
    m_numSouls = 2 * getLevel() + 5;
    m_bonus = 5000;
    m_racer->setSouls(0);
    m_racer->setSprays(10);
    if(getLevel() == 1 && getLives() == 3)
    {
        m_score = 0;
        m_tempScore = 0;
    }
    m_score = 0;
    m_score += m_tempScore;
    
    m_racer->setState(false);
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    int lane;
    setGameStatText("Score: " + to_string(m_score)  + "  Lvl: " + to_string(getLevel()) + "  Souls2Save: " + to_string(m_numSouls - m_racer->getSouls()) + "  Lives: " + to_string(getLives()) + "  Health: " + to_string(m_racer->getHP()) + "  Sprays: " + to_string(m_racer->getNumSprays()) + "  Bonus: " + to_string(m_bonus));
    chanceAddActors();
    for(int i = 0 ; i < m_vector.size(); i++)
    {
        if(!m_vector.at(i)->isDead())
        {
            m_vector.at(i)->doSomething();
            
            
            if(m_vector.at(i)->canDrive())
            {
                lane = m_vector.at(i)->getLane();
            }
             
             /*if( (!m_vector.at(i)->isDead()) && (m_vector.at(i)->isCollisionAvoidanceWorthy()) )
             {
                 int lane_no = m_vector.at(i)->getLane();
                 switch(lane_no)
                 {
                     case 1:
                         m_occupiedLanes[0] = true;
                         break;
                     case 2:
                         m_occupiedLanes[1] = true;
                         break;
                     case 3:
                         m_occupiedLanes[2] = true;
                         break;
                     default:
                         break;
                 }
             }*/
        }
    }//end for loop
    /*for(int k = 0; k < m_vector.size(); k++)
    {
        if( (!m_vector.at(k)->isDead()) && (m_vector.at(k)->isCollisionAvoidanceWorthy()) )
        {
            int lane_no = m_vector.at(k)->getLane();
            switch(lane_no)
            {
                case 1:
                    m_occupiedLanes[0] = true;
                    break;
                case 2:
                    m_occupiedLanes[1] = true;
                    break;
                case 3:
                    m_occupiedLanes[2] = true;
                    break;
                default:
                    break;
            }
        }
    }*/
    //update actors on screen
    m_bonus--;  //decrement bonus 1 per tick
    if(m_bonus < 0)
        m_bonus = 0;
    m_score += m_racer->getScore();
    m_racer->setScore(0);
    remove();
    insert();
    //update scoreline
    
    if(m_numSouls == m_racer->getSouls())
    {
        m_score += m_bonus;
        playSound(SOUND_FINISHED_LEVEL);
        return GWSTATUS_FINISHED_LEVEL;
    }
    if(m_racer->getHP() <= 0)
    {
        decLives();
        m_racer->setLife(m_racer->getLife()-1);
        playSound(SOUND_PLAYER_DIE);
        m_tempScore = m_score;
        m_bonus = 5000;
        return GWSTATUS_PLAYER_DIED;
    }

    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp() //use loop w std::iterator to delete all actors at the end
{
    if(m_vector.empty())
        return;
    vector<Actor*>::iterator it = m_vector.end()-1;
    for(; it != m_vector.begin()-1;)
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
        
    return ( (delta_x < (radius_sum * 0.25)) && (delta_y < (radius_sum * 0.6)) );
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

int StudentWorld::chanceOilSlick()
{
    int oil_max = std::max(150 - getLevel() * 10, 40);
    return randInt(0, oil_max-1);
}
int StudentWorld::chanceZPed()
{
    int zped_max = std::max(100 - getLevel() * 10, 20);
    return randInt(0, zped_max-1);
}
int StudentWorld::chanceHPed()
{
    int hped_max = std::max(200 - getLevel() * 10, 30);
    return randInt(0, hped_max-1);
}
int StudentWorld::chanceBottleRefill()
{
    return randInt(0,100 + 10 * getLevel());
}
int StudentWorld::chanceSoul() {return randInt(0,99);}

int StudentWorld::chanceCab()
{
    int cab_max = max(100-getLevel()*10,20);
    return randInt(0,cab_max-1);
}

void StudentWorld::chanceAddActors()
{
    if(chanceOilSlick() == 0)
    {
        OilSlick* oil = new OilSlick(randInt(LEFT_EDGE+SPRITE_WIDTH, RIGHT_EDGE - SPRITE_WIDTH), VIEW_HEIGHT, this);
        m_vector.push_back(oil);
    }
    if(chanceZPed() == 0)
    {
        ZombiePedestrian* zped = new ZombiePedestrian(randInt(0, VIEW_WIDTH), VIEW_HEIGHT, this);
        m_vector.push_back(zped);
    }
    if(chanceHPed() == 0)
    {
        HumanPedestrian* hped = new HumanPedestrian(randInt(0, VIEW_WIDTH), VIEW_HEIGHT, this);
        m_vector.push_back(hped);
    }
    if(chanceBottleRefill() == 0)
    {
        HolyWaterGoodie* water = new HolyWaterGoodie(randInt(LEFT_EDGE+SPRITE_WIDTH, RIGHT_EDGE - SPRITE_WIDTH), VIEW_HEIGHT, this);
        m_vector.push_back(water);
    }
    if(chanceSoul() == 0)
    {
        SoulGoodie* soul = new SoulGoodie(randInt(LEFT_EDGE+SPRITE_WIDTH, RIGHT_EDGE - SPRITE_WIDTH), VIEW_HEIGHT, this);
        m_vector.push_back(soul);
    }
    if(chanceCab() == 0)
    {
        double cur_lane;
        int num = randInt(0,2);
        if(num % 2 == 1)
            cur_lane = ROAD_CENTER + ROAD_WIDTH/3;
        else if(num % 1 == 1)
            cur_lane = ROAD_CENTER - ROAD_WIDTH/3;
        else
            cur_lane = ROAD_CENTER;
        if(cur_lane == m_racer->getLane())
            cur_lane++;
            
        ZombieCab* cab = new ZombieCab(cur_lane, VIEW_HEIGHT - SPRITE_HEIGHT / 2, this);
        m_vector.push_back(cab);
    }
}

bool StudentWorld::sprayFirstAppropriateActor(Actor* a)
{
    vector<Actor*>::iterator it = m_vector.begin();
    for(; it != m_vector.end(); it++)
    {
        if(((*it)->isDead() == false) && a->isOverlap(*it) && (*it)->beSprayedIfAppropriate() == true)
        {
            //increaseScore(*it)
            
            return true;
        }
    }
    return false;
}


void StudentWorld::addActor(Actor* a)
{
    m_vector.push_back(a);
}

bool StudentWorld::hasCollInLane(Actor* a)
{
    const int NO_LANE = 0;  //actors aren't in the same lane on the road
    const int LEFT_LANE = 1;
    const int MIDDLE_LANE = 2;
    const int RIGHT_LANE = 3;
    if(a->getX() > LEFT_EDGE && a->getX() < left_middle)
        return LEFT_LANE;
    else if(a->getX() > left_middle && a->getX() < right_middle)
        return MIDDLE_LANE;
    else if(a->getX() > right_middle && a->getX() < RIGHT_EDGE)
        return RIGHT_LANE;
    else
        return NO_LANE;
}
