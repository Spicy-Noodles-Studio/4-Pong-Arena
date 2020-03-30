#include "Health.h"
#include <sstream>
#include "GameObject.h"

Health::Health(GameObject* gameObject) : UserComponent(gameObject)
{

}

Health::~Health()
{

}

void Health::start()
{
	alive = true;
	maxHealth = health;

}


void Health::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties()) {
		std::stringstream ss(prop.second);

		if (prop.first == "health") {
			if (!(ss >> health))
				LOG("HEALTH: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else if (prop.first == "size") {
			if (!(ss >> size))
				LOG("HEALTH: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else
			LOG("HEALTH: Invalid property name \"%s\"", prop.first.c_str());
	}
}

void Health::receiveDamage(int damage)
{
	
	health -= damage;
	if (health < 0) health = 0;

	if (health == 0)
	{
			die();
	}
}

void Health::die()
{
	alive = false;

	// kick the player out of the game (?)
	//...
	// save info to show in the winner screen (position of the podium, kills, etc.) (?)
	//...
}



int Health::getHealth()
{
	return health;
}

int Health::getMaxHealth()
{
	return maxHealth;
}


double Health::getTriggerSize()
{
	return size;
}

void Health::setTriggerSize(double _size)
{
	this->size = _size;
}

void Health::setHealth(int health)
{
	this->health = health;
}

bool Health::isAlive()
{
	return alive;
}


void Health::onObjectEnter(GameObject* other)
{
	if (other->getTag() == "ball") {
		receiveDamage(1);
	}
}
