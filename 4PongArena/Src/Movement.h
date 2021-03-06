#pragma once
#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <UserComponent.h>

class RigidBody;

class Movement : public UserComponent
{
public:
	Movement(GameObject* gameObject);
	virtual ~Movement();


	void moveRight();
	void moveLeft();
	void stop();

	void setNormal(const Vector3& normal);
	const Vector3& getNormal() const;

	void setVelocity(float velocity);
	float getVelocity() const;

private:
	void move();
	const Vector3 getRight() const;
	const Vector3 getLeft() const;
	RigidBody* rigidBody;
	Vector3 direction;
	Vector3 normal;
	float velocity;

protected:
	virtual void start();
	virtual void update(float deltaTime);
	virtual void handleData(ComponentData* data);

};

#endif