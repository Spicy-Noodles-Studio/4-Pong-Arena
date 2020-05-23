#include "Score.h"

void Score::initScorePlayer(ScorePlayer* player)
{
	if (player != nullptr) {
		player->timeAlive = 0;
		player->numOfGoals = 0;
		player->numOfSelfGoals = 0;
		player->numOfBallsHit = 0;
	}
}

Score::Score()
{
	numPlayers = 0;
}

Score::~Score()
{
	for (auto score : playerScores)
		delete score;

	playerScores.clear();
}

void Score::initScore(int numOfPlayers)
{
	for (auto score : playerScores)
		delete score;
	playerScores.clear();

	this->numPlayers = numOfPlayers;

	for (int i = 0; i < numOfPlayers; i++)
	{
		ScorePlayer* score = new ScorePlayer();
		initScorePlayer(score);
		this->playerScores.push_back(score);
	}
}

void Score::clearIds()
{
	playerId.clear();
}

void Score::setTimeAlive(int playerIndex, int originalTime, int timeOfDeath)
{
	playerScores.at(playerIndex)->timeAlive = originalTime - timeOfDeath;
}

void Score::goalMade(int playerIndex)
{
	playerScores.at(playerIndex)->numOfGoals++;
}

void Score::goalSelfMade(int playerIndex)
{
	playerScores.at(playerIndex)->numOfSelfGoals++;
}

void Score::ballHit(int playerIndex)
{
	playerScores.at(playerIndex)->numOfBallsHit++;
}

int Score::getTimeAlive(int playerIndex)
{
	return playerScores.at(playerIndex)->timeAlive;
}

int Score::getNumOfGoals(int playerIndex)
{
	return playerScores.at(playerIndex)->numOfGoals;
}

int Score::getNumOfSelfGoals(int playerIndex)
{
	return playerScores.at(playerIndex)->numOfSelfGoals;
}

int Score::getNumOfBallsHit(int playerIndex)
{
	return playerScores.at(playerIndex)->numOfBallsHit;
}

void Score::pushPlayerId(int id)
{
	playerId.push_back(id);
}

int Score::getPlayerId(int pos)
{
	if (pos < playerId.size())
		return playerId.at(pos);
	else
		return -1;
}
