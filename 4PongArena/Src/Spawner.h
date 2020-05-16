#pragma once
#ifndef SPAWNER_H
#define SPAWNER_H

#include <UserComponent.h>

class SoundEmitter;
class ParticleManager;

class Spawner : public UserComponent
{
private:
	SoundEmitter* soundEmitter;
	ParticleManager* particleManager;

	float velocity;
	double angle;

public:
	Spawner(GameObject* gameObject);
	virtual ~Spawner();

	virtual void start();
	virtual void handleData(ComponentData* data);

	void shoot(GameObject* ball);
};
#endif