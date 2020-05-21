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

	bool playButtonClick();
	bool optionsButtonClick();
	bool exitButtonClick();

	bool controlsButtonClick();
	bool creditsButtonClick();

	void initMusic();

public:
	MainMenu(GameObject* gameObject);
	virtual ~MainMenu();

	virtual void start();
};

#endif