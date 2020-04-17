#include "Ball.h"
#include <GameObject.h>
#include <RigidBody.h>
#include <sstream>

#include <ComponentRegister.h>

REGISTER_FACTORY(Ball);

Ball::Ball(GameObject* gameObject): UserComponent(gameObject), rigidBody(nullptr), velocity(0.0f)
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
	if (rigidBody != nullptr)
		rigidBody->setLinearVelocity(rigidBody->getLinearVelocity().normalized() * velocity);
}

void Ball::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "velocity")
		{
			if (!(ss >> velocity))
				LOG("BALL: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else
			LOG("BALL: Invalid property name \"%s\"", prop.first.c_str());
	}
}

float Ball::getVelocity()
{
	return velocity;
}

void Ball::setVelocity(float velocity)
{
	this->velocity = velocity;
}