#pragma once

#include "UserComponent.h"


class Health : public UserComponent
{
public:
	Health(GameObject* gameObject);
	virtual ~Health();

	virtual void start();
	virtual void handleData(ComponentData* data);
	virtual void onObjectEnter(GameObject* other);

	int getHealth();
	int getMaxHealth();
	void setHealth(int health);
	bool isAlive();

	void receiveDamage(int damage);
	void die();

private:
	int maxHealth;
	int health; // 1 life = 2 health points

	bool alive; // player alive

};