#pragma once
#ifndef DEATH_H
#define DEATH_H

#include <UserComponent.h>

class RigidBody;
class Health;
class Score;
class GameManager;
class SoundEmitter;

class Death : public UserComponent
{
private:
	RigidBody* rigidBody;
	Health* health;
	SoundEmitter* soundEmitter;

	Vector3 initialPosition;

	std::string wallMeshId, wallMeshName;
	Vector3 wallScale;
	Score* scores;
	GameManager* manager;
	int id;

	void die();

public:
	Death(GameObject* gameObject);
	virtual ~Death();

	virtual void start();
	virtual void update(float deltaTime);
	virtual void handleData(ComponentData* data);
};

#endif