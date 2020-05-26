#include "Score.h"
#include "ErrorManagement.h"

void Score::initScorePlayer(ScorePlayer* player)
{
	if (notNull(player)) {
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
	playerIDs.clear();
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
		if (notNull(score)) {
			initScorePlayer(score);
			this->playerScores.push_back(score);
		}
	}
}

std::vector<int>& Score::getPlayerIDs()
{
	return playerIDs;
}

void Score::clearIDs()
{
	playerIDs.clear();
}

void Score::setTimeAlive(int playerIndex, int originalTime, int timeOfDeath)
{
	if (playerIndex >= 0 && playerIndex < playerScores.size())
		playerScores.at(playerIndex)->timeAlive = originalTime - timeOfDeath;

}

void Score::goalMade(int playerIndex)
{
	if (playerIndex >= 0 && playerIndex < playerScores.size())
		playerScores.at(playerIndex)->numOfGoals++;

}

void Score::goalSelfMade(int playerIndex)
{
	if (playerIndex >= 0 && playerIndex < playerScores.size())
		playerScores.at(playerIndex)->numOfSelfGoals++;

}

void Score::ballHit(int playerIndex)
{
	if (playerIndex >= 0 && playerIndex < playerScores.size())
		playerScores.at(playerIndex)->numOfBallsHit++;

}

int Score::getTimeAlive(int playerIndex)
{
	if (playerIndex >= 0 && playerIndex < playerScores.size())
		return playerScores.at(playerIndex)->timeAlive;
	else
		return -1;
}

int Score::getNumOfGoals(int playerIndex)
{
	if (playerIndex >= 0 && playerIndex < playerScores.size())
		return playerScores.at(playerIndex)->numOfGoals;
	else
		return -1;
}

int Score::getNumOfSelfGoals(int playerIndex)
{
	if (playerIndex >= 0 && playerIndex < playerScores.size())
		return playerScores.at(playerIndex)->numOfSelfGoals;
	else
		return -1;
}

int Score::getNumOfBallsHit(int playerIndex)
{
	if (playerIndex >= 0 && playerIndex < playerScores.size())
		return playerScores.at(playerIndex)->numOfBallsHit;
	else
		return -1;
}