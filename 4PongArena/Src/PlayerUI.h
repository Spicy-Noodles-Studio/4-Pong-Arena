#pragma once
#ifndef PLAYER_UI_H
#define PLAYER_UI_H

#include <UserComponent.h>
#include <UIElement.h>

class Camera;
class Health;

class PlayerUI : public UserComponent
{
private:
	Camera* mainCamera;
	Health* health;

	std::string name;

	UIElement playerHUD;
	UIElement playerIndicator;

	void updateIndicator();

public:
	PlayerUI(GameObject* gameObject);
	virtual ~PlayerUI();

	virtual void start();
	virtual void update(float deltaTime);

	void updateHealth();
};

#endif