#include "PlayerController.h"
#include <InputSystem.h>
#include <GameObject.h>
#include <RigidBody.h>
#include <sstream>

#include "Health.h"
#include "ComponentRegister.h"

REGISTER_FACTORY(PlayerController);

PlayerController::PlayerController(GameObject* gameObject) : UserComponent(gameObject)
{

}

PlayerController::~PlayerController()
{

}

void PlayerController::start()
{
	rigidBody = gameObject->getComponent<RigidBody>();
	rigidBody->setMovementConstraints(Vector3(1, 0, 1));
	rigidBody->setRotationConstraints(Vector3(0, 0, 0));

	health = gameObject->getComponent<Health>();
}

void PlayerController::update(float deltaTime)
{
	if (health != nullptr && health->isAlive())
	{
		Vector3 dir = Vector3(0, 0, 0);

		if (player.index == -1)
		{
			if (player.id == 1 || player.id == 3)
			{
				if (InputSystem::GetInstance()->isKeyPressed("A"))
					dir = Vector3(-1, 0, 0);
				else if (InputSystem::GetInstance()->isKeyPressed("D"))
					dir = Vector3(1, 0, 0);
			}
			else
			{
				if (InputSystem::GetInstance()->isKeyPressed("W"))
					dir = Vector3(0, 0, -1);
				else if (InputSystem::GetInstance()->isKeyPressed("S"))
					dir = Vector3(0, 0, 1);
			}
		}
		else
		{
			if (player.id == 1 || player.id == 3)
			{
				if (InputSystem::GetInstance()->getLeftJoystick(player.index).first < 0 || InputSystem::GetInstance()->isButtonPressed(player.index, "Left"))
					dir = Vector3(-1, 0, 0);
				else if (InputSystem::GetInstance()->getLeftJoystick(player.index).first > 0 || InputSystem::GetInstance()->isButtonPressed(player.index, "Right"))
					dir = Vector3(1, 0, 0);
			}
			else
			{
				if (InputSystem::GetInstance()->getLeftJoystick(player.index).second > 0 || InputSystem::GetInstance()->isButtonPressed(player.index, "Up"))
					dir = Vector3(0, 0, -1);
				else if (InputSystem::GetInstance()->getLeftJoystick(player.index).second < 0 || InputSystem::GetInstance()->isButtonPressed(player.index, "Down"))
					dir = Vector3(0, 0, 1);
			}
		}
		rigidBody->addForce(dir * force);
	}
}

void PlayerController::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "force")
		{
			if (!(ss >> force))
				LOG("PLAYER CONTROLLER: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else if (prop.first == "id")
		{
			if (!(ss >> player.id))
				LOG("PLAYER CONTROLLER: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else if (prop.first == "index")
		{
			if (!(ss >> player.index))
				LOG("PLAYER CONTROLLER: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else
			LOG("PLAYER CONTROLLER: Invalid property name \"%s\"", prop.first.c_str());
	}
}

int PlayerController::getPlayerId()
{
	return player.id;
}