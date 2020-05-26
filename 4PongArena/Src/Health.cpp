#include "Health.h"
#include <GameObject.h>
#include <sstream>

#include <ComponentRegister.h>
#include <SoundEmitter.h>

REGISTER_FACTORY(Health);

Health::Health(GameObject* gameObject) : UserComponent(gameObject), soundEmitter(nullptr), health(0)
{

}

Health::~Health()
{
	soundEmitter = nullptr;
}

void Health::start()
{
	checkNullAndBreak(gameObject);
	
	soundEmitter = gameObject->getComponent<SoundEmitter>();
	if (notNull(soundEmitter)) soundEmitter->setVolume(1.2);
}

void Health::handleData(ComponentData* data)
{
	checkNullAndBreak(data);

	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "health")
		{
			setInt(health);
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
	if (health > 0 && notNull(soundEmitter)) soundEmitter->playSound("Damage");
	if (health < 0) health = 0;
}

bool Health::isAlive() const
{
	return health > 0;
}