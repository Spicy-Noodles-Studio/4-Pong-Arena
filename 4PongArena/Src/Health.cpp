#include "Health.h"

#include <ComponentRegister.h>
#include <GameObject.h>
#include <sstream>

REGISTER_FACTORY(Health);

Health::Health(GameObject* gameObject) : UserComponent(gameObject), health(0)
{

}

Health::~Health()
{

}

void Health::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "health")
		{
			if (!(ss >> health))
				LOG("HEALTH: Invalid value for property with name \"%s\"", prop.first.c_str());
		}
		else
			LOG("HEALTH: Invalid property name \"%s\"", prop.first.c_str());
	}
}

int Health::getHealth() const
{
	return health;
}

void Health::setHealth(int health)
{
	this->health = health;
}

bool Health::isAlive() const
{
	return health > 0;
}

bool Health::isDead() const
{
	return !isAlive();
}

void Health::receiveDamage(int damage)
{
	health -= damage;
	if (health < 0) health = 0;
}