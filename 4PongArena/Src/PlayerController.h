#pragma once
#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

#include <UserComponent.h>

class InputSystem;
class Movement;

class PlayerController : public UserComponent
{
private:
	InputSystem* inputSystem;
	Movement* movement;

	int controllerIndex;

public:
	PlayerController(GameObject* gameObject);
	virtual ~PlayerController();

	virtual void start();
	virtual void update(float deltaTime);
	virtual void handleData(ComponentData* data);

	void setIndex(int index);
	int getIndex() const;

private:
	void checkInput() const;
	Vector3 getInputAxis() const;
	Vector3 getKeyboardAxis() const;
	Vector3 getControllerAxis() const;
};

#endif