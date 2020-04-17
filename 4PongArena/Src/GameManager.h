#pragma once
#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <UserComponent.h>
#include "Player.h"

class GameManager : public UserComponent
{
private:
	static GameManager* instance;

	int playersAlive;
	std::vector<Player> players;
	std::vector<GameObject*> generators;
	std::vector<GameObject*> balls;

	int health;
	int time;

	bool paused;

	std::string level;
	std::string song;

public:
	GameManager();
	GameManager(GameObject* gameObject);
	virtual ~GameManager();

	static GameManager* GetInstance();

	virtual void start();

	void setPlayersAlive(int players);
	int getPlayersAlive() const;

	void setPlayers(std::vector<Player>& players);
	std::vector<Player> getPlayers() const;

	void setGenerators(std::vector<GameObject*>& generators);
	std::vector<GameObject*> getGenerators() const;

	void setBalls(std::vector<GameObject*>& balls);
	std::vector<GameObject*> getBalls() const;

	void setHealth(int health);
	int getHealth() const;

	void setTime(int time);
	int getTime() const;

	void setLevel(std::string level);
	std::string getLevel() const;

	void setSong(std::string song);
	std::string getSong() const;

	void pause(bool paused);
	bool isPaused();
};

#endif