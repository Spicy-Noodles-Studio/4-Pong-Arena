#pragma once
#ifndef GOAL_H
#define GOAL_H

#include <UserComponent.h>

class Health;

class Goal : public UserComponent
{
public:
	Goal(GameObject* gameObject);
	virtual ~Goal();

	virtual void onObjectEnter(GameObject* other);

	void setScore(int score);
	int getScore() const;
	void setKeeper(GameObject* keeper);
	GameObject* getKeeper();

private:
	int score;
	Health* health;
};

#endif