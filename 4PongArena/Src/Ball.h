#pragma once
#ifndef BALL_H
#define BALL_H

#include <UserComponent.h>

class RigidBody;

class Ball : public UserComponent
{
private:
	RigidBody* rigidBody;
	float maxVelocity;

public:
	Ball(GameObject* gameObject);
	virtual ~Ball();

	virtual void start();
	virtual void update(float deltaTime);
	virtual void handleData(ComponentData* data);
};

#endif