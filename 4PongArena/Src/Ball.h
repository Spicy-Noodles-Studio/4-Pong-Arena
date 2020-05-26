#pragma once
#ifndef BALL_H
#define BALL_H

#include <UserComponent.h>

class RigidBody;
class SoundEmitter;
class ParticleManager;

class Ball : public UserComponent
{
private:
	RigidBody* rigidBody;
	SoundEmitter* soundEmitter;
	ParticleManager* particleManager;
	float volume;

	float velocity;
	float targetVelocity;
	float acceleration;

	int idPlayer;

public:
	Ball(GameObject* gameObject);
	virtual ~Ball();
	void setVelocity(float velocity);
	void setTargetVelocity(float targetVelocity);
	void setAcceleration(float acceleration);
	
	void setIdPlayerHit(int id);
	int getIdPlayerHit() const;

protected:
	virtual void onCollisionEnter(GameObject* other);
	virtual void start();
	virtual void update(float deltaTime);
};

#endif