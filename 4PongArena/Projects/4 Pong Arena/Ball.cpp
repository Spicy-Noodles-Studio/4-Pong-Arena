#include "Ball.h"
#include <ComponentRegister.h>
#include <GameObject.h>
#include <RigidBody.h>

REGISTER_FACTORY(Ball);

Ball::Ball(GameObject* gameObject): UserComponent(gameObject), rigidBody(nullptr), velocity(0.0f), direction(Vector3::ZERO)
{

}

Ball::~Ball()
{

}

void Ball::start()
{
	rigidBody = gameObject->getComponent<RigidBody>();
	rigidBody->setMovementConstraints(Vector3::IDENTITY - Vector3::UP);
}

void Ball::update(float deltaTime)
{
	if (rigidBody->getLinearVelocity().normalized() != direction)
		rigidBody->setLinearVelocity(direction.normalized() * velocity);
}

float Ball::getVelocity()
{
	return velocity;
}

void Ball::setVelocity(float velocity)
{
	this->velocity = velocity;
}

Vector3 Ball::getDirection()
{
	return direction;
}

void Ball::setDirection(const Vector3& direction)
{
	this->direction = direction;
}
