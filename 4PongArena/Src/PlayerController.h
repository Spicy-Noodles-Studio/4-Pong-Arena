#pragma once
#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

#include <UserComponent.h>

class InputSystem;
class Movement;

class PlayerController : public UserComponent
{
public:
	struct Player
	{
		int id; // Jugador, de 1 a 4
		int index; // Teclado = 4, mandos de 0 a 3
	};

private:
	Player player;
	InputSystem* inputSystem;
	Movement* movement;

public:
	PlayerController(GameObject* gameObject);
	~PlayerController();

	virtual void start();
	virtual void update(float deltaTime);
	virtual void handleData(ComponentData* data);

private:
	void checkInput() const;
	void checkKeyboard(bool& left, bool& right) const;
	void checkController(bool& left, bool& right) const;
};

#endif