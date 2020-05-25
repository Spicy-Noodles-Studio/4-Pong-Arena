#include "Movement.h"
#include <GameObject.h>
#include <RigidBody.h>
#include <sstream>

#include <ComponentRegister.h>

REGISTER_FACTORY(Movement)

Movement::Movement(GameObject* gameObject) : UserComponent(gameObject), rigidBody(nullptr), direction(Vector3::ZERO), normal(Vector3::FORWARD), velocity(0.0f)
{

}

Movement::~Movement()
{

}

void Movement::start()
{
	if (gameObject != nullptr) {
		rigidBody = gameObject->getComponent<RigidBody>();
		if (gameObject->transform != nullptr) normal = Vector3::ZERO - gameObject->transform->getPosition();
		normal *= Vector3(1.0, 0.0, 1.0);
		normal.normalize();
	}
	if (rigidBody != nullptr) {
		// Cancel rotations and translations through normal vector
		rigidBody->setRotationConstraints(Vector3::ZERO);
		rigidBody->setMovementConstraints(Vector3(abs(normal.z), 0.0, abs(normal.x)));
	}
}

void Movement::update(float deltaTime)
{
	move();
}

void Movement::handleData(ComponentData* data)
{
	if (data == nullptr) return;

	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "velocity")
		{
			if (!(ss >> velocity))
				LOG("MOVEMENT: Invalid value for property with name \"%s\"", prop.first.c_str());
		}
		else
			LOG("MOVEMENT: Invalid property name \"%s\"", prop.first.c_str());
	}
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
	if (rigidBody != nullptr) rigidBody->setLinearVelocity(direction.normalized() * velocity);
}

void Movement::setNormal(const Vector3& normal)
{
	if (this != nullptr) this->normal = normal;
}

const Vector3& Movement::getNormal() const
{
	return normal;
}

void Movement::setVelocity(float velocity)
{
	if (this != nullptr) this->velocity = velocity;
}

float Movement::getVelocity() const
{
	return velocity;
}

const Vector3 Movement::getRight() const
{
	Vector3 right = normal.normalized();
	right.x = -normal.z;
	right.z = normal.x;
	return right;
}

const Vector3 Movement::getLeft() const
{
	Vector3 left = normal.normalized();
	left.x = normal.z;
	left.z = -normal.x;
	return left;
}