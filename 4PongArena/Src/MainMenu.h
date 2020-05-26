#pragma once
#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "Menu.h"

class UILayout;

class MainMenu : public Menu
{
private:
	bool playButtonClick();
	bool optionsButtonClick();
	bool exitButtonClick();

	bool controlsButtonClick();
	bool creditsButtonClick();

	void initMusic();

public:
	MainMenu(GameObject* gameObject);
	virtual ~MainMenu();

protected:
	virtual void start();
};

#endif