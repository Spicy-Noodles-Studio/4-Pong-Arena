#pragma once
#include "OptionsMenu.h"



class GameObject;
class OptionsMenuScreen :
	public OptionsMenu
{
private:
	GameObject *screen;
	UIElement pauseMenu;
	UIElement optionsMenu;



	bool backToMenuButtonClick();

public:	
	OptionsMenuScreen(GameObject* gameObject);
	  virtual ~OptionsMenuScreen();

	  virtual void start();
};

