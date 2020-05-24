#pragma once
#ifndef CONFIGURATION_MENU_H
#define CONFIGURATION_MENU_H

#include "Menu.h"

#include <UserComponent.h>
#include <UIElement.h>

#include <vector>

const int MIN_TIME = 60;
const int MAX_TIME = 300;
const int CHANGE_TIME = 10;

const int MIN_HEALTH = 1;
const int MAX_HEALTH = 15;
const int CHANGE_HEALTH = 1;

const int MIN_PLAYERS = 2;

const int BASE_TYPES = 2;
const int OBSTACLES_TYPES = 2;
const int FORCES_TYPES = 2;

class InputSystem;
class UILayout;

class ConfigurationMenu : public Menu
{
private:
	InputSystem* inputSystem;
	UILayout* configurationLayout;

	UIElement settingsPanel;
	UIElement startButton;

	std::vector<std::pair<int, UIElement>> slots;

	int nPlayers;
	int health;

	int time;
	bool mode;

	float previewTime;
	float timer;

	bool songPreview;

	std::vector<std::string> timeModes;
	std::map<std::string, std::string> songNames;

	int levelBaseType;
	int levelObstaclesType;
	int levelForcesType;

	std::string song;

private:
	void checkInput();

	void fillSlot(int index);
	void clearSlot(int index);

	int isIndexConnected(int index);

	bool changeHealth(int value);

	bool changeTimeMode(int value);
	bool changeTime(int value);

	bool changeLevelBase(int value);
	bool changeLevelObstacles(int value);
	bool changeLevelForces(int value);

	bool randomizeButtonClick();

	bool changeSong(int value);
	bool previewSong(bool value);

	void stopPreview();

	bool slotButtonClick(int index, std::string name);

	bool startButtonClick();
	bool settingsButtonClick();
	bool backButtonClick();

public:
	ConfigurationMenu(GameObject* gameObject);
	virtual ~ConfigurationMenu();

protected:
	virtual void start();
	virtual void update(float deltaTime);
};

#endif