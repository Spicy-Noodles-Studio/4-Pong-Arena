#pragma once
#ifndef HEALTH_H
#define HEALTH_H

#include <UserComponent.h>


class Health : public UserComponent
{
public:
	Health(GameObject* gameObject);
	virtual ~Health();

	virtual void onObjectEnter(GameObject* other);
	virtual void handleData(ComponentData* data);

	int getHealth() const;
	void setHealth(int health);
	bool isAlive() const;
	bool isDead() const;

	void receiveDamage(int damage);

private:
	int health;
};

#endif