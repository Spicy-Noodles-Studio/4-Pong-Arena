#include "PlayerController.h"
#include <InputSystem.h>
#include <GameObject.h>
#include <sstream>

#include "Movement.h"
#include <Trail.h>
#include <ComponentRegister.h>
#include <SoundEmitter.h>

REGISTER_FACTORY(PlayerController);

PlayerController::PlayerController(GameObject* gameObject) : UserComponent(gameObject), inputSystem(nullptr), movement(nullptr), soundEmitter(nullptr), controllerIndex(-1), hasMoved(false), moving(false)
{

}

PlayerController::~PlayerController()
{
	inputSystem = nullptr;
	soundEmitter = nullptr;
	movement = nullptr;
}

void PlayerController::start()
{
	checkNullAndBreak(gameObject);

	inputSystem = InputSystem::GetInstance();
	checkNull(inputSystem);

	movement = gameObject->getComponent<Movement>();
	soundEmitter = gameObject->getComponent<SoundEmitter>();
	Trail* trail = gameObject->getComponent<Trail>();
	if (notNull(soundEmitter)) soundEmitter->setVolume(0.8);
	if (notNull(trail)) trail->start();

	moving = false;
	hasMoved = false;
}

void PlayerController::update(float deltaTime)
{
	checkInput();
	if (hasMoved && !moving)
		moving = true;
	else if (moving && !hasMoved)
		moving = false;
}

void PlayerController::handleData(ComponentData* data)
{
	checkNullAndBreak(data);

	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "index")
		{
			setInt(controllerIndex);
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
	checkNullAndBreak(movement);

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
	if (notNull(inputSystem)) {
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
	if (notNull(inputSystem)) {
		std::pair<int, int> leftJoystick = inputSystem->getLeftJoystick(controllerIndex);
		if (leftJoystick.first < 0 || inputSystem->isButtonPressed(controllerIndex, "Left")) axis.x += -1;	// Left
		if (leftJoystick.first > 0 || inputSystem->isButtonPressed(controllerIndex, "Right")) axis.x += 1;	// Right
		if (leftJoystick.second < 0 || inputSystem->isButtonPressed(controllerIndex, "Up")) axis.z += -1;	// Up
		if (leftJoystick.second > 0 || inputSystem->isButtonPressed(controllerIndex, "Down")) axis.z += 1;	// Down
	}

	return axis;
}