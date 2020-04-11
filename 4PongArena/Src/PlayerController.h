#pragma once
#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

#include <UserComponent.h>

class GameObject;
class RigidBody;
class Health;

struct Player
{
	int id; // Jugador, de 1 a 4
	int index; // Teclado = -1, mandos de 0 a 3
};

class PlayerController : public UserComponent
{
private:
	RigidBody* rigidBody;
	Health* health;

	Player player;
	int force;

public:
	PlayerController(GameObject* gameObject);
	virtual ~PlayerController();

	virtual void start();
	virtual void update(float deltaTime);
	virtual void handleData(ComponentData* data);

	int getPlayerId();
};

#endif