#pragma once
#ifndef SPAWNER_H
#define SPAWNER_H

#include <UserComponent.h>

class SoundEmitter;

class Spawner : public UserComponent
{
private:
	SoundEmitter* soundEmitter;

	float velocity;
	double angle;
	
public:
	Spawner(GameObject* gameObject);
	virtual ~Spawner();

	virtual void handleData(ComponentData* data);

	void shoot(GameObject* ball);

	virtual void start();
};

#endif