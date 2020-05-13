#pragma once
#ifndef DEATH_H
#define DEATH_H

#include <UserComponent.h>

class RigidBody;
class Health;
class Score;
class GameManager;
class MeshRenderer;
class SoundEmitter;

class Death : public UserComponent
{
private:
	RigidBody* rigidBody;
	Health* health;
	SoundEmitter* soundEmitter;

	Vector3 initialPosition;

	std::string wallMeshId, wallMeshName;
	Vector3 playerColour;
	std::pair<Vector3, Vector3> baseColour;
	std::pair<Vector3, Vector3> neonColour;

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

	void setPlayerColour(const Vector3& colour);
	void setwallColours(const std::pair<Vector3, Vector3>& baseColour, const std::pair<Vector3, Vector3>& neonColour);
	void setWallScale(const Vector3& wallScale);
};

#endif