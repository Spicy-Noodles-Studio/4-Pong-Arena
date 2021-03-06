#pragma once
#ifndef IA_PADDLE_H
#define IA_PADDLE_H

#include <UserComponent.h>

class Movement;
class SoundEmitter;

class IAPaddle : public UserComponent
{
private:
	enum class State { CHOOSE_TARGET, MOVE };
	State currentState;
	GameObject* targetBall;
	Movement* movement;

	SoundEmitter* soundEmitter;
	float volume;

	float decisionTime;
	float decisionTimer;

	int id;

public:
	IAPaddle(GameObject* gameObject);
	virtual ~IAPaddle();

	void setId(int id);
	int getId() const;

private:
	void processChooseTargetState();
	void processMoveState();

	void takeDecision();

	bool isBallBehind(const Vector3& ballPosition);
	bool isBallHeadingToMe(const Vector3& ballDirection);

protected:

	virtual void start();
	virtual void update(float deltaTime);

	virtual void handleData(ComponentData* data);
};

#endif