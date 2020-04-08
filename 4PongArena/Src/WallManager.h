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
	RigidBody* rigidBody;
	Health* health;
	Vector3 initialPosition;

	std::string wallMeshId, wallMeshName;
	std::string aliveMeshId, aliveMeshName;
public:
	WallManager(GameObject* gameObject);
	~WallManager();

	virtual void start();
	virtual void update(float deltaTime);
	virtual void handleData(ComponentData* data);

	void changeShapeToWall();
};


#endif