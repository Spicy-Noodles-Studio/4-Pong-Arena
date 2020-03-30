#include "PlayerController.h"
#include <InputSystem.h>
#include <sstream>
#include <Scene.h>
#include <SceneManager.h>
PlayerController::PlayerController(GameObject* gameObject) : UserComponent(gameObject)
{

}

void PlayerController::start()
{
	rigidBody = gameObject->getComponent<RigidBody>();
}

void PlayerController::update(float deltaTime)
{
	if (health==nullptr)
	{
		Vector3 pos = Vector3(0, 0, 1);
		if (player.id == 1)
			pos = gameObject->transform->getPosition() + Vector3(0, 0, 1);
		else if (player.id == 2)
			pos = gameObject->transform->getPosition() + Vector3(1, 0, 0);
		else if (player.id == 3)
			pos = gameObject->transform->getPosition() + Vector3(0, 0, -1);
		else if (player.id == 4)
			pos = gameObject->transform->getPosition() + Vector3(-1, 0, 0);

		GameObject* sensor=instantiate("Sensor", pos);

		health = sensor->getComponent<Health>();
		if (player.id == 1 || player.id == 3) {
			if (health != nullptr)
				sensor->getComponent<RigidBody>()->multiplyScale(Vector3(health->getTriggerSize(), 1, 1));
		}
		else {
			if (health != nullptr)
				sensor->getComponent<RigidBody>()->multiplyScale(Vector3(1, 1, health->getTriggerSize()));
		}

		OriginalPosition = gameObject->transform->getPosition();

		//health = sensor->getComponent<Health>();
	}
	UserComponent::update(deltaTime);

	if (health!=nullptr&&health->isAlive()&&!wall)
	{

		Vector3 dir = Vector3(0, 0, 0);

		if (player.index == -1)
		{
			rigidBody->setStatic(false);
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
	else if (!health->isAlive()&& !wall)
	{
		wall = true;
		changeShapeToWall();
		rigidBody->setLinearVelocity(Vector3(0,0,0));
		gameObject->transform->setPosition(OriginalPosition);
	}
	else
		rigidBody->setStatic(true);

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
		else if (prop.first == "id")
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

void PlayerController::changeShapeToWall()
{
	if (player.id == 1 || player.id == 3) 
	{
		if (health != nullptr)
		{
			gameObject->transform->setScale(Vector3(gameObject->transform->getScale().x * health->getTriggerSize(), gameObject->transform->getScale().y, gameObject->transform->getScale().z));
			rigidBody->multiplyScale(Vector3(health->getTriggerSize(), 1, 1));
		}
	}
	else
	{
		if (health != nullptr)
		{
			rigidBody->multiplyScale(Vector3(1, 1, health->getTriggerSize()));
			gameObject->transform->setScale(Vector3(gameObject->transform->getScale().x, gameObject->transform->getScale().y, gameObject->transform->getScale().z * health->getTriggerSize()));
		}
	}
}
