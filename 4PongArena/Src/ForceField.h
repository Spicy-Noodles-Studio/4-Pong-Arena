#pragma once
#ifndef FORCE_FIELD_H
#define FORCE_FIELD_H

#include <UserComponent.h>
#include <GameObject.h>
#include <RigidBody.h>

class ForceField : public UserComponent
{
private:
	RigidBody* rigidBody;

	float force;
	float stateTime;

	bool random;

	enum state {DISABLED, FORWARDS, BACKWARDS};

	state currentState;
	float currentTime;

	virtual void onTriggerStay(GameObject* other);

public:
	ForceField(GameObject* gameObject);

	virtual void start();
	virtual void update(float deltaTime);
	virtual void handleData(ComponentData* data);

	void setState(std::string state);
	void setForce(float force);
	void setTime(float time);
	void setRandom(bool random);
};

#endif