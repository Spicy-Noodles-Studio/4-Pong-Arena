#pragma once
#ifndef SCORE_H
#define SCORE_H

#include <UserComponent.h>

class Health;

class Score : public UserComponent
{
public:
	Score(GameObject* gameObject);
	~Score();

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