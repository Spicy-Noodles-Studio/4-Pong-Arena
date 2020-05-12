#include "ForceField.h"
#include <GameObject.h>
#include <RigidBody.h>
#include <sstream>

#include "Ball.h"

#include <ComponentRegister.h>

REGISTER_FACTORY(ForceField);

ForceField::ForceField(GameObject* gameObject) : UserComponent(gameObject), targetVelocity(0.0f), acceleration(0.0f)
{

}

ForceField::~ForceField()
{

}

void ForceField::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "targetVelocity")
		{
			if (!(ss >> targetVelocity))
				LOG("FORCE FIELD: Invalid value for property with name \"%s\"", prop.first.c_str());
		}
		else if (prop.first == "acceleration")
		{
			if (!(ss >> acceleration))
				LOG("FORCE FIELD: Invalid value for property with name \"%s\"", prop.first.c_str());
		}
		else
			LOG("FORCE FIELD: Invalid property with name \"%s\"", prop.first.c_str());
	}
}

void ForceField::onObjectEnter(GameObject* other)
{
	RigidBody* rigidBody = other->getComponent<RigidBody>();
	Ball* ball = other->getComponent<Ball>();

	if (ball == nullptr || rigidBody == nullptr) return;

	ball->setTargetVelocity(targetVelocity);
	ball->setAcceleration(acceleration);
}

void ForceField::setTargetVelocity(float targetVelocity)
{
	this->targetVelocity = targetVelocity;
}

void ForceField::setAcceleration(float acceleration)
{
	this->acceleration = acceleration;
}