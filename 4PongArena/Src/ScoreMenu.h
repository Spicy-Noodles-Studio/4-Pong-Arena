#pragma once
#ifndef SCORE_MENU_H
#define SCORE_MENU_H

#include "Menu.h"

#include <UserComponent.h>
#include <UIElement.h>
#include <vector>
#include <string>

class Score;

class ScoreMenu : public Menu
{
private:
	Score* score;
	std::vector<UIElement> texts;
	std::vector<UIElement> panels;

	std::vector<int> positions;

	void reposition(int numOfPlayers);
	void initStatistics(int numOfPlayers);

	void setNumOfHits(int playerIndex);

	void setNumOfGoals(int playerIndex);
	void setNumOfSelfGoals(int playerIndex);

	void setTimeAlive(int playerIndex);

	bool checkControllersInput();

public:
	ScoreMenu(GameObject* gameObject);
	virtual ~ScoreMenu();

protected:
	virtual void start();
	virtual void update(float deltaTime);
};

#endif