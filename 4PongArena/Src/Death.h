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
	RigidBody* rigidBody;
	Health* health;

	Vector3 initialPosition;

	std::string wallMeshId, wallMeshName;
	Vector3 playerColour;
	Vector3 wallColourD;
	Vector3 wallColourE;
	Vector3 neonColourD;
	Vector3 neonColourE;

	MeshRenderer* meshRenderer;
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

	void setPlayerColour(Vector3 colour);
	void setwallColours(Vector3 colourWall, Vector3 colourWallEmissive, Vector3 colourNeon, Vector3 colourNeonEmissive);
	void setWallScale(const Vector3& wallScale);
};

#endif