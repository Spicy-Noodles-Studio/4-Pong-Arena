#pragma once
#ifndef DEATH_H
#define DEATH_H

#include <UserComponent.h>

class RigidBody;
class Health;
class Score;
class GameManager;
class MeshRenderer;

class Death : public UserComponent
{
private:
	GameManager* gameManager;

	MeshRenderer* meshRenderer;
	RigidBody* rigidBody;
	Health* health;
	Score* scores;

	Vector3 initialPosition;

	std::string wallMeshId, wallMeshName;
	Vector3 wallScale;

	Vector3 playerColour;

	int id;

	void die();

public:
	Death(GameObject* gameObject);
	virtual ~Death();

	virtual void start();
	virtual void update(float deltaTime);
	virtual void handleData(ComponentData* data);

	void setPlayerColour(Vector3 colour);
};

#endif