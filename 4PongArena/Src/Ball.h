#pragma once
#ifndef BALL_H
#define BALL_H

#include <UserComponent.h>

class RigidBody;

class Ball : public UserComponent
{
private:
	RigidBody* rigidBody;
	float velocity;
	float targetVelocity;
	float acceleration;

public:
	Ball(GameObject* gameObject);
	virtual ~Ball();

	virtual void start();
	virtual void update(float deltaTime);

	void setVelocity(float velocity);
	void setTargetVelocity(float targetVelocity);
	void setAcceleration(float acceleration);
};

#endif