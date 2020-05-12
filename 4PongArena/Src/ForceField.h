#pragma once
#ifndef FORCE_FIELD_H
#define FORCE_FIELD_H

#include <UserComponent.h>

class ForceField : public UserComponent
{
private:
	// Properties given to the ball
	float targetVelocity;
	float acceleration;

public:
	ForceField(GameObject* gameObject);
	virtual ~ForceField();

	virtual void handleData(ComponentData* data);
	virtual void onObjectEnter(GameObject* other);

	void setTargetVelocity(float targetVelocity);
	void setAcceleration(float acceleration);
};

#endif