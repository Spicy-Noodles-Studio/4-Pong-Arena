#include "Spawner.h"
#include <GameObject.h>
#include <RigidBody.h>
#include <MathUtils.h>
#include <sstream>

#include "Ball.h"

#include <ComponentRegister.h>

REGISTER_FACTORY(Spawner);

Spawner::Spawner(GameObject* gameObject) : UserComponent(gameObject), velocity(3.0f), angle(25)
{

}

Spawner::~Spawner()
{

}

void Spawner::start()
{
	Vector3 direction = Vector3::ZERO - gameObject->transform->getPosition();
	gameObject->transform->setWorldRotation(direction);
}

void Spawner::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "velocity")
		{
			if (!(ss >> velocity))
				LOG("SPAWNER: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else if (prop.first == "angle")
		{
			if (!(ss >> angle))
				LOG("SPAWNER: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else
			LOG("SPAWNER: Invalid property name \"%s\"", prop.first.c_str());
	}
}

void Spawner::shoot(GameObject* ball)
{
	Vector3 direction = Vector3::ZERO - gameObject->transform->getPosition();
	direction.rotateAroundAxis(Vector3::UP, random(-angle, angle));

	//gameObject->transform->setRotation(direction);

	direction.y = 0;

	

	if (ball != nullptr)
	{
		ball->transform->setPosition(gameObject->transform->getPosition() + direction.normalized());

		ball->getComponent<Ball>()->setVelocity(velocity);
		ball->getComponent<Ball>()->setTargetVelocity(velocity);

		ball->getComponent<RigidBody>()->setLinearVelocity(direction.normalized() * velocity);
	}
}