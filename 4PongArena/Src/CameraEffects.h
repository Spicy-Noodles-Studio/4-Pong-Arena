#pragma once
#ifndef CAMERAEFFECTS_H
#define CAMERAEFFECTS_H

#include <UserComponent.h>

class Transform;

class CameraEffects : public UserComponent
{
private:
	float max, min, current;

	enum State { IDLE, FADEIN, FADEOUT, SHAKE };

	State state;
	Transform* cam;

	Vector3 shakeDir;
	Vector3 rotationDir;
	Vector3 initialRotation;
	Vector3 initialPosition;

	float dirX;
	float dirY;
	float dirZ;
	float moves;
	float time;
	float vel;

	float minRange;
	float maxRange;
	float duration;

public: 
	CameraEffects(GameObject* gameObject);
	virtual ~CameraEffects();
	void fadeOut();
	void fadeIn();
	void setDarkness();
	bool isFading() const;

	void shake(Vector3 rotDir);

protected:
	virtual void start();
	virtual void update(float deltaTime);
	virtual void handleData(ComponentData* data);

};

#endif