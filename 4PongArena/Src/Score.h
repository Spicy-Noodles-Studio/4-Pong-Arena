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
		int numOfSelfGoals;
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
	void setTimeAlive(int playerIndex, int originalTime, int timeOfDeath);
	void goalMade(int playerIndex);
	void goalSelfMade(int playerIndex);
	void ballHit(int playerIndex);
	void setPositionOnLeaderBoard(int playerIndex, int position);

	int getTimeAlive(int playerIndex);
	int getNumOfGoals(int playerIndex);
	int getNumOfSelfGoals(int playerIndex);
	int getNumOfBallsHit(int playerIndex);
	int getPositionOnLeaderBoard(int playerIndex);
};


#endif