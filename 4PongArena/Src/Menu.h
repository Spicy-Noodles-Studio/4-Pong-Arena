#pragma once

#ifndef MENU_H
#define MENU_H

#include <UserComponent.h>

class InputSystem;
class InterfaceSystem;
class SoundEmitter;
class SceneManager;

class Menu : public UserComponent
{
protected:
	std::string backSound = "Back_Button";
	std::string buttonSound = "Button_Click";
	std::string menuMusic = "Menu_loop";

	InputSystem* inputSystem;
	InterfaceSystem* interfaceSystem;
	SoundEmitter* soundEmitter;
	SceneManager* sceneManager;
	GameObject* mainCamera;

	virtual void start();
	virtual void buttonClick(const std::string& sound);
	virtual bool backButtonClick();

public:
	Menu(GameObject* gameObject);
	virtual ~Menu();

};

#endif