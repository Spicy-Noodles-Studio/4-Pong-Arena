#pragma once
#ifndef WALL_MANAGER_H
#define WALL_MANAGER_H

#include <UserComponent.h>

class GameObject;
class RigidBody;
class Health;

class WallManager : public UserComponent
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
	WallManager(GameObject* gameObject);
	virtual ~WallManager();

	virtual void start();
	virtual void update(float deltaTime);
	virtual void handleData(ComponentData* data);

private:
	void changeShapeToWall();
};

#endif