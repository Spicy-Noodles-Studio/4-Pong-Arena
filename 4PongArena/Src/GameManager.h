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

	std::vector<Vector3> playerColours;
	int playersAlive;
	std::vector<Player> players;
	int totalPlayers;

	bool IA;

	int health;
	int time;
	int initialTime;
	bool paused;

	int levelBase;
	int levelObstacles;
	int levelForces;

	std::string song;

	std::string lastLevel;
	std::string lastSong;

	Score scores;

public:
	GameManager();
	GameManager(GameObject* gameObject);
	virtual ~GameManager();

	static GameManager* GetInstance();

	virtual void start();

	void setPlayersAlive(int players);
	int getPlayersAlive() const;
	void setTotalPlayers(int players);
	int getTotalPlayers() const;

	void setPlayers(std::vector<Player>& players);
	std::vector<Player> getPlayers() const;
	std::vector<Vector3>& getPlayerColours();

	void setIA(bool IA);
	bool getIA() const;

	void setHealth(int health);
	int getHealth() const;

	void setTime(int time);
	int getTime() const;
	void setInitialTime(int time);
	int getInitialTime() const;

	void setLevelBase(int levelBase);
	void setLevelObstacles(int levelObstacles);
	void setLevelForces(int levelForces);

	int getLevelBase() const;
	int getLevelObstacles() const;
	int getLevelForces() const;

	std::string getLastLevel() const;

	void setSong(std::string song);
	std::string getSong() const;
	std::string getLastSong() const;

	void pause(bool paused);
	bool isPaused();

	Score* getScore();

};

#endif