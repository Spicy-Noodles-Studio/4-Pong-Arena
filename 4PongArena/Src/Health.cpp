#include "Health.h"
#include <GameObject.h>
#include <sstream>

#include <ComponentRegister.h>
#include <SoundEmitter.h>

REGISTER_FACTORY(Health);

Health::Health(GameObject* gameObject) : UserComponent(gameObject), health(0)
{

}

Health::~Health()
{

}

void Health::start()
{
	if (gameObject != nullptr) soundEmitter = gameObject->getComponent<SoundEmitter>();
	if (soundEmitter != nullptr) soundEmitter->setVolume(1.2);
}

void Health::handleData(ComponentData* data)
{
	if (data == nullptr) return;

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

void Health::receiveDamage(int damage)
{
	health -= damage;
	if (health > 0) soundEmitter->playSound("Damage");
	if (health < 0) health = 0;
}

bool Health::isAlive() const
{
	return health > 0;
}