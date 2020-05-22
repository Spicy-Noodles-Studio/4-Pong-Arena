#pragma once
#ifndef GOAL_H
#define GOAL_H

#include <UserComponent.h>

class Health;
class Score;
class GameManager;
class CameraEffects;
class ParticleManager;

class Goal : public UserComponent
{
public:
	Goal(GameObject* gameObject);
	virtual ~Goal();
	void setScore(int score);
	int getScore() const;

	void setKeeper(GameObject* keeper);
	GameObject* getKeeper();

private:
	Health* health;
	int score;
	Score* scores;
	GameManager* manager;
	ParticleManager* particleManager;
	int id;
	CameraEffects* cameraEffects;

protected:
	virtual void start();
	virtual void onObjectEnter(GameObject* other);

};

#endif