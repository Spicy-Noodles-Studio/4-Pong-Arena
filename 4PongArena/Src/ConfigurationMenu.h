#pragma once
#ifndef CONFIGURATION_MENU_H
#define CONFIGURATION_MENU_H

#include <UserComponent.h>
#include <UIElement.h>

const int MIN_TIME = 60;
const int MAX_TIME = 300;

const int MIN_HEALTH = 1;
const int MAX_HEALTH = 15;

const int BASE_TYPES = 3;
const int OBSTACLES_TYPES = 3;
const int FORCES_TYPES = 3;

const int MIN_PLAYERS = 0;

class InputSystem;
class UILayout;

class ConfigurationMenu : public UserComponent
{
private:
	InputSystem* inputSystem;
	UILayout* configurationLayout;
	UIElement startButton;

	std::vector<std::pair<int, UIElement>> slots;

	bool IA;

	int filledSlots;
	int health;
	int time;

	int levelBaseType;
	int levelObstaclesType;
	int levelForcesType;

	std::vector<std::string> songNames;
	int songIndex;

	void checkInput();

	void fillSlot(int slotIndex, int deviceIndex);
	int isSlotFilled(int index);

	void clearSlot(int index);
	void reorderSlots(int index);

	bool changeFiller(bool value);
	bool changeHealth(int value);
	bool changeTime(int value);
	bool changeSong(int value);

	bool changeLevelBase(int value);
	bool changeLevelObstacles(int value);
	bool changeLevelForces(int value);

	bool startButtonClick();
	bool backButtonClick();

public:
	ConfigurationMenu(GameObject* gameObject);
	virtual ~ConfigurationMenu();

	virtual void start();
	virtual void update(float deltaTime);
};

#endif