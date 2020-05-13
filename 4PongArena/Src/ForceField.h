#pragma once
#ifndef FORCE_FIELD_H
#define FORCE_FIELD_H

#include <UserComponent.h>

class SoundEmitter;

class ForceField : public UserComponent
{
private:
	SoundEmitter* soundEmitter;
	float volume;

	// Properties given to the ball
	float targetVelocity;
	float acceleration;

public:
	ForceField(GameObject* gameObject);
	virtual ~ForceField();

	virtual void start();
	virtual void update(float deltaTime);
	virtual void handleData(ComponentData* data);
	virtual void onObjectEnter(GameObject* other);

	void setTargetVelocity(float targetVelocity);
	void setAcceleration(float acceleration);
};
#endif