#pragma once
#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <UserComponent.h>
#include "Player.h"
#include "Score.h"
//class Score;

class GameManager : public UserComponent
{
private:
	static GameManager* instance;

	int playersAlive;
	std::vector<Player> players;
	int totalPlayers;

	bool IA;

	int health;
	int time;
	int initialTime;
	bool paused;

	std::string level;
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

	void setIA(bool IA);
	bool getIA() const;

	void setHealth(int health);
	int getHealth() const;

	void setTime(int time);
	int getTime() const;
	int getInitialTime() const;

	void setLevel(std::string level);
	std::string getLevel() const;
	std::string getLastLevel() const;

	void setSong(std::string song);
	std::string getSong() const;
	std::string getLastSong() const;

	void pause(bool paused);
	bool isPaused();

	Score* getScore();

};

#endif