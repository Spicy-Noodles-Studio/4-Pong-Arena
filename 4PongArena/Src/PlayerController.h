#pragma once
#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

#include <UserComponent.h>
#include "Player.h"

class GameObject;
class RigidBody;
class Health;

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

	void setPlayer(int id, int index);
	Player getPlayer() const;
};

#endif