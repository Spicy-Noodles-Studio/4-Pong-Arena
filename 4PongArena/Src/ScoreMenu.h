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
	UIElement player1Text;
	UIElement player2Text;
	UIElement player3Text;
	UIElement player4Text;

	UIElement player1Panel;
	UIElement player2Panel;
	UIElement player3Panel;
	UIElement player4Panel;
	

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