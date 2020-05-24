#pragma once
#ifndef PAUSE_MENU_H
#define PAUSE_MENU_H

#include "Menu.h"

#include <UserComponent.h>
#include <UIElement.h>

class InputSystem;
class Game;
class Countdown;

class PauseMenu : public Menu
{
private:
	InputSystem* inputSystem;
	Game* game;
	Countdown* countdown;

	UIElement pauseMenu;
	UIElement pausePanel;

	UIElement optionsMenu;

	bool optionsButtonClick();
	bool exitButtonClick();

	bool checkControllersInput();

public:
	PauseMenu(GameObject* gameObject);
	virtual ~PauseMenu();

	// opens the Pause Menu and pauses the game
	void setPaused(bool paused);
	bool isVisible();

protected:
	virtual void start();
	virtual void preUpdate(float deltaTime);
};

#endif