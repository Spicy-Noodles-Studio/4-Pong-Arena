#include "Movement.h"
#include <ComponentRegister.h>
#include <GameObject.h>
#include <RigidBody.h>

REGISTER_FACTORY(Movement)

Movement::Movement(GameObject* gameObject) : UserComponent(gameObject), rigidBody(nullptr), velocity(0.0f), direction(Vector3::ZERO), normal(Vector3::FORWARD)
{

}

Movement::~Movement()
{

}

void Movement::start()
{
	rigidBody = gameObject->getComponent<RigidBody>();
	velocity = 5.0f; // TODO: pasarlo por el handleData
	normal = Vector3::ZERO - gameObject->transform->getPosition();
	normal *= Vector3(1.0, 0.0, 1.0);
	normal.normalize();
	// Cancel rotations and translations through normal vector
	rigidBody->setRotationConstraints(Vector3::ZERO);
	rigidBody->setMovementConstraints(Vector3(abs(normal.x), 1.0, abs(normal.z)));
}

void Movement::update(float deltaTime)
{
	move();
}

void Movement::moveRight()
{
	direction = getRight();
}

void Movement::moveLeft()
{
	direction = getLeft();
}

void Movement::stop()
{
	direction = Vector3::ZERO;
}

void Movement::move()
{
	rigidBody->setLinearVelocity(direction.normalized() * velocity);
}

void Movement::setNormal(const Vector3& normal)
{
	this->normal = normal;
}

const Vector3& Movement::getNormal() const
{
	return normal;
}

void Movement::setVelocity(float velocity)
{
	this->velocity = velocity;
}

float Movement::getVelocity() const
{
	return velocity;
}

const Vector3 Movement::getRight() const
{
	Vector3 right = normal;
	right.rotateAroundAxis(Vector3::UP, -90);
	return right;
}

const Vector3 Movement::getLeft() const
{
	Vector3 left = normal;
	left.rotateAroundAxis(Vector3::UP, 90);
	return left;
}
