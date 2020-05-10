#pragma once
#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "Menu.h"

class InputSystem;
class UILayout;

class MainMenu : public Menu
{
private:
	InputSystem* inputSystem;

	bool musicPlaying;

	bool singlePlayerButtonClick();
	bool multiplayerButtonClick();
	bool optionsButtonClick();
	bool exitButtonClick();

public:
	MainMenu(GameObject* gameObject);
	virtual ~MainMenu();

	virtual void start();
	virtual void update(float deltaTime);
};

#endif