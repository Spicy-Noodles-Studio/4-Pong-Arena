#pragma once
#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

#include <UserComponent.h>
#include "Player.h"

class InputSystem;
class Movement;

class PlayerController : public UserComponent
{
private:
	InputSystem* inputSystem;
	Movement* movement;
	Player player;

public:
	PlayerController(GameObject* gameObject);
	virtual ~PlayerController();

	virtual void start();
	virtual void update(float deltaTime);
	virtual void handleData(ComponentData* data);

	void setPlayer(int id, int index);
	Player getPlayer() const;

private:
	void checkInput() const;
	Vector3 getInputAxis() const;
	Vector3 getKeyboardAxis() const;
	Vector3 getControllerAxis() const;
};

#endif