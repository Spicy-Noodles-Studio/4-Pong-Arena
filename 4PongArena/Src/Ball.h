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

public:
	Ball(GameObject* gameObject);
	virtual ~Ball();

	virtual void start();
	virtual void update(float deltaTime);
	virtual void handleData(ComponentData* data);

	float getVelocity();
	void setVelocity(float velocity);
};

#endif