#include "Ball.h"
#include <GameObject.h>
#include <RigidBody.h>
#include <sstream>

#include <ComponentRegister.h>

REGISTER_FACTORY(Ball);

Ball::Ball(GameObject* gameObject): UserComponent(gameObject), rigidBody(nullptr), maxVelocity(0.0f)
{

}

Ball::~Ball()
{

}

void Ball::start()
{
	rigidBody = gameObject->getComponent<RigidBody>();
}

void Ball::update(float deltaTime)
{ 
	if (rigidBody != nullptr && rigidBody->getLinearVelocity().magnitude() > maxVelocity)
		rigidBody->setLinearVelocity(rigidBody->getLinearVelocity().normalized() * maxVelocity);
}

void Ball::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "maxVelocity")
		{
			if (!(ss >> maxVelocity))
				LOG("BALL: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else
			LOG("BALL: Invalid property name \"%s\"", prop.first.c_str());
	}
}