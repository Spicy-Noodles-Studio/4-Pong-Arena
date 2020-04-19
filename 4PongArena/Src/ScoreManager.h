#pragma once
#ifndef SCORE_MANAGER_H
#define SCORE_MANAGER_H

#include <UserComponent.h>
#include <vector>
#include <string>
#include "UIElement.h"

class GameManager;

class ScoreManager :
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

	void initStatistics(int numOfPlayers);
	void reposition(int numOfPlayers);

	void setNumOfHits(int playerIndex);
	void setNumOfGoals(int playerIndex);
	void setTimeAlive(int playerIndex);
	void setPositionOnLeaderBoard(int playerIndex);

public:
	ScoreManager(GameObject* gameObject);
	virtual ~ScoreManager();

	virtual void start();

	
};


#endif