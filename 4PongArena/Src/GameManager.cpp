#include "GameManager.h"
#include <Timer.h>




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
	playerColours = { {1,0,0}, {0,0,1}, {1,1,0}, {0,1,0} };
	levelColours = { { { 0.32207590341567993 ,0.18894420564174652, 0.5277079343795776},{0.001, 0.0001 ,0.000001 }, {0.778335690498352 ,0.10328326374292374, 0.8000000715255737},{1.556671380996704, 0.20656652748584747, 1.6000001430511475} },
	{ { 0.9999999999 ,0.5370338559150696 ,0.3051641583442688},{0.25999999046325684, 0.13962879741636414, 0.07934267825923769 }, {0.8000000715255737, 0.4460046887397766 ,0.13527119159698486},{1.6000001430511475, 0.8920093774795532, 0.2705423831939697} },
	{ { 0.30954939126968384 ,0.8000000715255737, 0.22599053382873535},{0.10215129395283284 ,0.2640000102519977, 0.07457687239185873 }, {0.7807604670524597 ,0.8000000715255737 ,0.0},{1.5615209341049194 ,1.6000001430511475, 0.0} } };
	dontDestroyOnLoad(gameObject);
}

void GameManager::setPlayersAlive(int players)
{
	this->playersAlive = players;
}

int GameManager::getPlayersAlive() const
{
	return playersAlive;
}

void GameManager::setTotalPlayers(int players)
{
	totalPlayers = players;
}

int GameManager::getTotalPlayers() const
{
	return totalPlayers;
}

void GameManager::setPlayers(std::vector<Player>& players)
{
	this->players = players;
}

std::vector<Player> GameManager::getPlayers() const
{
	return players;
}

std::vector<Vector3>& GameManager::getPlayerColours()
{
	return playerColours;
}

std::vector<std::vector<Vector3>>& GameManager::getLevelColours()
{
	return levelColours;
}

void GameManager::setIA(bool IA)
{
	this->IA = IA;
}

bool GameManager::getIA() const
{
	return IA;
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

void GameManager::setLevelBase(int levelBase)
{
	this->levelBase = levelBase;
	//this->lastLevel = level;
}

void GameManager::setLevelObstacles(int levelObstacles)
{
	this->levelObstacles = levelObstacles;
	//this->lastLevel = level;
}

void GameManager::setLevelForces(int levelForces)
{
	this->levelForces = levelForces;
	//this->lastLevel = level;
}

int GameManager::getLevelBase() const
{
	return levelBase;
}

int GameManager::getLevelObstacles() const
{
	return levelObstacles;
}

int GameManager::getLevelForces() const
{
	return levelForces;
}

std::string GameManager::getLastLevel() const
{
	return lastLevel;
}

void GameManager::setSong(std::string song)
{
	this->song = song;
	this->lastSong = song;
}

std::string GameManager::getSong() const
{
	return song;
}

std::string GameManager::getLastSong() const
{
	return lastSong;
}

void GameManager::pause(bool paused)
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

Score* GameManager::getScore()
{
	return &scores;
}
