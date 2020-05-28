#include "SpawnerManager.h"
#include <GameObject.h>
#include <MeshRenderer.h>
#include <RigidBody.h>
#include <MathUtils.h>
#include <sstream>

#include "Spawner.h"
#include "Game.h"
#include "Countdown.h"
#include "ParticleManager.h"

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
		if (notNull(pool[i]) && !pool[i]->isActive())
		{
			found = true;
			ball = pool[i];

			if (notNull(ball)) {
				ball->setActive(true);
				MeshRenderer* ballRenderer = ball->getComponent<MeshRenderer>();
				if (notNull(ballRenderer)) ballRenderer->setVisible(true);
			}
		}
		else
			i++;
	}

	if (i >= pool.size())
	{
		ball = instantiate("Ball");
		if (notNull(ball)) pool.push_back(ball);
	}

	return ball;
}

SpawnerManager::SpawnerManager(GameObject* gameObject) : UserComponent(gameObject), countdown(nullptr), game(nullptr), time(0.0f), generationTime(4.0f), minimumTime(1.0f), lastUsed(0)
{

}

SpawnerManager::~SpawnerManager()
{
	game = nullptr;
	countdown = nullptr;
	spawners.clear();
	pool.clear();
}

void SpawnerManager::start()
{
	GameObject* object = findGameObjectWithName("Game");
	GameObject* countdownObject = findGameObjectWithName("Countdown");
	if (notNull(object)) game = object->getComponent<Game>();
	if (notNull(countdownObject)) countdown = countdownObject->getComponent<Countdown>();

	for (int i = 0; i < 25; i++)
	{
		GameObject* ball = instantiate("Ball");
		if (notNull(ball)) {
			ball->setActive(false);
			MeshRenderer* ballRenderer = ball->getComponent<MeshRenderer>();
			if (notNull(ballRenderer))
				ballRenderer->setVisible(false);
			pool.push_back(ball);
		}
	}

	time = generationTime;
}

void SpawnerManager::update(float deltaTime)
{
	if (notNull(countdown) && !countdown->isCounting() && notNull(game) && game->getTime() != 0)
	{
		if (time > 0)
			time -= deltaTime;
		else
		{
			int index = chooseSpawn();
			if (index > 0 && index < spawners.size()) {
				Spawner* spawner = spawners[index]->getComponent<Spawner>();
				if (notNull(spawner))
					spawner->shoot(getBall());
			}
			time = generationTime;
		}
	}
}

void SpawnerManager::handleData(ComponentData* data)
{
	checkNullAndBreak(data);

	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "generationTime")
		{
			setFloat(generationTime);
		}
		else if (prop.first == "minimumTime")
		{
			setFloat(minimumTime);
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

void SpawnerManager::deactivateAll()
{
	for (int i = 0; i < pool.size(); i++)
	{
		if (pool[i] != nullptr && pool[i]->isActive())
		{
			pool[i]->setActive(false);

			ParticleManager* particleManager = pool[i]->getComponent<ParticleManager>();
			if (notNull(particleManager)) particleManager->stop();

			MeshRenderer* renderer = pool[i]->getComponent<MeshRenderer>();
			if (notNull(renderer)) renderer->setVisible(false);
		}
	}
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