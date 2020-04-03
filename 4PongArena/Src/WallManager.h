#pragma once
#ifndef WALL_MANAGER_H
#define WALL_MANAGER_H

#include <UserComponent.h>

class GameObject;
class RigidBody;
class Health;


class WallManager :
	public UserComponent
{
private:
	Health* health;
	Vector3 OriginalPosition;
	  bool wall=false;

	  RigidBody* rigidBody;


	  std::string wallMeshId, wallMeshName;
	  Vector3 wallScale = { 1,1,1 };
	  std::string aliveMeshId, aliveMeshName;
	  Vector3 aliveScale;
public:
	WallManager(GameObject* gameObject);

	virtual void start();
	virtual void update(float deltaTime);
	virtual void handleData(ComponentData* data);
	void changeShapeToWall();

	bool IsWall();
	Health* GetHealth();
};


#endif