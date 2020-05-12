#pragma once
#ifndef HEALTH_H
#define HEALTH_H

#include <UserComponent.h>

class SoundEmitter;

class Health : public UserComponent
{
public:
	Health(GameObject* gameObject);
	virtual ~Health();

	virtual void start();
	virtual void handleData(ComponentData* data);

	int getHealth() const;
	void setHealth(int health);
	void receiveDamage(int damage);

	bool isAlive() const;

private:
	int health;
	SoundEmitter* soundEmitter;
};

#endif