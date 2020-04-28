#pragma once
#ifndef GOAL_H
#define GOAL_H

#include <UserComponent.h>

class Health;
class Score;
class GameManager;

class Goal : public UserComponent
{
public:
	Goal(GameObject* gameObject);
	virtual ~Goal();

	virtual void start();
	virtual void onObjectEnter(GameObject* other);

	void setScore(int score);
	int getScore() const;

	void setKeeper(GameObject* keeper);
	GameObject* getKeeper();

private:
	Health* health;
	int score;
	Score* scores;
	GameManager* manager;
	int id;
};

#endif