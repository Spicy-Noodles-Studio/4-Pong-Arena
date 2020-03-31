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

	state currentState = FORWARDS;
	float currentTime;

	//virtual void onTriggerEnter(GameObject* other);
	virtual void onTriggerStay(GameObject* other);

public:
	ForceField(GameObject* gameObject);

	virtual void start();
	virtual void update(float deltaTime);
	virtual void handleData(ComponentData* data);

	void setState(std::string state);
	void setForce(float _force) { force = _force; }
	void setTime(float time) { stateTime = time; }
	void setRandom(bool _random) { random = _random; }

};

#endif