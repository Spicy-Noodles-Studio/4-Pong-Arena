#include "Ball.h"
#include <GameObject.h>
#include <RigidBody.h>
#include <sstream>

#include <ComponentRegister.h>

REGISTER_FACTORY(Ball);

Ball::Ball(GameObject* gameObject): UserComponent(gameObject), rigidBody(nullptr), velocity(0.0f), targetVelocity(0.0f), acceleration(0.0f)
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
	if (velocity < targetVelocity)
		velocity += acceleration * deltaTime;
	else
		velocity = targetVelocity;

	if (rigidBody != nullptr)
		rigidBody->setLinearVelocity(rigidBody->getLinearVelocity().normalized() * velocity);
}

void Ball::setVelocity(float velocity)
{
	this->velocity = velocity;
}

void Ball::setTargetVelocity(float targetVelocity)
{
	this->targetVelocity = targetVelocity;
}

void Ball::setAcceleration(float acceleration)
{
	this->acceleration = acceleration;
}