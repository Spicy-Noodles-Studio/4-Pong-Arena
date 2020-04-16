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
	Vector3 direction;

public:
	Ball(GameObject* gameObject);
	~Ball();

	virtual void start();
	virtual void update(float deltaTime);

	float getVelocity();
	void setVelocity(float velocity);
	Vector3 getDirection();
	void setDirection(const Vector3& direction);

};

#endif