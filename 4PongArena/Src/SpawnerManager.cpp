#include "SpawnerManager.h"
#include <GameObject.h>
#include <MeshRenderer.h>
#include <RigidBody.h>
#include <MathUtils.h>
#include <sstream>

#include "Spawner.h"
#include "Countdown.h"

#include <ComponentRegister.h>

REGISTER_FACTORY(SpawnerManager);

int SpawnerManager::chooseSpawn()
{
	int i = 0;
	while (i == lastUsed)
		i = random(0, spawners.size());

	return lastUsed = i;
}

GameObject* SpawnerManager::getBall()
{
	int i = 0;
	bool found = false;
	GameObject* ball = nullptr;

	while (i < pool.size() && !found)
	{
		if (!pool[i]->isActive())
		{
			found = true;
			ball = pool[i];

			ball->setActive(true);
			ball->getComponent<MeshRenderer>()->setVisible(true);
		}
		else
			i++;
	}

	if (i >= pool.size())
	{
		ball = instantiate("Ball");
		pool.push_back(ball);
	}

	return ball;
}

SpawnerManager::SpawnerManager(GameObject* gameObject) : UserComponent(gameObject), countdown(nullptr), time(0.0f), generationTime(4.0f), minimumTime(1.0f), lastUsed(0)
{

}

SpawnerManager::~SpawnerManager()
{

}

void SpawnerManager::start()
{
	countdown = findGameObjectWithName("Countdown")->getComponent<Countdown>();

	for (int i = 0; i < 25; i++)
	{
		pool.push_back(instantiate("Ball"));
		pool.back()->setActive(false);
		pool.back()->getComponent<MeshRenderer>()->setVisible(false);
	}

	time = generationTime;
}

void SpawnerManager::update(float deltaTime)
{
	if (!countdown->isCounting())
	{
		if (time > 0)
			time -= deltaTime;
		else
		{
			int index = chooseSpawn();
			spawners[index]->getComponent<Spawner>()->shoot(getBall());

			time = generationTime;
		}
	}
}

void SpawnerManager::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "generationTime")
		{
			if (!(ss >> generationTime))
				LOG("SPAWNER MANAGER: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else if (prop.first == "minimumTime")
		{
			if (!(ss >> minimumTime))
				LOG("SPAWNER MANAGER: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else
			LOG("SPAWNER MANAGER: Invalid property name \"%s\"", prop.first.c_str());
	}
}

void SpawnerManager::setSpawners(std::vector<GameObject*>& spawners)
{
	this->spawners = spawners;
}

std::vector<GameObject*> SpawnerManager::getPool() const
{
	return pool;
}

void SpawnerManager::setGenerationTime(float generationTime)
{
	this->generationTime = generationTime;
}

float SpawnerManager::getGenerationTime() const
{
	return generationTime;
}

float SpawnerManager::getMinimumTime() const
{
	return minimumTime;
}