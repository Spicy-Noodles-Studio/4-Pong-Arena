#pragma once
#ifndef DEATH_H
#define DEATH_H

#include <UserComponent.h>

class GameObject;
class RigidBody;
class Health;

class Death : public UserComponent
{
private:
	GameObject* goal;
	RigidBody* rigidBody;
	Health* health;

	Vector3 initialPosition;
	float goalOffset;

	std::string wallMeshId, wallMeshName;
	Vector3 wallScale;
	std::string aliveMeshId, aliveMeshName;
	Vector3 aliveScale;

public:
	Death(GameObject* gameObject);
	virtual ~Death();

	virtual void start();
	virtual void update(float deltaTime);
	virtual void handleData(ComponentData* data);

private:
	void die();
};

#endif