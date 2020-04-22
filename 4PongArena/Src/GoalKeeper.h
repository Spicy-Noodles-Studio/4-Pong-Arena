#pragma once
#ifndef GOAL_KEEPER_H
#define GOAL_KEEPER_H

#include <UserComponent.h>

class Health;
class Score;
class GameManager;

class GoalKeeper : public UserComponent
{
private:
	GameObject* goal;
	float offset;
	Health* health;
	Score* scores;
	GameManager* manager;
	int id;
public:
	GoalKeeper(GameObject* gameObject);
	virtual ~GoalKeeper();

	virtual void start();
	virtual void handleData(ComponentData* data);
	virtual void onCollisionEnter(GameObject* other);

	GameObject* getGoal() const;
};

#endif