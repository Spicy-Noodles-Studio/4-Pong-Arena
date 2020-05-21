#include "PlayerController.h"
#include <InputSystem.h>
#include <GameObject.h>
#include <sstream>

#include "Movement.h"
#include <Trail.h>
#include <ComponentRegister.h>
#include <SoundEmitter.h>

REGISTER_FACTORY(PlayerController);

PlayerController::PlayerController(GameObject* gameObject) : UserComponent(gameObject), inputSystem(nullptr), movement(nullptr), controllerIndex(-1)
{

}

PlayerController::~PlayerController()
{

}

void PlayerController::start()
{
	if (gameObject != nullptr) {
		inputSystem = InputSystem::GetInstance();
		movement = gameObject->getComponent<Movement>();
		soundEmitter = gameObject->getComponent<SoundEmitter>();
		if (soundEmitter != nullptr) soundEmitter->setVolume(0.8);
		gameObject->getComponent<Trail>()->start();
	}
	moving = false;
	hasMoved = false;
}

void PlayerController::update(float deltaTime)
{
	checkInput();
	if (hasMoved && !moving) {
		moving = true;
		if (soundEmitter != nullptr) soundEmitter->playSound("Paddle_Move");
	}
	else if (moving && !hasMoved) moving = false;
}

void PlayerController::handleData(ComponentData* data)
{
	if (data == nullptr) return;

	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "index")
		{
			if (!(ss >> controllerIndex))
				LOG("PLAYER CONTROLLER: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else
			LOG("PLAYER CONTROLLER: Invalid property name \"%s\"", prop.first.c_str());
	}
}

void PlayerController::setIndex(int index)
{
	controllerIndex = index;
}

int PlayerController::getIndex() const
{
	return controllerIndex;
}

void PlayerController::checkInput()
{
	if (movement == nullptr) return;

	Vector3 normal = movement->getNormal();
	Vector3 motionDirection(-normal.z, 0.0, normal.x);
	Vector3 directionMask = Vector3(abs(-normal.z), 0.0, abs(normal.x)).normalized();
	Vector3 axis = getInputAxis();
	axis *= directionMask; axis *= motionDirection;

	if (axis.x > 0.0) { movement->moveRight(); hasMoved = true; }
	else if (axis.x < 0.0) { movement->moveLeft(); hasMoved = true; }
	else if (axis.z > 0.0) { movement->moveRight(); hasMoved = true; }
	else if (axis.z < 0.0) { movement->moveLeft(); hasMoved = true; }
	else { movement->stop(); hasMoved = false; }
}

Vector3 PlayerController::getInputAxis() const
{
	if (controllerIndex < 0 || controllerIndex > 4) return Vector3::ZERO;

	if (controllerIndex == 4)
		return getKeyboardAxis();

	return getControllerAxis();
}

Vector3 PlayerController::getKeyboardAxis() const
{
	Vector3 axis = Vector3::ZERO;
	if (inputSystem != nullptr) {
		if (inputSystem->isKeyPressed("A")) axis.x += -1;	// Left
		if (inputSystem->isKeyPressed("D")) axis.x += 1;	// Right
		if (inputSystem->isKeyPressed("W")) axis.z += -1;	// Up
		if (inputSystem->isKeyPressed("S")) axis.z += 1;	// Down
	}

	return axis;
}

Vector3 PlayerController::getControllerAxis() const
{
	Vector3 axis = Vector3::ZERO;
	if (inputSystem != nullptr) {
		std::pair<int, int> leftJoystick = inputSystem->getLeftJoystick(controllerIndex);
		if (leftJoystick.first < 0 || inputSystem->isButtonPressed(controllerIndex, "Left")) axis.x += -1;	// Left
		if (leftJoystick.first > 0 || inputSystem->isButtonPressed(controllerIndex, "Right")) axis.x += 1;	// Right
		if (leftJoystick.second < 0 || inputSystem->isButtonPressed(controllerIndex, "Up")) axis.z += -1;	// Up
		if (leftJoystick.second > 0 || inputSystem->isButtonPressed(controllerIndex, "Down")) axis.z += 1;	// Down
	}

	return axis;
}