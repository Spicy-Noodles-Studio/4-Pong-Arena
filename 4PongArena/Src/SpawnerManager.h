#pragma once
#ifndef SPAWNER_MANAGER_H
#define SPAWNER_MANAGER_H

#include <UserComponent.h>

class Game;
class Countdown;

class SpawnerManager : public UserComponent
{
private:
	std::vector<GameObject*> spawners;
	std::vector<GameObject*> pool;

	Game* game;
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


	void setSpawners(std::vector<GameObject*>& spawners);
	std::vector<GameObject*> getPool() const;

	void deactivateAll();

	void setGenerationTime(float generationTime);
	float getGenerationTime() const;

	float getMinimumTime() const;

protected:
	virtual void start();
	virtual void update(float deltaTime);
	virtual void handleData(ComponentData* data);

};

#endif