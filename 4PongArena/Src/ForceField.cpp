#include "ForceField.h"
#include <InputSystem.h>
#include <sstream>
#include <math.h>


ForceField::ForceField(GameObject* gameObject) : UserComponent(gameObject)
{
}

void ForceField::start()
{
	rigidBody = gameObject->getComponent<RigidBody>();
	currentState == FORWARDS;
	currentTime = 0;
}

void ForceField::update(float deltaTime)
{
	UserComponent::update(deltaTime);

	currentTime += deltaTime;

	if (currentTime > stateTime) {

		if (random) {
			int rnd = rand() % 3;
			switch (rnd) {
			case 0:
				currentState = FORWARDS;
				break;
			case 1:
				currentState = BACKWARDS;
				break;
			case 2:
				currentState = DISABLED;
				break;
			default:
				currentState = DISABLED;
				break;
			}
		}
		else {
			switch (currentState) {
			case DISABLED:
				currentState = FORWARDS;
				break;
			case FORWARDS:
				currentState = BACKWARDS;
				break;
			case BACKWARDS:
				currentState = DISABLED;
				break;
			}
		}

		currentTime = 0;
	}
}

void ForceField::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "force")
		{
			if(!(ss >> force))
				LOG("FORCE FIELD: Invalid property with name \"%s\"", prop.first.c_str());
		}
		if (prop.first == "stateTime")
		{
			if(!(ss >> stateTime))
				LOG("FORCE FIELD: Invalid property with name \"%s\"", prop.first.c_str());
		}
		if (prop.first == "random")
		{
			if (!(ss >> random))
				LOG("FORCE FIELD: Invalid property with name \"%s\"", prop.first.c_str());
		}
	}
}

/// States: "DISABLED" | "FORWARDS" | "BACKWARDS"
void ForceField::setState(std::string state)
{
	if (state == "DISABLED") currentState = DISABLED;
	else if (state == "FORWARDS") currentState = FORWARDS;
	else if(state == "DISABLED") currentState = BACKWARDS;
	else currentState = DISABLED;
}

void ForceField::onObjectStay(GameObject* other)
{
	if (currentState == DISABLED) return;

	RigidBody* ball = other->getComponent<RigidBody>();

	if (ball == nullptr) return;

	Vector3 forceDirection = {0,0,0};

	if (currentState == FORWARDS) {
		forceDirection = ball->getLinearVelocity();
	}
	else {
		forceDirection = other->transform->getPosition() - gameObject->transform->getPosition();	
	}
	ball->addForce(forceDirection.normalized() * force);
}