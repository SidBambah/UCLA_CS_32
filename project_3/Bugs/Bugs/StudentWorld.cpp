#include "StudentWorld.h"
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "Compiler.h"
using namespace std;

StudentWorld::StudentWorld(std::string assetDir) : GameWorld(assetDir) {
	for (int i = 0; i < 4; i++) {
		allCompilers[i] = nullptr;
		m_anthills[i] = nullptr;
	}
}

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}


int StudentWorld::loadField() {
	std::string fieldFile = getFieldFilename();
	std::string error;
	if (f.loadField(fieldFile, error) != Field::LoadResult::load_success)
	{
		setError(fieldFile + " " + error);
		return GWSTATUS_LEVEL_ERROR;
	}

	std::vector<std::string> fileNames = getFilenamesOfAntPrograms();
	for (unsigned int i = 0; i < fileNames.size(); i++) {
		allCompilers[i] = new Compiler;                                    //COMPILER MEMORY ALLOCATIONS
		if (!allCompilers[i]->compile(fileNames[i], error)) {
			setError(fileNames[0] + " " + error);
			return GWSTATUS_LEVEL_ERROR;
		}
	}
	for (int i = 0; i < VIEW_WIDTH; i++) {
		for (int j = 0; j < VIEW_HEIGHT; j++) {
			Field::FieldItem item = f.getContentsOf(i, j);
			if (item == Field::FieldItem::rock)
				m_actors[j][i].push_back(new Pebble(i, j, this));
			if (item == Field::FieldItem::grasshopper)
				m_actors[j][i].push_back(new BabyHopper(i, j, this));
			if (item == Field::FieldItem::food)
				m_actors[j][i].push_back(new Food(i, j, 6000, this));                           //FIELD GENERATION
			if (item == Field::FieldItem::poison)
				m_actors[j][i].push_back(new Poison(i, j, this));
			if (item == Field::FieldItem::anthill0) {
				AntHill* temp = new AntHill(i, j, 0, allCompilers[0], this);
				m_actors[j][i].push_back(temp);
				m_anthills[0] = temp;
			}
			if (item == Field::FieldItem::anthill1) {
				AntHill* temp = new AntHill(i, j, 1, allCompilers[1], this);
				m_actors[j][i].push_back(temp);
				m_anthills[1] = temp;
			}
			if (item == Field::FieldItem::anthill2) {
				AntHill* temp = new AntHill(i, j, 2, allCompilers[2], this);
				m_actors[j][i].push_back(temp);
				m_anthills[2] = temp;
			}
			if (item == Field::FieldItem::anthill3) {
				AntHill* temp = new AntHill(i, j, 3, allCompilers[3], this);
				m_actors[j][i].push_back(temp);
				m_anthills[3] = temp;
			}
			if (item == Field::FieldItem::water)
				m_actors[j][i].push_back(new WaterPool(i, j, this));
		}
	}
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::removeDeadSimulationObjects() {
	for (int i = 0; i < VIEW_WIDTH; i++) {
		for (int j = 0; j < VIEW_HEIGHT; j++) {
			std::list<Actor*>::iterator ic = m_actors[j][i].begin();
			while (ic != m_actors[j][i].end()) {                                     //LOOP THAT REMOVES DEAD OBJECTS
				if ((*ic)->isDead()) {
					delete (*ic);
					ic = m_actors[j][i].erase(ic);
				}
				else
					ic++;
			}
		}
	}
}

void StudentWorld::addActor(Actor* a, int x, int y) {
	m_actors[y][x].push_back(a);
	Food* temp = dynamic_cast<Food*>(a);
	Pheromone* temp2 = dynamic_cast<Pheromone*>(a);
	if (temp != nullptr) {
		std::list<Actor*>::iterator ic = m_actors[y][x].begin();
		while (ic != m_actors[y][x].end()) {
			Food* foodtoRemove = dynamic_cast<Food*>(*ic);
			if (foodtoRemove != nullptr) {
				temp->setHit(foodtoRemove->getHit());               //JOINS MULTIPLE FOOD OBJECTS ON ONE SQUARE
				foodtoRemove->setDead();
			}
			
			ic++;
		}
	}
	if (temp2 != nullptr) {
		std::list<Actor*>::iterator ic = m_actors[y][x].begin();
		while (ic != m_actors[y][x].end()) {                       //JOINS MULTIPLE PHEROMONE OBJECTS ON ONE SQUARE
			Pheromone* pheromonetoRemove = dynamic_cast<Pheromone*>(*ic);
			if (pheromonetoRemove != nullptr) {
				temp2->setHit(pheromonetoRemove->getHit());
				pheromonetoRemove->setDead();
			}
			ic++;
		}
	}
}

Actor* StudentWorld::getEdibleAt(int x, int y) {
	std::list<Actor*>::iterator ic = m_actors[y][x].begin();
	while (ic != m_actors[y][x].end()) {
		Food* temp = dynamic_cast<Food*>(*ic);               //RETURNS A POINTER TO FOOD OBJECTS ON SQUARE
		if (temp != nullptr)
			return temp;
		ic++;
	}
	return nullptr;
}

bool StudentWorld::stunAllStunnableAt(int x, int y) {
	std::list<Actor*>::iterator ic = m_actors[y][x].begin();
	while (ic != m_actors[y][x].end()) {
		Insect* temp = dynamic_cast<Insect*>(*ic);
		if(temp != nullptr)
			temp->stun();
		ic++;
	}
	return true;
}

bool StudentWorld::poisonAllPoisonableAt(int x, int y) {
	std::list<Actor*>::iterator ic = m_actors[y][x].begin();
	while (ic != m_actors[y][x].end()) {
		Insect* temp = dynamic_cast<Insect*>(*ic);
		if (temp != nullptr)
			temp->poison();
		ic++;
	}
	return true;;
}

bool StudentWorld::grassHopperBite(int x, int y) {
	std::list<Actor*>::iterator ic = m_actors[y][x].begin();
	while (ic != m_actors[y][x].end()) {
		Insect* temp = dynamic_cast<Insect*>(*ic);
		if (temp != nullptr) {
			temp->bite(50);
			return true;
		}
		ic++;
	}
	return true;;
}

bool StudentWorld::antBite(int x, int y, int colonyNum) {
	std::list<Actor*>::iterator ic = m_actors[y][x].begin();
	while (ic != m_actors[y][x].end()) {                          //ANT BITES ALL ENEMIES ON SQUARE
		Ant* temp = dynamic_cast<Ant*>(*ic);					// EXCEPT ANTS OF SAME COLONY
		if (temp != nullptr) {
			if (temp->getColonyNum() != colonyNum)
				temp->bite(15);
		}
		Insect* temp2 = dynamic_cast<Insect*>(*ic);
		if (temp2 != nullptr)
			temp2->bite(15);
		ic++;
	}
	return true;
}

int StudentWorld::getWinningAnt() {
	int	winningAntNumber = 0;
	int numAnts[4] = { numberOfAnts(0), numberOfAnts(1), numberOfAnts(2), numberOfAnts(3) };
	for (int i = 0; i < 4; i++) {
		if (numAnts[winningAntNumber] < numAnts[i])
			winningAntNumber = i;
	}
	return winningAntNumber;
}

void StudentWorld::setDisplayText() {
	int	winningAntNumber = getWinningAnt();
	ostringstream output;
	output << "Ticks:" << setw(5) << m_ticks << " ";
	for (int i = 0; i < 4; i++) {
		if (allCompilers[i] != nullptr) {
			if (i == winningAntNumber) {
				output << allCompilers[i]->getColonyName() << "*: ";
				output.fill('0');
				output << setw(2) << numberOfAnts(i);                  //FORMATS AND DISPLAYS A STRING 
				output << "  ";											//OF ALL ANTS CREATED BY EACH ANTHILL
			}
			else {
				output << allCompilers[i]->getColonyName() << ": ";		
				output.fill('0');
				output << setw(2) << numberOfAnts(i);
				output << "  ";
			}
		}
		if (allCompilers[i] == nullptr)
			break;
	}
	string s = output.str();
	setGameStatText(s);
}

int StudentWorld::numberOfAnts(int anthill) {
	for (int i = 0; i < VIEW_WIDTH; i++) {
		for (int j = 0; j < VIEW_HEIGHT; j++) {
			std::list<Actor*>::iterator ic = m_actors[j][i].begin();             //RETURNS NUMBER OF ANTS AT
			while (ic != m_actors[j][i].end()) {								//PROVIDED ANTHILL
				AntHill* temp = dynamic_cast<AntHill*>(*ic);
				if (temp != nullptr && temp->getColonyNum() == anthill) {
					if (temp->getCompiler() != nullptr)
						return temp->getNumAnts();
					if (temp->getCompiler() == nullptr)
						return -1;
				}
				ic++;
			}
		}
	}
	return 0;
}

bool StudentWorld::isMyAnthill(int x, int y, int colonyNum) {
	std::list<Actor*>::iterator ic = m_actors[y][x].begin();               //DETERMINES IF AN ANT IS ON ITS 
	while (ic != m_actors[y][x].end()) {									//OWN ANTHILL
		AntHill* temp = dynamic_cast<AntHill*>(*ic);
		if (temp != nullptr) {
			if(temp->getColonyNum() == colonyNum)
				return true;
		}
		ic++;
	}
	return false;;
}

bool StudentWorld::isEnemy(int x, int y, int colonyNum) {
	std::list<Actor*>::iterator ic = m_actors[y][x].begin();
	while (ic != m_actors[y][x].end()) {
		Insect* temp = dynamic_cast<Insect*>(*ic);
		Ant* temp2 = dynamic_cast<Ant*>(*ic);								//DETERMINES IF THERE IS AN ENEMY
		if (temp != nullptr) {												//ON THIS POSITION
			if (temp2 == nullptr)
				return true;
			if (temp2->getColonyNum() != colonyNum)
				return true;
		}
			ic++;
	}
	return false;
}

int StudentWorld::pheromoneStrength(int x, int y, int colonyNum) {
	std::list<Actor*>::iterator ic = m_actors[y][x].begin();
	while (ic != m_actors[y][x].end()) {
		Pheromone* temp = dynamic_cast<Pheromone*>(*ic);                   //DETERMINES THE AMOUNT OF ANT'S TYPE
		if (temp != nullptr) {												//OF PHEROMONE ON SQUARE
			if (temp->getColonyNum() == colonyNum)
				return temp->getHit();
		}
	}
	return 0;
}