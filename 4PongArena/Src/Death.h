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
	GameManager* gameManager;

	MeshRenderer* meshRenderer;
	RigidBody* rigidBody;
	Health* health;
	Score* scores;
	SoundEmitter* soundEmitter;

	Vector3 initialPosition;

	std::string wallMeshId, wallMeshName;
	Vector3 wallScale;

	std::pair<Vector3, Vector3> baseColour;
	std::pair<Vector3, Vector3> neonColour;
	Vector3 playerColour;

	int id;

	void die();

public:
	Death(GameObject* gameObject);
	virtual ~Death();
	void setPlayerColour(const Vector3& colour);
	void setwallColours(const std::pair<Vector3, Vector3>& baseColour, const std::pair<Vector3, Vector3>& neonColour);
	void setWallScale(const Vector3& wallScale);

protected:
	virtual void start();
	virtual void update(float deltaTime);
	virtual void handleData(ComponentData* data);

};

#endif