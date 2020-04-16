#pragma once
#ifndef IA_PADDLE_H
#define IA_PADDLE_H

#include <UserComponent.h>

class Movement;

class IAPaddle : public UserComponent
{
private:
	enum class State { CHOOSE_TARGET, MOVE };
	State currentState;
	GameObject* targetBall;
	Movement* movement;
	float decisionTime;
	float decisionTimer;

	std::vector<GameObject*> balls; // TODO: por eficiencia, deberia ser un puntero a un vector de �GameManager?

public:
	IAPaddle(GameObject* gameObject);
	virtual ~IAPaddle();

	virtual void start();
	virtual void update(float deltaTime);

	virtual void handleData(ComponentData* data);

private:
	void processChooseTargetState();
	void processMoveState();

	void takeDecision();
	bool isBallBehind(const Vector3& ballPosition);
	bool isBallHeadingToMe(const Vector3& ballDirection);
};

#endif
