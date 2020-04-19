#pragma once
#ifndef SCORE_H
#define SCORE_H


#include <vector>
#include <string>

class Score
{
	class ScorePlayer
	{
	public:
		int timeAlive;
		int numOfGoals;
		int numOfBallsHit;
		int position;
	};
private:
	int numPlayers;
	
	std::vector<ScorePlayer*> playerScores;

	void initScorePlayer(ScorePlayer* player);
	

public:
	Score();
	~Score();
	void initScore(int numOfPlayers);
	void setTimeAlive(int PlayerIndex, int originalTime, int timeOfDeath);
	void goalMade(int PlayerIndex);
	void ballHit(int PlayerIndex);
	void setPositionOnLeaderBoard(int PlayerIndex, int position);

	int getTimeAlive(int playerIndex);
	int getNumOfGoals(int playerIndex);
	int getNumOfBallsHit(int playerIndex);
	int getPositionOnLeaderBoard(int playerIndex);


	


};


#endif