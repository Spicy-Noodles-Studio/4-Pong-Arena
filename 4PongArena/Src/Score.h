#pragma once
#ifndef SCORE_H
#define SCORE_H

#include <vector>

class Score
{
	class ScorePlayer
	{
	public:
		int timeAlive;
		int numOfGoals;
		int numOfSelfGoals;
		int numOfBallsHit;
	};
private:
	std::vector<ScorePlayer*> playerScores;
	int numPlayers;
	std::vector<int> playerId;
	void initScorePlayer(ScorePlayer* player);

public:
	Score();
	virtual ~Score();

	void initScore(int numOfPlayers);
	void clearIds();

	void setTimeAlive(int playerIndex, int originalTime, int timeOfDeath);
	void goalMade(int playerIndex);
	void goalSelfMade(int playerIndex);
	void ballHit(int playerIndex);

	int getTimeAlive(int playerIndex);
	int getNumOfGoals(int playerIndex);
	int getNumOfSelfGoals(int playerIndex);
	int getNumOfBallsHit(int playerIndex);
	void pushPlayerId(int id);
	int getPlayerId(int pos);

};


#endif