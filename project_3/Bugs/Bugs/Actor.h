#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

//CLASS PROTOTYPES

class Compiler;
class StudentWorld;

//ACTOR CLASS DEFINITION

class Actor : public GraphObject {
public:
	Actor(int imageID, int startX, int startY, StudentWorld* world, Direction dir = right, int depth = 0);
	virtual void doSomething() = 0;
	virtual bool blocksInsect() { return false; }
	virtual bool isDead() { return m_dead; }
	virtual void setDead() { m_dead = true; }
	bool isBlocked(int x, int y);
	virtual bool isEdible() const { return false; }
	virtual bool isPheromone() const { return false; }
	StudentWorld* getWorld() { return m_world; }
private:
	bool m_dead;
	StudentWorld* m_world;
};

//ENERGYHOLDER CLASS DEFINITION

class EnergyHolder : public Actor {
public:
	EnergyHolder(int imageID, int startX, int startY, StudentWorld* world, Direction dir = right, int depth = 0);
	int getHit() { return m_hit; }
	void setHit(int hit) { m_hit = hit; }
private:
	int m_hit;
};

//INSECT CLASS DEFINITION

class Insect : public EnergyHolder {
public: 
	Insect(int imageID, int startX, int startY, StudentWorld* world, Direction dir = right, int depth = 0);
	virtual void doSomething() = 0;
	virtual void stun() = 0;
	virtual void poison() = 0;
	virtual void bite(int amt) = 0;
	int getSleep() { return m_sleep; }
	void setSleep(int sleep) { m_sleep = sleep; }
private:
	int m_sleep;
};

//PEBBLE CLASS DEFINITION

class Pebble : public Actor {
public:
	Pebble(int startX, int startY, StudentWorld* world);
	virtual void doSomething() { return; }
	virtual bool blocksInsect() { return true; }

};

//FOOD CLASS DEFINITION

class Food : public EnergyHolder {
public:
	Food(int startX, int startY, int units, StudentWorld* world);
	void doThing();
	virtual void doSomething() { doThing(); }
	virtual bool isEdible() const { return true; }
};

//ANTHILL CLASS DEFINITION

class AntHill : public EnergyHolder {
public:
	AntHill(int startx, int startY, int colonyNum, Compiler* compiler, StudentWorld* world);
	void doThing();
	int getNumAnts() { return m_numAnts; }
	void setNumAnts(int numAnts) { m_numAnts = numAnts; }
	int getColonyNum() { return m_colonyNum; }
	virtual void doSomething() { doThing(); }
	virtual void setDead();
	Compiler* getCompiler() { return m_compiler; }
private:
	Compiler* m_compiler;
	int m_numAnts;
	int m_colonyNum;
	int imageID;
};

//PHEROMONE CLASS DEFINITION

class Pheromone : public EnergyHolder {
public:
	Pheromone(int imageID, int startX, int startY, int colonyNum, StudentWorld* world);
	void doThing();
	virtual void doSomething() { doThing(); }
	virtual bool isPheromone() const { return true; }
	int getColonyNum() { return m_colonyNum; }
private:
	int m_colonyNum;
};

//WATERPOOL CLASS DEFINITION

class WaterPool : public EnergyHolder {
public:
	WaterPool(int startX, int startY, StudentWorld* world);
	void doThing();
	virtual void doSomething() { doThing(); }
};

//POISON CLASS DEFINITION

class Poison : public EnergyHolder {
public:
	Poison(int startX, int startY, StudentWorld* world);
	void doThing();
	virtual void doSomething() { doThing(); }
};

//ANT CLASS DEFINITION

class Ant : public Insect {
public:
	Ant(int imageID, int startX, int startY, int colonyNum, Compiler* compiler, StudentWorld* world);
	void doThing();
	virtual void doSomething() { doThing(); }
	virtual void bite(int amt);
	virtual void stun();
	virtual void poison();
	virtual void setDead();
	bool compilerInterpreter();
	bool checkIfCommand(int condition);
	int getColonyNum() { return m_colonyNum; }
private:
	int m_stunned;
	int m_colonyNum;
	int m_heldFood;
	int m_lastRandomNumber;
	int m_ic;
	bool m_previouslyBlocked;
	bool m_previouslyStunned;
	bool m_previouslyBitten;
	Compiler* m_compiler;
};

//GRASSHOPPER CLASS DEFINITION

class GrassHopper : public Insect {
public:
	GrassHopper(int imageID, int startX, int startY, StudentWorld* world);
	virtual void doSomething() = 0;
	void moveInSetDirection();
	int getDistance() { return m_distance; }
	bool eatFood();
private:
	int m_distance;
};

//BABYHOPPER CLASS DEFINITION

class BabyHopper : public GrassHopper {
public:
	BabyHopper(int startX, int startY, StudentWorld* world);
	virtual void stun();
	virtual void poison();
	void doThing();
	virtual void doSomething() { doThing(); }
	virtual void bite(int amt);
private:
	int m_stunned;
};

//ADULTHOPPER CLASS DEFINITION

class AdultHopper : public GrassHopper {
public:
	AdultHopper(int startX, int startY, StudentWorld* world);
	void doThing();
	virtual void stun() { return; }
	virtual void poison() { return; }
	virtual void doSomething() { doThing(); }
	virtual void bite(int amt);
};

int pheromoneIDChooser(int colonyNum);
int antIDChooser(int colonyNum);

#endif // ACTOR_H_