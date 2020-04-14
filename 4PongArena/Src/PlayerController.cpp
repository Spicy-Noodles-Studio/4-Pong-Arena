#include "PlayerController.h"
#include <ComponentRegister.h>
#include <InputSystem.h>
#include <GameObject.h>
#include <sstream>

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

		if (prop.first == "id") {
			if (!(ss >> player.id))
				LOG("PLAYER CONTROLLER: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else if (prop.first == "index") {
			if (!(ss >> player.index))
				LOG("PLAYER CONTROLLER: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else
			LOG("PLAYER CONTROLLER: Invalid property name \"%s\"", prop.first.c_str());
	}
}

void PlayerController::setPlayer(int id, int index)
{
	player.id = id;
	player.index = index;
}

Player PlayerController::getPlayer() const
{
	return player;
}

void PlayerController::checkInput() const
{
	if (movement == nullptr) return;

	Vector3 normal = movement->getNormal();
	Vector3 motionDirection(-normal.z, 0.0, normal.x);
	Vector3 directionMask = Vector3(abs(-normal.z), 0.0, abs(normal.x)).normalized();
	Vector3 axis = getInputAxis();
	axis *= directionMask; axis *= motionDirection;

	if (axis.x > 0.0) movement->moveRight();
	else if (axis.x < 0.0) movement->moveLeft();
	else if (axis.z > 0.0) movement->moveRight();
	else if (axis.z < 0.0) movement->moveLeft();
	else movement->stop();
}

Vector3 PlayerController::getInputAxis() const
{
	if (player.index < 0 || player.index > 4) return Vector3::ZERO;

	if (player.index == 4)
		return getKeyboardAxis();
	
	return getControllerAxis();
}

Vector3 PlayerController::getKeyboardAxis() const
{
	Vector3 axis = Vector3::ZERO;
	if (inputSystem->isKeyPressed("A")) axis.x += -1;	// Left
	if (inputSystem->isKeyPressed("D")) axis.x += 1;	// Right
	if (inputSystem->isKeyPressed("W")) axis.z += -1;	// Up
	if (inputSystem->isKeyPressed("S")) axis.z += 1;	// Down

	return axis;
}

Vector3 PlayerController::getControllerAxis() const
{
	Vector3 axis = Vector3::ZERO;
	std::pair<int, int> leftJoystick = inputSystem->getLeftJoystick(player.index);
	if (leftJoystick.first < 0 || inputSystem->isButtonPressed(player.index, "Left")) axis.x += -1;	// Left
	if (leftJoystick.first > 0 || inputSystem->isButtonPressed(player.index, "Right")) axis.x += 1;	// Right
	if (leftJoystick.second < 0 || inputSystem->isButtonPressed(player.index, "Up")) axis.z += -1;	// Up
	if (leftJoystick.second > 0 || inputSystem->isButtonPressed(player.index, "Down")) axis.z += 1;	// Down

	return axis;
}