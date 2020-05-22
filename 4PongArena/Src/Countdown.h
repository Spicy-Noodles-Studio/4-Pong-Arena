#pragma once
#ifndef COUNTDOWN_H
#define COUNTDOWN_H

#include <UserComponent.h>
#include <UIElement.h>

#include <chrono>

class SoundEmitter;

class Countdown : public UserComponent
{
private:
	SoundEmitter* soundEmitter;
	UIElement panel;

	std::vector<GameObject*> players;

	float time;
	std::chrono::steady_clock::time_point last;

	bool startCounting;
	bool countingDown;

	std::string previousCount;

public:
	Countdown(GameObject* gameObject);
	virtual ~Countdown();

	bool isCounting() const;
	float getRemainingTime();

protected:
	virtual void start();
	virtual void update(float deltaTime);
	virtual void handleData(ComponentData* data);


};

#endif