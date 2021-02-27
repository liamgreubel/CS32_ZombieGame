#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <vector>

class Actor;
class GhostRacer;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir);
    virtual ~StudentWorld();
    
    virtual int init();
    virtual int move();
    virtual void cleanUp();

      // Return a pointer to the world's GhostRacer.
    GhostRacer* getGhostRacer() {return m_racer;}

      // Add an actor to the world.
    void addActor(Actor* a);

      // Record that a soul was saved.
    void recordSoulSaved();

      // If actor a overlaps some live actor that is affected by a holy water
      // projectile, inflict a holy water spray on that actor and return true;
      // otherwise, return false.  (See Actor::beSprayedIfAppropriate.)
    bool sprayFirstAppropriateActor(Actor* a);

      // Return true if actor a1 overlaps actor a2, otherwise false.
    bool overlaps(const Actor* a1, const Actor* a2) const;

      // If actor a overlaps this world's GhostRacer, return a pointer to the
      // GhostRacer; otherwise, return nullptr
    GhostRacer* getOverlappingGhostRacer(Actor* a) const;
    void remove();
    void insert();
    void setSpray();
    int chanceOilSlick();
    int chanceZPed();
    int chanceHPed();
    int chanceBottleRefill();
    int chanceSoul();
    void chanceAddActors();
    //int chanceHeal();
    
private:
    std::vector<Actor*> m_vector;
    GhostRacer* m_racer;
    double y;
    int m_numSouls;
    int m_bonus;
    int m_score;
    int m_tempScore;
    int m_runningScore;
};

#endif

/*
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
*/
//#endif // STUDENTWORLD_H_

