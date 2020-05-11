#pragma once
#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <UserComponent.h>
#include "Player.h"
#include "Score.h"

class GameManager : public UserComponent
{
private:
	static GameManager* instance;

	Score scores;

	std::vector<int> playerRanking;
	std::vector<Player> players;
	std::vector<Vector3> playerColours;
	std::vector<GameObject*> paddles;

	int playersAlive;
	int initialPlayers;

	bool IA;
	bool paused;

	int health;

	int time;
	int initialTime;

	int winner;

	int levelBase;
	int levelObstacles;
	int levelForces;

	std::string song;

public:
	GameManager();
	GameManager(GameObject* gameObject);
	virtual ~GameManager();

	static GameManager* GetInstance();

	virtual void start();

	Score* getScore();

	void initPlayerRanking(int tam);
	void setPlayerRanking(int index, int rank);
	int getPlayerRanking(int index) const;

	void setPlayers(std::vector<Player>& players);
	std::vector<Player> getPlayers() const;

	std::vector<Vector3>& getPlayerColours();
	std::vector<GameObject*>& getPaddles();

	void setPlayersAlive(int players);
	int getPlayersAlive() const;

	void setInitialPlayers(int players);
	int getInitialPlayers() const;

	void setIA(bool IA);
	bool getIA() const;

	void setPaused(bool paused);
	bool isPaused();

	void setHealth(int health);
	int getHealth() const;

	void setTime(int time);
	int getTime() const;

	void setInitialTime(int time);
	int getInitialTime() const;

	void setWinner(int winner);
	int getWinner() const;

	void setLevelBase(int levelBase);
	int getLevelBase() const;

	void setLevelObstacles(int levelObstacles);
	int getLevelObstacles() const;

	void setLevelForces(int levelForces);
	int getLevelForces() const;

	void setSong(std::string song);
	std::string getSong() const;
};

#endif