#include "PlayerController.h"
#include <sstream>
#include <InputSystem.h>
#include <ComponentRegister.h>
#include <GameObject.h>

#include "Movement.h"

REGISTER_FACTORY(PlayerController);

PlayerController::PlayerController(GameObject* gameObject) : UserComponent(gameObject), inputSystem(nullptr), movement(nullptr), player()
{

}

PlayerController::~PlayerController()
{
}

void PlayerController::start()
{
	inputSystem = InputSystem::GetInstance();
	movement = gameObject->getComponent<Movement>();
}

void PlayerController::update(float deltaTime)
{
	checkInput();
}

void PlayerController::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "id")
		{
			ss >> player.id;
		}
		else if (prop.first == "index")
		{
			ss >> player.index;
		}
		else
			LOG("PlayerController: Invalid property name \"%s\"", prop.first.c_str());
	}
}

void PlayerController::checkInput() const
{
	if (movement == nullptr) return;

	bool leftPressed = false;
	bool rightPressed = false;

	if (player.index == 4) checkKeyboard(leftPressed, rightPressed);
	else if (player.index >= 0 && player.index <= 3) checkController(leftPressed, rightPressed);
	else return;

	//If both are pressed or none of them are, then stop
	if ((rightPressed && leftPressed) || (!rightPressed && !leftPressed)) {
		movement->stop(); 
		return;
	}

	if (rightPressed) movement->moveRight();
	else if (leftPressed) movement->moveLeft();
}

void PlayerController::checkKeyboard(bool& left, bool& right) const
{
	if (player.index != 4) return;
	
	// Horizontal axis or vertical axis
	std::string lLetter = player.id % 2 == 1 ? player.id / 2 == 0 ? "A" : "D" : player.id / 2 == 1 ? "S" : "W";
	std::string rLetter = player.id % 2 == 1 ? player.id / 2 == 0 ? "D" : "A" : player.id / 2 == 1 ? "W" : "S";
	
	left = inputSystem->isKeyPressed(lLetter);
	right = inputSystem->isKeyPressed(rLetter);
}

void PlayerController::checkController(bool& left, bool& right) const
{
	if (player.index < 0 || player.index > 3) return;

	std::pair<int, int> leftJoystick = inputSystem->getLeftJoystick(player.index);
	
	left =	player.id % 2 == 1 ?
			player.id / 2 == 0 ?	leftJoystick.first < 0 || inputSystem->isButtonPressed(player.index, "Left") :
									leftJoystick.first > 0 || inputSystem->isButtonPressed(player.index, "Right") :
			player.id / 2 == 1 ?	leftJoystick.second > 0 || inputSystem->isButtonPressed(player.index, "Down") :
									leftJoystick.second < 0 || inputSystem->isButtonPressed(player.index, "Up");

	right =	player.id % 2 == 1 ?
			player.id / 2 == 0 ?	leftJoystick.first > 0 || inputSystem->isButtonPressed(player.index, "Right") :
									leftJoystick.first < 0 || inputSystem->isButtonPressed(player.index, "Left") :
			player.id / 2 == 1 ?	leftJoystick.second < 0 || inputSystem->isButtonPressed(player.index, "Up") :
									leftJoystick.second > 0 || inputSystem->isButtonPressed(player.index, "Down");
}