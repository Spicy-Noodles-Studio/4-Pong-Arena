#pragma once
#ifndef CREDITS_H
#define CREDITS_H

#include "Menu.h"

#include <vector>

class Credits : public Menu
{
private:
	std::vector<std::string> names;

	bool checkControllersInput();

public:
	Credits(GameObject* gameObject);
	virtual ~Credits();

protected:
	virtual void start();
	virtual void update(float deltaTime);
};

#endif