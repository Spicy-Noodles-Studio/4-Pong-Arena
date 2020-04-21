#include "Score.h"

void Score::initScorePlayer(ScorePlayer* player)
{
	player->timeAlive = 0;
	player->position = 1;
	player->numOfBallsHit = 0;
	//player->numOfAirAttacks = 0;
	//player->numOfGroundAttacks = 0;
	player->numOfGoals = 0;
	player->numOfSelfGoals = 0;
	
}

Score::Score()
{
	numPlayers = 0;
}

Score::~Score()
{
	for (auto score:playerScores)
	{

		delete score;
	}
	playerScores.clear();
}

void Score::initScore(int numOfPlayers)
{
	playerScores.clear();
	this->numPlayers = numOfPlayers;
	for (int i = 0; i < numOfPlayers; i++)
	{
		ScorePlayer* score = new ScorePlayer();
		
		initScorePlayer(score);
		this->playerScores.push_back(score);
	}
}

void Score::setTimeAlive(int PlayerIndex, int originalTime, int timeOfDeath)
{
	playerScores.at(PlayerIndex-1)->timeAlive = originalTime - timeOfDeath;
}

void Score::goalMade(int PlayerIndex)
{
	playerScores.at(PlayerIndex-1)->numOfGoals++;
}

void Score::goalSelfMade(int PlayerIndex)
{
	playerScores.at(PlayerIndex - 1)->numOfSelfGoals++;
}

void Score::ballHit(int PlayerIndex)
{
	playerScores.at(PlayerIndex-1)->numOfBallsHit++;
}

void Score::setPositionOnLeaderBoard(int PlayerIndex, int position)
{
	playerScores.at(PlayerIndex-1)->position=position;
}

int Score::getTimeAlive(int playerIndex)
{
	return playerScores.at(playerIndex - 1)->timeAlive;
}

int Score::getNumOfGoals(int playerIndex)
{
	return playerScores.at(playerIndex - 1)->numOfGoals;
}

int Score::getNumOfSelfGoals(int playerIndex)
{
	return playerScores.at(playerIndex - 1)->numOfSelfGoals;
}

int Score::getNumOfBallsHit(int playerIndex)
{
	return playerScores.at(playerIndex - 1)->numOfBallsHit;
}

int Score::getPositionOnLeaderBoard(int playerIndex)
{
	return playerScores.at(playerIndex - 1)->position;
}
