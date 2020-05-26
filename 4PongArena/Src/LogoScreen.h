#pragma once
#ifndef LOGO_SCREEN_H
#define LOGO_SCREEN_H

#include "Menu.h"

class LogoScreen : public Menu
{
private:
	float time;

	bool checkControllersInput();

public:
	LogoScreen(GameObject* gameObject);
	virtual ~LogoScreen();

protected:
	virtual void start();
	virtual void update(float deltaTime);
};

#endif