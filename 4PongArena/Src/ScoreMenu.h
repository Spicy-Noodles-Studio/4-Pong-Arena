#pragma once
#ifndef SCORE_MENU_H
#define SCORE_MENU_H

#include <UserComponent.h>
#include <vector>
#include <string>
#include "UIElement.h"

class GameManager;

class ScoreMenu :
	public UserComponent
{
	GameManager* manager;
	std::vector<UIElement> texts;
	std::vector<UIElement> panels;

private:
		// events
	bool resetButtonClick();
	bool backButtonClick();

	void initStatistics(int numOfPlayers);
	void reposition(int numOfPlayers);

	void setNumOfHits(int playerIndex);
	void setNumOfGoals(int playerIndex);
	void setNumOfSelfGoals(int playerIndex);
	void setTimeAlive(int playerIndex);
	void setPositionOnLeaderBoard(int playerIndex);

public:
	ScoreMenu(GameObject* gameObject);
	virtual ~ScoreMenu();

	virtual void start();

};


#endif