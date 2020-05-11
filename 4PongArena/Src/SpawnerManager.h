#pragma once
#ifndef SPAWNER_MANAGER_H
#define SPAWNER_MANAGER_H

#include <UserComponent.h>

class Countdown;

class SpawnerManager : public UserComponent
{
private:
	std::vector<GameObject*> spawners;
	std::vector<GameObject*> pool;

	Countdown* countdown;

	float time;
	float generationTime;
	float minimumTime;

	int lastUsed;

	int chooseSpawn();
	GameObject* getBall();

public:
	SpawnerManager(GameObject* gameObject);
	virtual ~SpawnerManager();

	virtual void start();
	virtual void update(float deltaTime);
	virtual void handleData(ComponentData* data);

	void setSpawners(std::vector<GameObject*>& spawners);
	std::vector<GameObject*> getPool() const;

	void setGenerationTime(float generationTime);
	float getGenerationTime() const;

	float getMinimumTime() const;
};

#endif