#include "PlayerController.h"
#include <InputSystem.h>
#include <sstream>

PlayerController::PlayerController(GameObject* gameObject) : UserComponent(gameObject)
{

}

void PlayerController::start()
{
	rigidBody = gameObject->getComponent<RigidBody>();
}

void PlayerController::update(float deltaTime)
{
	UserComponent::update(deltaTime);

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
		if (player.id == 1 || player.id == 4)
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

void PlayerController::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "force")
		{
			ss >> force;
		}
		if (prop.first == "id")
		{
			ss >> player.id;
		}
		if (prop.first == "index")
		{
			ss >> player.index;
		}
	}
}