﻿#include "GameManager.h"
#include <ComponentRegister.h>
#include <SoundEmitter.h>
#include <GameObject.h>
#include <Timer.h>
#include <WindowManager.h>

REGISTER_FACTORY(GameManager);

GameManager* GameManager::instance = nullptr;

GameManager::GameManager() : UserComponent(nullptr)
{

}

GameManager::GameManager(GameObject* gameObject) : UserComponent(gameObject), song(""), health(5), time(60), initialTime(time), timeMode(false),
levelBase(0), levelForces(0), levelObstacles(0), menuMusic(false), paused(false), gameEnded(false), initialBrightness(0.5)
{
	if (instance == nullptr) {
		instance = this;
		WindowManager::GetInstance()->setBrightness(initialBrightness);
	}
	else
		destroy(gameObject);

	playerIndexes = std::vector<int>(4, -1);
}

GameManager::~GameManager()
{
	if (instance == this)
		instance = nullptr;
}

GameManager* GameManager::GetInstance()
{
	return instance;
}

void GameManager::start()
{
	playerColours = { {1,0,0}, {0,0,1}, {1,1,0}, {0,1,0} };

	if (soundEmitter == nullptr && gameObject != nullptr)
		soundEmitter = gameObject->getComponent<SoundEmitter>();

	dontDestroyOnLoad(gameObject);
}

Score* GameManager::getScore()
{
	return &scores;
}

void GameManager::initPlayerRanking(int tam)
{
	playerRanking = std::vector<int>(tam, 0);
}

void GameManager::setPlayerRanking(int index, int rank)
{
	if (index > 0 && (index - 1) < playerRanking.size())
		playerRanking[index - 1] = rank;
}

int GameManager::getPlayerRanking(int index) const
{
	if (index > 0 && (index - 1) < playerRanking.size())
		return playerRanking[index - 1];

	return -1;
}

void GameManager::setPlayerIndexes(std::vector<int>& playerIndexes)
{

	this->playerIndexes = playerIndexes;
}

std::vector<int>& GameManager::getPlayerIndexes()
{
	return playerIndexes;
}

std::vector<Vector3>& GameManager::getPlayerColours()
{
	return playerColours;
}

std::vector<GameObject*>& GameManager::getPaddles()
{
	return paddles;
}

void GameManager::setPlayersAlive(int players)
{
	this->playersAlive = players;
}

int GameManager::getPlayersAlive() const
{
	return playersAlive;
}

void GameManager::setInitialPlayers(int players)
{
	initialPlayers = players;
}

int GameManager::getInitialPlayers() const
{
	return initialPlayers;
}

void GameManager::setPaused(bool paused)
{
	if (paused)
		Timer::GetInstance()->setTimeScale(0.0f); //Pause the game
	else
		Timer::GetInstance()->setTimeScale(1.0f); //Resume the game

	this->paused = paused;
}

bool GameManager::isPaused()
{
	return paused;
}

void GameManager::setHealth(int health)
{
	this->health = health;
}

int GameManager::getHealth() const
{
	return health;
}

void GameManager::setTime(int time)
{
	this->time = time;

}

int GameManager::getTime() const
{
	return time;
}

void GameManager::setInitialTime(int time)
{
	this->initialTime = time;
	this->time = time;
}

int GameManager::getInitialTime() const
{
	return initialTime;
}

void GameManager::setTimeMode(bool mode)
{
	timeMode = mode;
}

bool GameManager::getTimeMode() const
{
	return timeMode;
}

void GameManager::setWinner(int winner)
{
	this->winner = winner;
}

int GameManager::getWinner() const
{
	return winner;
}

void GameManager::setLevelBase(int levelBase)
{
	this->levelBase = levelBase;
}

int GameManager::getLevelBase() const
{
	return levelBase;
}

void GameManager::setLevelObstacles(int levelObstacles)
{
	this->levelObstacles = levelObstacles;
}

int GameManager::getLevelObstacles() const
{
	return levelObstacles;
}

void GameManager::setLevelForces(int levelForces)
{
	this->levelForces = levelForces;
}

int GameManager::getLevelForces() const
{
	return levelForces;
}

void GameManager::setGameEnded(bool end)
{
	gameEnded = end;
}

bool GameManager::isGameEnded()
{
	return gameEnded;
}

void GameManager::setSong(std::string name)
{
	song = name;
}

std::string GameManager::getSong() const
{
	return song;
}

void GameManager::playMusic(std::string music)
{
	if (soundEmitter == nullptr) return;

	soundEmitter->stop(music);

	if(music == "")
		soundEmitter->playMusic(song);
	else
		soundEmitter->playMusic(music);
}

void GameManager::stopMusic(std::string music)
{
	if (soundEmitter != nullptr) soundEmitter->stop(music);
}

void GameManager::resumeMusic(std::string music)
{
	if (soundEmitter != nullptr) soundEmitter->resume(music);
}

void GameManager::pauseMusic(std::string music)
{
	if (soundEmitter != nullptr) soundEmitter->pause(music);
}

void GameManager::setMenuMusic(bool value)
{
	menuMusic = value;
}

bool GameManager::isMenuMusicPlaying() const
{
	return menuMusic;
}

float GameManager::getInitialBrightness() const
{
	return initialBrightness;
}

void GameManager::setMusicVolume(float volume)
{
	if (soundEmitter != nullptr) soundEmitter->setVolume(volume);
}