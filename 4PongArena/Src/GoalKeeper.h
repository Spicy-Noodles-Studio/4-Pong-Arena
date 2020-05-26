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
	GameObject* goalObject;
	float offset;
	Health* health;
	Score* scores;
	GameManager* gameManager;
	int id;
public:
	GoalKeeper(GameObject* gameObject);
	virtual ~GoalKeeper();
	GameObject* getGoal() const;

protected:
	virtual void start();
	virtual void handleData(ComponentData* data);
	virtual void onCollisionEnter(GameObject* other);

};

#endif