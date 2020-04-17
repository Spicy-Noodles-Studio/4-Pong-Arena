#pragma once
#ifndef BALL_GENERATOR_H
#define BALL_GENERATOR_H

#include <UserComponent.h>
#include <UIElement.h>

class BallGenerator : public UserComponent
{
private:
	float time;
	float generationTime;
	float minimumTime;

	float velocity;
	
public:
	BallGenerator(GameObject* gameObject);
	virtual ~BallGenerator();

	virtual void start();
	virtual void update(float deltaTime);
	virtual void handleData(ComponentData* data);

	void generateBall();

	void setGenerationTime(float generationTime);
	float getGenerationTime() const;
	float getMinimumTime() const;
};

#endif