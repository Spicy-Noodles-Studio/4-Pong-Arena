#pragma once
#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

#include <UserComponent.h>


class WallManager;
class GameObject;
class RigidBody;

struct Player
{
	int id; // Jugador, de 1 a 4
	int index; // Teclado = -1, mandos de 0 a 3
};

class PlayerController : public UserComponent
{
private:
	Player player;

	RigidBody* rigidBody;
	int force;
	Vector3 OriginalPosition;
	Health* health;
	bool wall;

	WallManager* wall;

	WallManager* wall;

public:
	PlayerController(GameObject* gameObject);

	virtual void start();
	virtual void update(float deltaTime);
	virtual void handleData(ComponentData* data);

};

#endif