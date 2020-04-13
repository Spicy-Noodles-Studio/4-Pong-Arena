﻿#include "GameManager.h"
#include <ComponentRegister.h>

REGISTER_FACTORY(GameManager);

GameManager* GameManager::instance = nullptr;

GameManager::GameManager() : UserComponent(nullptr)
{

}

GameManager::GameManager(GameObject* gameObject) : UserComponent(gameObject)
{
	if (instance == nullptr)
		instance = this;
	else
		destroy(gameObject);
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
	dontDestroyOnLoad(gameObject);
}

void GameManager::setPlayers(std::vector<Player>& players)
{
	this->players = players;
}

std::vector<Player> GameManager::getPlayers() const
{
	return players;
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

void GameManager::setLevel(std::string level)
{
	this->level = level;
}

std::string GameManager::getLevel() const
{
	return level;
}

void GameManager::setSong(std::string song)
{
	this->song = song;
}

std::string GameManager::getSong() const
{
	return song;
}