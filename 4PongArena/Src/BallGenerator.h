#pragma once
#ifndef BALLGENERATOR_H
#define BALLGENERATOR_H

#include "UserComponent.h"

#include <vector>
#include <string>

#include "UIElement.h"

#include "Vector3.h"


class BallGenerator : public UserComponent
{
private:

	std::vector<Vector3> positions = { {-2.0,0.5,-2.0}, {-2,0.5,2}, {2,0.5,-2}, {2,0.5,2} };
	std::vector<Vector3> dirs = { {1.0,0.0,1.0}, {1.0,0.0,-1.0}, {-1.0,0.0,1.0}, {-1.0,0.0,-1.0} };

	int lastGen;
	int balls;
	int players;

	float initTime;
	float minTime;
	float timeGen;


	float time;


	float vel;

private:
	
public:
	BallGenerator(GameObject* gameObject);
	~BallGenerator();

	virtual void start();
	virtual void update(float deltaTime);

	void playerDie();
	void gol();
	void generateBall();
};

#endif