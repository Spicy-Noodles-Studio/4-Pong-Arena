#pragma once
#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <UserComponent.h>
#include <queue>

#include "Score.h"

class SoundEmitter;

typedef std::pair<int, int> ii;

class Less
{
public:
	bool operator()(const ii& a, const ii& b)
	{
		return a.second < b.second;
	}
};

class GameManager : public UserComponent
{
private:
	static GameManager* instance;

	SoundEmitter* soundEmitter;
	Score scores;

	std::vector<int> playerIndexes;
	std::vector<int> playerRanking;
	std::priority_queue<ii, std::vector<ii>, Less> ranking;

	std::vector<Vector3> playerColours;
	std::vector<GameObject*> paddles;

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
	float initialSoundVolume;
	float initialMusicVolume;

	std::string song;
	std::string songName;

protected:
	virtual void start();

public:
	GameManager();
	GameManager(GameObject* gameObject);
	virtual ~GameManager();

	static GameManager* GetInstance();

	Score* getScore();

	void setPlayerIndexes(std::vector<int>& playerIndexes);
	std::vector<int>& getPlayerIndexes();

	void setPlayerRanking(int index, int rank);
	int getPlayerRanking(int index) const;

	std::priority_queue<ii, std::vector<ii>, Less>& getRanking();
	void emptyRanking();

	std::vector<Vector3>& getPlayerColours();

	std::vector<GameObject*>& getPaddles();
	void emptyPaddles();

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

	void setSongName(std::string name);
	std::string getSongName() const;

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
	float getInitialSoundVolume() const;
	float getInitialMusicVolume() const;
};

#endif