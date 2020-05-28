#pragma once
#ifndef PLAYER_UI_H
#define PLAYER_UI_H

#include <UserComponent.h>
#include <UIElement.h>

class Camera;
class PlayerIndex;
class Health;

class PlayerUI : public UserComponent
{
private:
	Camera* mainCamera;
	PlayerIndex* playerIndex;
	Health* health;

	std::string name;

	UIElement playerHUD;

	void updateHealth();

public:
	PlayerUI(GameObject* gameObject);
	virtual ~PlayerUI();

protected:
	virtual void start();
	virtual void update(float deltaTime);
};

#endif