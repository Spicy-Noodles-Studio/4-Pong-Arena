#include "ForceField.h"
#include <ComponentRegister.h>
#include <GameObject.h>
#include <RigidBody.h>
#include <MathUtils.h>
#include <sstream>

REGISTER_FACTORY(ForceField);

ForceField::ForceField(GameObject* gameObject) :	UserComponent(gameObject), force(0.0f), stateTime(0.0f), stateTimer(0.0f), random(false),
													currentState(State::FORWARDS)
{
	
}

ForceField::~ForceField()
{

}

void ForceField::start()
{
	currentState = State::FORWARDS;
	stateTimer = 0.0f;
}
	
void ForceField::update(float deltaTime)
{
	stateTimer += deltaTime;

	if (stateTimer >= stateTime) {
		changeState();
		stateTimer = 0;
	}
}

void ForceField::onTriggerStay(GameObject* other)
{
	if (currentState == State::DISABLED) return;

	RigidBody* ball = other->getComponent<RigidBody>();

	if (ball == nullptr) return;

	Vector3 forceDirection = { 0,0,0 };

	if (currentState == State::FORWARDS)
		forceDirection = gameObject->transform->getPosition() - other->transform->getPosition();
	else
		forceDirection = other->transform->getPosition() - gameObject->transform->getPosition();

	ball->addForce(forceDirection.normalized() * force);
}

void ForceField::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "force")
		{
			if(!(ss >> force))
				LOG("FORCE FIELD: Invalid value for property with name \"%s\"", prop.first.c_str());
		}
		else if (prop.first == "stateTime")
		{
			if(!(ss >> stateTime))
				LOG("FORCE FIELD: Invalid value for property with name \"%s\"", prop.first.c_str());
		}
		else if (prop.first == "random")
		{
			if (!(ss >> random))
				LOG("FORCE FIELD: Invalid value for property with name \"%s\"", prop.first.c_str());
		}
		else
			LOG("FORCE FIELD: Invalid property with name \"%s\"", prop.first.c_str());
	}
}

/// States: "DISABLED" | "FORWARDS" | "BACKWARDS"
void ForceField::setState(const std::string& state)
{
	if (state == "DISABLED") currentState = State::DISABLED;
	else if (state == "FORWARDS") currentState = State::FORWARDS;
	else if(state == "BACKWARDS") currentState = State::BACKWARDS;
	else currentState = State::DISABLED;
}

void ForceField::setState(State state)
{
	currentState = state;
}

void ForceField::setForce(float force)
{
	this->force = force;
}

void ForceField::setTime(float time)
{
	stateTime = time;
}

void ForceField::setRandom(bool random)
{
	this->random = random;
}

void ForceField::changeState()
{
	if (random) {
		int rnd = rand() % 3;
		switch (rnd) {
		case 0:
			currentState = State::FORWARDS;
			break;
		case 1:
			currentState = State::BACKWARDS;
			break;
		case 2:
			currentState = State::DISABLED;
			break;
		default:
			currentState = State::DISABLED;
			break;
		}
	}
	else {
		switch (currentState) {
		case State::DISABLED:
			currentState = State::FORWARDS;
			break;
		case State::FORWARDS:
			currentState = State::BACKWARDS;
			break;
		case State::BACKWARDS:
			currentState = State::DISABLED;
			break;
		}
	}
}