#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include "Actor.h"
#include "Field.h"
#include "Compiler.h"
#include <list>
#include <iostream>

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir);

	//INIT IMPLEMENTATOIN
	virtual int init(){
		m_ticks = 2000;
		return loadField();
	}

	//MOVE IMPLEMENTATION

	virtual int move(){
		m_ticks--;
		for (int i = 0; i < VIEW_WIDTH; i++) {
			for (int j = 0; j < VIEW_HEIGHT; j++) {
				std::list<Actor*>::iterator ic = m_actors[j][i].begin();
				if (ic == m_actors[j][i].end())
					continue;
				while( ic != m_actors[j][i].end()) {
					int oldX = (*ic)->getX();
					int oldY = (*ic)->getY();
					(*ic)->doSomething();
					int newX = (*ic)->getX();
					int newY = (*ic)->getY();
					if (oldX != newX || oldY != newY) {
						m_actors[(*ic)->getY()][(*ic)->getY()].push_back(*ic);
						ic = m_actors[j][i].erase(ic);
					}
					else {
						ic++;
					}
				}
			}
		}
		removeDeadSimulationObjects();
		setDisplayText();
		if (m_ticks == 0) {
			int winningAntNumber = getWinningAnt();
			for (int i = 0; i < 4; i++) {
				if (numberOfAnts(winningAntNumber) == numberOfAnts(i)) {
					if (winningAntNumber != i)
						return GWSTATUS_NO_WINNER;
				}
			}
			setWinner(allCompilers[winningAntNumber]->getColonyName());
			return GWSTATUS_PLAYER_WON;
		}
		return GWSTATUS_CONTINUE_GAME;
	}

	//CLEANUP IMPLEMENTATION

	virtual void cleanUp(){
		for (int i = 0; i < VIEW_WIDTH; i++) {
			for (int j = 0; j < VIEW_HEIGHT; j++) {
				for (std::list<Actor*>::iterator ic = m_actors[j][i].begin(); ic != m_actors[j][i].end(); ic++) {
					delete (*ic);
					(*ic) = nullptr;
				}
			}
		}
		for (int i = 0; i < 4; i++) {
			delete allCompilers[i];
			allCompilers[i] = nullptr;
		}
	}

	//HELPER FUNCTION DEFINITIONS

	void removeDeadSimulationObjects();
	int getWinningAnt();
	int loadField();
	void setDisplayText();
	void addActor(Actor* a, int x, int y);
	Actor* getEdibleAt(int x, int y);
	bool stunAllStunnableAt(int x, int y);
	AntHill* getAnthill(int colonyNum) { return m_anthills[colonyNum]; }
	int numberOfAnts(int anthill);
	bool poisonAllPoisonableAt(int x, int y);
	bool grassHopperBite(int x, int y);
	bool antBite(int x, int y, int colonyNum);
	bool isMyAnthill(int x, int y, int colonyNum);
	bool isEnemy(int x, int y, int colonyNum);
	int pheromoneStrength(int x, int y, int colonyNum);
	virtual bool isBlocked(int x, int y) {
		if (m_actors[y][x].size() == 0)
			return false;
		for (std::list<Actor*>::iterator ic = m_actors[y][x].begin(); ic != m_actors[y][x].end(); ic++) {
			if ((*ic)->blocksInsect() == true)
				return true;
		}
		return false;
	}

	// DESTRUCTION IMPLEMENTATION

	~StudentWorld(){ cleanUp(); }
private:
	int m_ticks;
	AntHill* m_anthills[4];                          //ARRAY OF POINTERS TO ANTHILLS
	Field f;
	std::list<Actor*> m_actors[VIEW_HEIGHT][VIEW_WIDTH];     //2D ARRAY OF LISTS
	Compiler* allCompilers[4];								//ARRAY OF COMPILER OBJECTS FOR ALL ANTHILLS
};

#endif // STUDENTWORLD_H_
