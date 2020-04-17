#pragma once
#ifndef FORCE_FIELD_H
#define FORCE_FIELD_H

#include <UserComponent.h>

class RigidBody;

class ForceField : public UserComponent
{
public:
	enum class State { DISABLED, FORWARDS, BACKWARDS };

private:
	// Impulse given to the ball
	float force;

	// Time to change state
	float stateTime;

	// Timer
	float stateTimer;

	// Boolean so State change is or is not random
	bool random;

	// Current ForceField state
	State currentState;

public:
	ForceField(GameObject* gameObject);
	virtual ~ForceField();

	virtual void start();
	virtual void update(float deltaTime);
	virtual void handleData(ComponentData* data);
	virtual void onTriggerStay(GameObject* other);

	void setState(const std::string& state);
	void setState(State state);

	void setForce(float force);
	void setTime(float time);
	void setRandom(bool random);

private:
	void changeState();
};

#endif