#include "Health.h"
#include <GameObject.h>
#include <sstream>

#include "ComponentRegister.h"

REGISTER_FACTORY(Health);

Health::Health(GameObject* gameObject) : UserComponent(gameObject)
{

}

Health::~Health()
{

}

void Health::start()
{
	maxHealth = health;
	alive = true;
}

void Health::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "health")
		{
			if (!(ss >> health))
				LOG("HEALTH: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else
			LOG("HEALTH: Invalid property name \"%s\"", prop.first.c_str());
	}
}

void Health::onObjectEnter(GameObject* other)
{
	if (other->getTag() == "Ball")
		receiveDamage(1);
}

int Health::getHealth()
{
	return health;
}

int Health::getMaxHealth()
{
	return maxHealth;
}

void Health::setHealth(int health)
{
	this->health = health;
}

void Health::receiveDamage(int damage)
{
	health -= damage;

	if (health < 0) health = 0;
	if (health == 0) alive = false;
}

bool Health::isAlive()
{
	return alive;
}