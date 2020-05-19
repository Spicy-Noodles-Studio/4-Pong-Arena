#pragma once
#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <UserComponent.h>
#include "Score.h"

class SoundEmitter;

class GameManager : public UserComponent
{
private:
	static GameManager* instance;

	SoundEmitter* soundEmitter;
	Score scores;

	std::vector<int> playerRanking;
	std::vector<int> playerIndexes;

	std::vector<Vector3> playerColours;
	std::vector<GameObject*> paddles;

	int playersAlive;
	int initialPlayers;
	int winner;

	bool menuMusic;
	bool paused;
	bool gameEnded;

	// Remember Configuration Menu's information
	int health;

	int time;
	int initialTime;
	bool timeMode;

	int levelBase;
	int levelObstacles;
	int levelForces;

	float initialBrightness;

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

	void setPlayerIndexes(std::vector<int>& playerIndexes);
	std::vector<int>& getPlayerIndexes();

	std::vector<Vector3>& getPlayerColours();
	std::vector<GameObject*>& getPaddles();

	void setPlayersAlive(int players);
	int getPlayersAlive() const;

	void setInitialPlayers(int players);
	int getInitialPlayers() const;

	void setPaused(bool paused);
	bool isPaused();

	void setHealth(int health);
	int getHealth() const;

	void setTime(int time);
	int getTime() const;

	void setInitialTime(int time);
	int getInitialTime() const;

	void setTimeMode(bool mode);
	bool getTimeMode() const;

	void setWinner(int winner);
	int getWinner() const;

	void setLevelBase(int levelBase);
	int getLevelBase() const;

	void setLevelObstacles(int levelObstacles);
	int getLevelObstacles() const;

	void setLevelForces(int levelForces);
	int getLevelForces() const;

	void setSong(std::string name);
	std::string getSong() const;

	void setGameEnded(bool end);
	bool isGameEnded();

	void setMusicVolume(float volume);

	void playMusic(std::string music);
	void stopMusic(std::string music);

	void resumeMusic(std::string music);
	void pauseMusic(std::string music);

	void setMenuMusic(bool value);
	bool isMenuMusicPlaying() const;

	float getInitialBrightness() const;
};

#endif