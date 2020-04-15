#include "Ball.h"
#include <ComponentRegister.h>
#include <GameObject.h>
#include <RigidBody.h>

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
	//rigidBody->setMovementConstraints(Vector3::UP); // ALVARO, SIGO SIN ENTENDER LOS "CONSTRAINTS"
}

void Ball::update(float deltaTime)
{ 
	rigidBody->setLinearVelocity(rigidBody->getLinearVelocity().normalized() * velocity);
}

float Ball::getVelocity()
{
	return velocity;
}

void Ball::setVelocity(float velocity)
{
	this->velocity = velocity;
}
