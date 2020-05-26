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
	
	Health* keeperHealth;
	GameObject* keeper;
	
	Score* scoreManager;
	GameManager* gameManager;
	ParticleManager* particleManager;
	CameraEffects* cameraEffects;

	int score;
	int id;

protected:
	virtual void start();
	virtual void onObjectEnter(GameObject* other);

};

#endif