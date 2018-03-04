#include "Actor.h"
#include "StudentWorld.h"
#include "Compiler.h"
#include <cmath>

//CONSTRUCTORS

Actor::Actor(int imageID, int startX, int startY, StudentWorld* world, Direction dir, int depth) : GraphObject(imageID, startX, startY, dir, depth, 0.25) { 
	m_world = world; 
	m_dead = false;
}
EnergyHolder::EnergyHolder(int imageID, int startX, int startY, StudentWorld* world, Direction dir, int depth) : Actor(imageID, startX, startY, world, dir, depth) {}

Insect::Insect(int imageID, int startX, int startY, StudentWorld* world, Direction dir, int depth) : EnergyHolder(imageID, startX, startY, world, dir, depth) {
	m_sleep = 0;
}

Pebble::Pebble(int startX, int startY, StudentWorld* world) : Actor(IID_ROCK, startX, startY, world, right, 1) {}

Food::Food(int startX, int startY, int units, StudentWorld* world) : EnergyHolder(IID_FOOD, startX, startY, world, right, 2) { setHit(units); }

AntHill::AntHill(int startX, int startY, int colonyNum, Compiler* compiler, StudentWorld* world) : EnergyHolder(IID_ANT_HILL, startX, startY, world, right, 2) {
	setHit(8999);
	m_colonyNum = colonyNum;
	switch (m_colonyNum) {
	case 0:
		imageID = IID_ANT_TYPE0;
		break;
	case 1:
		imageID = IID_ANT_TYPE1;
		break;
	case 2:
		imageID = IID_ANT_TYPE2;
		break;
	case 3:
		imageID = IID_ANT_TYPE3;
		break;
	}
	m_numAnts = 0;
	m_compiler = compiler;
}

Pheromone::Pheromone(int imageID, int startX, int startY, int colonyNum, StudentWorld* world) : EnergyHolder(imageID, startX, startY, world, right, 2) {
	setHit(256);
	m_colonyNum = colonyNum;
}

WaterPool::WaterPool(int startX, int startY, StudentWorld* world) : EnergyHolder(IID_WATER_POOL, startX, startY, world, right, 2) {}

Poison::Poison(int startX, int startY, StudentWorld* world) : EnergyHolder(IID_POISON, startX, startY, world, right, 2) {}

Ant::Ant(int imageID, int startX, int startY, int colonyNum, Compiler* compiler, StudentWorld* world)
	: Insect(imageID, startX, startY, world, static_cast<GraphObject::Direction>(randInt(1, 4)), 1)
{
	m_compiler = compiler;
	setHit(1500);
	m_colonyNum = colonyNum;
	m_lastRandomNumber = 0;
	m_ic = 0;
	m_previouslyStunned = false;
	m_previouslyBitten = false;
	m_previouslyBlocked = false;
	m_heldFood = 0;
	m_stunned = 0;
}

GrassHopper::GrassHopper(int imageID, int startX, int startY, StudentWorld* world) : Insect(imageID, startX, startY, world, static_cast<GraphObject::Direction>(randInt(1,4)), 1) {
	m_distance = randInt(2, 10);
}

BabyHopper::BabyHopper(int startX, int startY, StudentWorld* world) : GrassHopper(IID_BABY_GRASSHOPPER, startX, startY, world) { 
	setHit(500); 
	m_stunned = 0;
}

AdultHopper::AdultHopper(int startX, int startY, StudentWorld* world) : GrassHopper(IID_ADULT_GRASSHOPPER, startX, startY, world) { setHit(1600); }

//ACTOR HELPER FUNCTION(S)

bool Actor::isBlocked(int x, int y){ return m_world->isBlocked(x, y); }

//FOOD HELPER FUNCTION(S)

void Food::doThing() {
	if (getHit() == 0)
		setDead();
}

//ANTHILL HELPER FUNCTION(S)

void AntHill::setDead(){
	Actor::setDead();
}

void AntHill::doThing() {
	if (m_compiler == nullptr) {
		return;
	}
	setHit(getHit() - 1);
	if (getHit() == 0) {
		setDead();
		return;
	}
	if (getHit() >= 2000) {		
		m_numAnts++;
		getWorld()->addActor(new Ant(antIDChooser(m_colonyNum), getX(), getY(), m_colonyNum, getCompiler(), getWorld()), getX(), getY());
		setHit(getHit() - 1500);
		return;
	}
	Food* temp = static_cast<Food*>(getWorld()->getEdibleAt(getX(), getY()));
	if (temp != nullptr && temp->getHit() <= 10000) {
			setHit(getHit() + temp->getHit());
			temp->setHit(0);
			return;
	}
}

//PHEROMONE HELPER FUNCTION(S)

void Pheromone::doThing() {
	setHit(getHit() - 1);
	if (getHit() == 0)
		setDead();
	return;
}

//WATERPOOL HELPER FUNCTION(S)

void WaterPool::doThing() {
	getWorld()->stunAllStunnableAt(getX(), getY());
	return;
}

//POISON HELPER FUNCTION(S)

void Poison::doThing() {
	getWorld()->poisonAllPoisonableAt(getX(), getY());
	return;
}

//ANT HELPER FUNCTION(S)

void Ant::doThing() {
	setHit(getHit() - 1);
	if (getHit() == 0) {
		getWorld()->addActor(new Food(getX(), getY(), 100, getWorld()), getX(), getY());
		setDead();
		return;
	}
	if (getSleep() != 0) {
		setSleep(getSleep() - 1);
		return;
	}
	if (compilerInterpreter() == false) {
		setDead();
	}
	return;
}

void Ant::bite(int amt) {
	m_previouslyBitten = true;
	setHit(getHit() - amt);
	if (getHit() <= 0) {
		getWorld()->addActor(new Food(getX(), getY(), 100, getWorld()), getX(), getY());
		setDead();
		return;
	}
	return;
}

void Ant::poison() {
	setHit(getHit() - 150);
	if (getHit() <= 0) {
		getWorld()->addActor(new Food(getX(), getY(), 100, getWorld()), getX(), getY());
		setDead();
		return;
	}
	return;
}

void Ant::stun() {
	if (m_stunned > 0)
		m_stunned--;
	if (m_stunned == 0) {
		m_stunned = 3;
		setSleep(getSleep() + 2);
	}
	return;
}

void Ant::setDead() {
	Actor::setDead();
	if (getWorld()->getAnthill(m_colonyNum)->getNumAnts() == 0)
		return;
	getWorld()->getAnthill(m_colonyNum)->setNumAnts(getWorld()->getAnthill(m_colonyNum)->getNumAnts() - 1);
}

bool Ant::checkIfCommand(int condition) {
	switch (condition) {
	case 9:
	{
		if (m_lastRandomNumber == 0) {
			return true;
		}			
		return false;
	}
	case 3:
	{
		if (m_heldFood != 0)
			return true;
		return false;
	}
	case 4:
	{
		if (getHit() <= 25)
			return true;
		return false; 
	}
	case 7:
	{
		return getWorld()->isEnemy(getX(), getY(), m_colonyNum);
	}
	case 6:
	{
		return ! (getWorld()->getEdibleAt(getX(), getY()) == nullptr);
	}
	case 5:
	{
		if (getWorld()->isMyAnthill(getX(), getY(), m_colonyNum) == true)
			return true;
		return false;
	}
	case 1:
	{
		switch (getDirection()) {
		case up:
			if (getWorld()->pheromoneStrength(getX(), getY() + 1, m_colonyNum) > 0)
				return true;
			return false;
		case down:
			if (getWorld()->pheromoneStrength(getX(), getY() - 1, m_colonyNum) > 0)
				return true;
			return false;
		case left:
			if (getWorld()->pheromoneStrength(getX() - 1, getY(), m_colonyNum) > 0)
				return true;
			return false;
		case right:
			if (getWorld()->pheromoneStrength(getX() + 1, getY(), m_colonyNum) > 0)
				return true;
			return false;
		}
		return false;
	}
	case 0:
	{
		switch (getDirection()) {
		case up:
			return getWorld()->isEnemy(getX(), getY() + 1, m_colonyNum);
		case down:
			return getWorld()->isEnemy(getX(), getY() - 1, m_colonyNum);
		case left:
			return getWorld()->isEnemy(getX() - 1, getY(), m_colonyNum);
		case right:
			return getWorld()->isEnemy(getX() + 1, getY() - 1, m_colonyNum);

		}
		return false;
	}
	case 2:
	{
		return m_previouslyBitten;
	}
	case 8:
	{
		return m_previouslyBlocked;
	}
	}
	return false;
}

bool Ant::compilerInterpreter() {
	Compiler::Command cmd;
	if (m_ic > 9)
		m_ic = 0;
	int ic = m_ic;
	while (m_ic < 10) {
		if (!(*m_compiler).getCommand(ic, cmd))
			return false;
		switch (cmd.opcode) {
		case Compiler::Opcode::emitPheromone:
			m_ic++;
			if (getWorld()->pheromoneStrength(getX(), getY(), m_colonyNum) <= 512) {
				getWorld()->addActor(new Pheromone(pheromoneIDChooser(m_colonyNum), getX(), getY(), m_colonyNum, getWorld()), getX(), getY());
				return true;
			}
			if (getWorld()->pheromoneStrength(getX(), getY(), m_colonyNum) == 768) {
				return true;
			}
			if (getWorld()->pheromoneStrength(getX(), getY(), m_colonyNum) <= 512) {
				Pheromone* toAdd = new Pheromone(pheromoneIDChooser(m_colonyNum), getX(), getY(), m_colonyNum, getWorld());
				toAdd->setHit(768 - getWorld()->pheromoneStrength(getX(), getY(), m_colonyNum));
				getWorld()->addActor(toAdd, getX(), getY());
				return true;
			}
		case Compiler::Opcode::faceRandomDirection:
			setDirection(static_cast<GraphObject::Direction>(randInt(1, 4)));
			m_ic++;
			return true;
		case Compiler::Opcode::rotateClockwise:
			m_ic++;
			if (getDirection() == up) {
				setDirection(right);
				return true;
			}
			else if (getDirection() == down) {
				setDirection(left);
				return true;
			}
			else if (getDirection() == left) {
				setDirection(up);
				return true;
			}
			else if (getDirection() == right) {
				setDirection(down);
				return true;
			}
		case Compiler::Opcode::rotateCounterClockwise:
			m_ic++;
			if (getDirection() == up) {
				setDirection(left);
				return true;
			}
			if (getDirection() == down) {
				setDirection(right);
				return true;
			}
			if (getDirection() == left) {
				setDirection(down);
				return true;
			}
			if (getDirection() == right) {
				setDirection(up);
				return true;
			}
		case Compiler::Opcode::moveForward:
			m_ic++;
			switch (getDirection()) {
			case up:
				if (getWorld()->isBlocked(getX(), getY() + 1)) {
					m_previouslyBlocked = true;
					return true;
				}
				moveTo(getX(), getY() + 1);
				m_previouslyBitten = false;
				m_previouslyBlocked = false;
				return true;
			case down:
				if (getWorld()->isBlocked(getX(), getY() - 1)) {
					m_previouslyBlocked = true;
					return true;
				}
				moveTo(getX(), getY() - 1);
				m_previouslyBitten = false;
				m_previouslyBlocked = false;
				return true;
			case left:
				if (getWorld()->isBlocked(getX() - 1, getY())) {
					m_previouslyBlocked = true;
					return true;
				}
				moveTo(getX() - 1, getY());
				m_previouslyBitten = false;
				m_previouslyBlocked = false;
				return true;
			case right:
				if (getWorld()->isBlocked(getX() + 1, getY())) {
					m_previouslyBlocked = true;
					return true;
				}
				moveTo(getX() + 1, getY());
				m_previouslyBitten = false;
				m_previouslyBlocked = false;
				return true;
			}
		case Compiler::Opcode::bite:
			m_ic++;
			getWorld()->antBite(getX(), getY(), m_colonyNum);
			return true;
		case Compiler::Opcode::pickupFood:
		{
			m_ic++;
			Food* temp = dynamic_cast<Food*>(getWorld()->getEdibleAt(getX(), getY()));
			if (temp != nullptr) {
				if (temp->getHit() >= 400) {
					if (m_heldFood == 1800)
						return true;
					if (m_heldFood <= 1400) {
						m_heldFood += 400;
						temp->setHit(temp->getHit() - 400);
						return true;
					}
					if (m_heldFood > 1400) {
						temp->setHit(temp->getHit() - (1800 - m_heldFood));
						m_heldFood = 1800;
						return true;
					}
				}
				if (temp->getHit() < 400) {
					if (m_heldFood == 1800)
						return true;
					if ((m_heldFood + temp->getHit()) <= 1800) {
						m_heldFood = m_heldFood + temp->getHit();
						temp->setHit(0);
						return true;
					}
					if ((m_heldFood + temp->getHit()) > 1800) {
						temp->setHit(temp->getHit() - (1800 - m_heldFood));
						m_heldFood = 1800;
						return true;
					}
				}
			}
			return true;
		}
		case Compiler::Opcode::dropFood:
			getWorld()->addActor(new Food(getX(), getY(), m_heldFood, getWorld()), getX(), getY());
			m_heldFood = 0;
			m_ic++;
			return true;
		case Compiler::Opcode::eatFood:
			if (m_heldFood > 100) {
				setHit(getHit() + 100);
				m_heldFood -= 100;
			}
			if (m_heldFood < 100) {
				setHit(getHit() + m_heldFood);
				m_heldFood = 0;
			}
			m_ic++;
			return true;
		case Compiler::Opcode::generateRandomNumber:
			m_lastRandomNumber = randInt(0, stoi(cmd.operand2));
			m_ic++;
			break;
		case Compiler::Opcode::if_command:
		{
			if (checkIfCommand(stoi(cmd.operand1)) == true) {
				ic = stoi(cmd.operand2);
				break;
			}
			else{
				m_ic++;
				break;
			}
		}
		case Compiler::Opcode::goto_command:
		{
			ic = stoi(cmd.operand1);
			break;
		}
		}
	}
	return true;
}

//GRASSHOPPER HELPER FUNCTION(S)

void GrassHopper::moveInSetDirection(){
	if (m_distance == 0) {
		setDirection(static_cast<GraphObject::Direction>(randInt(1, 4)));
		m_distance += randInt(2, 10);
		return;
	}
	switch (getDirection()) {
	case up:
		if (getWorld()->isBlocked(getX(), getY() + 1)) {
			m_distance = 0;
			return;
		}
		moveTo(getX(), getY() + 1);
		m_distance--;
		return;
	case right:
		if (getWorld()->isBlocked(getX() + 1, getY())) {
			m_distance = 0;
			return;
		}
		moveTo(getX() + 1, getY());
		m_distance--;
		return;
	case down:
		if (getWorld()->isBlocked(getX(), getY() - 1)) {
			m_distance = 0;
			return;
		}
		moveTo(getX(), getY() - 1);
		m_distance--;
		return;
	case left:
		if (getWorld()->isBlocked(getX() - 1, getY())) {
			m_distance = 0;
			return;
		}
		moveTo(getX() - 1, getY());
		m_distance--;
		return;
	default:
		return;
	}
}

bool GrassHopper::eatFood() {
	Food* temp = static_cast<Food*>(getWorld()->getEdibleAt(getX(), getY()));
	if (temp != nullptr) {
		if (temp->getHit() >= 200) {
			setHit(getHit() + 200);
			temp->setHit(temp->getHit() - 200);
		}
		else {
			setHit(getHit() + temp->getHit());
			temp->setHit(0);
		}
		if (randInt(1, 2) == 1)
			return true;
	}
	return false;
}

//BABYHOPPER HELPER FUNCTION(S)

void BabyHopper::bite(int amt) {
	setHit(getHit() - amt);
	if (getHit() == 0) {
		getWorld()->addActor(new Food(getX(), getY(), 100, getWorld()), getX(), getY());
		setDead();
	}
}

void BabyHopper::doThing() {
	setHit(getHit() - 1);
	if (getHit() == 0) {
		setDead();
		return;
	}
	if (getSleep() != 0) {
		setSleep(getSleep() - 1);
		return;
	}
	setSleep(2);
	if (getHit() >= 1600) {
		getWorld()->addActor(new AdultHopper(getX(), getY(), getWorld()), getX(), getY());
		getWorld()->addActor(new Food(getX(), getY(), 100, getWorld()), getX(), getY());
		setDead();
		return;
	}
	
	if(eatFood() == true)
		return;
		
	moveInSetDirection();
}

void BabyHopper::poison() {
	setHit(getHit() - 150);
	if (getHit() <= 0) {
		getWorld()->addActor(new Food(getX(), getY(), 100, getWorld()), getX(), getY());
		setDead();
	}
}

void BabyHopper::stun() {
	if (m_stunned > 0)
		m_stunned--;
	if (m_stunned == 0) {
		m_stunned = 3;
		setSleep(getSleep() + 2);
	}
}

//ADULTHOPPER HELPER FUNCTION(S)

void AdultHopper::doThing() {
	setHit(getHit() - 1);
	if (getHit() == 0) {
		getWorld()->addActor(new Food(getX(), getY(), 100, getWorld()), getX(), getY());
		setDead();
		return;
	}
	if (getSleep() != 0) {
		setSleep(getSleep() - 1);
		return;
	}
	setSleep(2);
	if (randInt(1, 3) == 1) {
		getWorld()->grassHopperBite(getX(), getY());
		return;
	}
	
	if (randInt(1,10) == 1) {
		int radius = randInt(1, 10);
		int x_move = radius * static_cast<int>(cos(randInt(0, 2 * 3.14)));
		int y_move = radius * static_cast<int>(sin(randInt(0, 2 * 3.14)));
		if (! getWorld()->isBlocked(getX() + x_move, getY() + y_move)) {
			if (getX() + x_move > VIEW_WIDTH || getY() + y_move < VIEW_HEIGHT)
				return;
			moveTo(getX() + x_move, getY() + y_move);
			return;
		}
	}
	if(eatFood() == true)
		return;
		
	moveInSetDirection();
}

void AdultHopper::bite(int amt) {
	setHit(getHit() - amt);
	if (getHit() == 0) {
		getWorld()->addActor(new Food(getX(), getY(), 100, getWorld()), getX(), getY());
		setDead();
		return;
	}
	if (randInt(1, 2) == 1)
		getWorld()->grassHopperBite(getX(), getY());
}

//EXTRA HELPER FUNCTIONS

int pheromoneIDChooser(int colonyNum) {
	switch (colonyNum) {
	case 0:
		return IID_PHEROMONE_TYPE0;
	case 1:
		return IID_PHEROMONE_TYPE1;
	case 2:
		return IID_PHEROMONE_TYPE2;
	case 3:
		return IID_PHEROMONE_TYPE3;
	default:
		return -1;
	}
}

int antIDChooser(int colonyNum) {
	switch (colonyNum) {
	case 0:
		return IID_ANT_TYPE0;
	case 1:
		return IID_ANT_TYPE1;
	case 2:
		return IID_ANT_TYPE2;
	case 3:
		return IID_ANT_TYPE3;
	default:
		return -1;
	}
}
