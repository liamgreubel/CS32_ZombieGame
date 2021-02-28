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

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{}

int StudentWorld::init()
{
    //creation of GhostRacer and BorderLines
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
    //last y coordinate saved to make the user appear to be driving forward
    y = m_vector.back()->getY();
    
    //set number of souls to rescue, level, bonus, etc. for starting conditions
    m_numSouls = 2 * getLevel() + 5;
    m_bonus = 5000;
    m_racer->setSouls(0);
    m_racer->setSprays(10);
    //if the user just started the game start at 0
    if(getLevel() == 1 && getLives() == 3)
    {
        m_score = 0;
        m_tempScore = 0;
    }
    //otherwise, their score from the last life will be added to the running total
    m_score = 0;
    m_score += m_tempScore;
    
    //update that the racer has started the game
    m_racer->setState(false);
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    int lane;
    
    //update game stat text
    setGameStatText("Score: " + to_string(m_score)  + "  Lvl: " + to_string(getLevel()) + "  Souls2Save: " + to_string(m_numSouls - m_racer->getSouls()) + "  Lives: " + to_string(getLives()) + "  Health: " + to_string(m_racer->getHP()) + "  Sprays: " + to_string(m_racer->getNumSprays()) + "  Bonus: " + to_string(m_bonus));
    //add actors randomly by their respective odds
    chanceAddActors();
    //give each actor a chance to do something
    for(int i = 0 ; i < m_vector.size(); i++)
    {
        if(!m_vector.at(i)->isDead())
        {
            m_vector.at(i)->doSomething();
            
            //INCOMPLEETE - tring to implement zombie cab mechanics
            if(m_vector.at(i)->canDrive())
            {
                lane = m_vector.at(i)->getLane();
            }
        }
    }//end for loop
    
    //update actors on screen
    m_bonus--;  //decrement bonus 1 per tick
    if(m_bonus < 0)
        m_bonus = 0; //bonus can't be negative
    m_score += m_racer->getScore(); //update score
    m_racer->setScore(0);
    
    //remove dead actors
    remove();
    //insert new border lines
    insert();
    
    //update scoreline
    if(m_numSouls == m_racer->getSouls())
    {
        m_score += m_bonus;
        playSound(SOUND_FINISHED_LEVEL);
        return GWSTATUS_FINISHED_LEVEL;
    }
    //if the user loses all hp, reset game and decremeent lives
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

//deletes and removes all actors after GW_STATUS... is called
void StudentWorld::cleanUp()
{
    if(m_vector.empty())
        return;
    //deletes from back to front
    vector<Actor*>::iterator it = m_vector.end()-1;
    for(; it != m_vector.begin()-1;)
    {
        delete *it;
        m_vector.pop_back();
        it = m_vector.end()-1;
    }
}

//removes dead actors at the end of every tick
void StudentWorld::remove()
{
    //deletes and removes from front to back if dead
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

//destructor
StudentWorld::~StudentWorld()
{
    cleanUp();
}

//general function used by most Actors to tell if they overlap with another Actor
bool StudentWorld::overlaps(const Actor* a1, const Actor* a2) const
{
    double delta_x = abs(a1->getX() - a2->getX());
    double delta_y = abs(a1->getY() - a2->getY());
    double radius_sum = a1->getRadius() + a2->getRadius();
        
    return ( (delta_x < (radius_sum * 0.25)) && (delta_y < (radius_sum * 0.6)) );
}

//inserts new borders after every tick
void StudentWorld::insert()
{
    //insert based on last y coordinate
    double new_border_y = (double)VIEW_HEIGHT - (double)SPRITE_HEIGHT;
    y = y - 4 - m_racer->getVerticalSpeed();
    double last_y = y;
    double delta_y = new_border_y - last_y;
    //create new yellow borders on either side
    if(delta_y >= SPRITE_HEIGHT)
    {
        Yellow* leftLine = new Yellow(LEFT_EDGE,VIEW_HEIGHT,this);
        m_vector.push_back(leftLine);
        Yellow* rightLine = new Yellow(RIGHT_EDGE,VIEW_HEIGHT,this);
        m_vector.push_back(rightLine);
    }
    //create new lane borders on either side of the road
    if(delta_y >= (4*SPRITE_HEIGHT))
    {
        White* leftLine = new White(left_middle,new_border_y,this);
        m_vector.push_back(leftLine);
        White* rightLine = new White(right_middle,new_border_y,this);
        m_vector.push_back(rightLine);
        
        //update last y coordinate
        y = (m_vector.back())->getY();
    }
}

//chance of an oil slick spawning
int StudentWorld::chanceOilSlick()
{
    int oil_max = std::max(150 - getLevel() * 10, 40);
    return randInt(0, oil_max-1);
}
//chance of a zombie ped spawning
int StudentWorld::chanceZPed()
{
    int zped_max = std::max(100 - getLevel() * 10, 20);
    return randInt(0, zped_max-1);
}
//chance of a human ped spawning
int StudentWorld::chanceHPed()
{
    int hped_max = std::max(200 - getLevel() * 10, 30);
    return randInt(0, hped_max-1);
}
//chance of a holy water bottle spawning
int StudentWorld::chanceBottleRefill()
{
    return randInt(0,100 + 10 * getLevel());
}
//chance of a lost soul spawning
int StudentWorld::chanceSoul() {return randInt(0,99);}

//chance of a zombie cab spawning
int StudentWorld::chanceCab()
{
    int cab_max = max(100-getLevel()*10,20);
    return randInt(0,cab_max-1);
}

//collectively adds new actors based on chances listed above. Written out this way so
//each Actor has an independent chance of spawning in without all spawning in at once
void StudentWorld::chanceAddActors()
{
    //add new oil slick
    if(chanceOilSlick() == 0)
    {
        OilSlick* oil = new OilSlick(randInt(LEFT_EDGE+SPRITE_WIDTH, RIGHT_EDGE - SPRITE_WIDTH), VIEW_HEIGHT, this);
        m_vector.push_back(oil);
    }
    //add new zombie ped
    if(chanceZPed() == 0)
    {
        ZombiePedestrian* zped = new ZombiePedestrian(randInt(0, VIEW_WIDTH), VIEW_HEIGHT, this);
        m_vector.push_back(zped);
    }
    //add new human ped
    if(chanceHPed() == 0)
    {
        HumanPedestrian* hped = new HumanPedestrian(randInt(0, VIEW_WIDTH), VIEW_HEIGHT, this);
        m_vector.push_back(hped);
    }
    //add new holy water refill
    if(chanceBottleRefill() == 0)
    {
        HolyWaterGoodie* water = new HolyWaterGoodie(randInt(LEFT_EDGE+SPRITE_WIDTH, RIGHT_EDGE - SPRITE_WIDTH), VIEW_HEIGHT, this);
        m_vector.push_back(water);
    }
    //add new lost soul
    if(chanceSoul() == 0)
    {
        SoulGoodie* soul = new SoulGoodie(randInt(LEFT_EDGE+SPRITE_WIDTH, RIGHT_EDGE - SPRITE_WIDTH), VIEW_HEIGHT, this);
        m_vector.push_back(soul);
    }
    //add new zombie cab - NOT FINISHED
    if(chanceCab() == 0)
    {
        //unfinished - currently spawns cabs in at y = VIEW_HEIGHT in one of 3 lanes
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

//BUGGED - when overlapping with appropriate actor, projectile keeps moving and doesn't affect any Actor
bool StudentWorld::sprayFirstAppropriateActor(Actor* a)
{
    vector<Actor*>::iterator it = m_vector.begin();
    for(; it != m_vector.end(); it++)
    {
        //if the other actor is alive, overlapping with a holy water spray and is a zombie ped, cab, soul,
        //return true to signal that the holy water spray should be updated based on its action
        if(((*it)->isDead() == false) && a->isOverlap(*it) && (*it)->beSprayedIfAppropriate() == true)
        {
            //increaseScore(*it)//to be implemented: increase score for hitting certain Actors
            return true;
        }
    }
    return false;
}

//adds actor into world - used in Actor.cpp
void StudentWorld::addActor(Actor* a)
{
    m_vector.push_back(a);
}

//INCOMPLETE - checks if another actor is in the same lane
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
