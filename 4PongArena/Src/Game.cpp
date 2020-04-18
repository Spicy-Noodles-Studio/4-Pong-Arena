#include "Game.h"
#include <GaiaData.h>
#include <SceneManager.h>
#include <UILayout.h>
#include <GameObject.h>

#include "PlayerController.h"
#include "IAPaddle.h"
#include "Health.h"
#include "SpawnerManager.h"
#include "GameManager.h"

#include <ComponentRegister.h>

REGISTER_FACTORY(Game);

void Game::createLevel()
{
	GaiaData levelData;
	levelData.load("./Assets/Levels/" + GameManager::GetInstance()->getLevel() + ".level");

	// player initial transforms
	GaiaData playerData = levelData.find("PlayerTransforms");
	for (int i = 0; i < playerData.size(); i++)
	{
		std::stringstream ss(playerData[i][0].getValue());
		double posX, posY, posZ;

		if (!(ss >> posX >> posY >> posZ))
		{
			LOG_ERROR("GAME", "invalid player position \"%s\"", playerData[i][0].getValue().c_str());
			continue;
		}

		ss = std::stringstream(playerData[i][1].getValue());
		double rotX, rotY, rotZ;

		if (!(ss >> rotX >> rotY >> rotZ))
		{
			LOG_ERROR("GAME", "invalid player rotation \"%s\"", playerData[i][1].getValue().c_str());
			continue;
		}

		playerTransforms.push_back({ { posX, posY, posZ }, { rotX, rotY, rotZ } });
	}

	// spawner initial transforms
	GaiaData spawnerData = levelData.find("SpawnerTransforms");
	for (int i = 0; i < spawnerData.size(); i++)
	{
		std::stringstream ss(spawnerData[i][0].getValue());
		double posX, posY, posZ;

		if (!(ss >> posX >> posY >> posZ))
		{
			LOG_ERROR("GAME", "invalid player position \"%s\"", spawnerData[i][0].getValue().c_str());
			continue;
		}

		ss = std::stringstream(spawnerData[i][1].getValue());
		double rotX, rotY, rotZ;

		if (!(ss >> rotX >> rotY >> rotZ))
		{
			LOG_ERROR("GAME", "invalid player rotation \"%s\"", spawnerData[i][1].getValue().c_str());
			continue;
		}

		spawnerTransforms.push_back({ { posX, posY, posZ }, { rotX, rotY, rotZ } });
	}

	// wall initial scale
	GaiaData wallData = levelData.find("WallScale");

	std::stringstream ss(wallData[0].getValue());
	double scaX, scaY, scaZ;

	if (!(ss >> scaX >> scaY >> scaZ))
		LOG_ERROR("GAME", "invalid player position \"%s\"", wallData[0].getValue().c_str());

	wallScale = { scaX, scaY, scaZ };
}

void Game::createPlayers()
{
	std::vector<Player> players = gameManager->getPlayers();

	int nPlayers = players.size();

	for (int i = 0; i < nPlayers; i++)
	{
		GameObject* paddle = instantiate("Paddle", playerTransforms[i].first);
		paddle->transform->setRotation(playerTransforms[i].second);

		paddle->getComponent<PlayerController>()->setPlayer(players[i].id, players[i].index);
		paddle->getComponent<Health>()->setHealth(gameManager->getHealth());

		paddles.push_back(paddle);
	}

	int nUnfilled = MAX_PLAYERS - nPlayers;

	if (nUnfilled > 0)
	{
		for (int i = 0; i < nUnfilled; i++)
		{
			if (gameManager->getIA())
			{
				GameObject* paddleIA = instantiate("IA", playerTransforms[i + nPlayers].first);
				paddleIA->transform->setRotation(playerTransforms[i + nPlayers].second);

				paddleIA->getComponent<IAPaddle>()->setId(MAX_PLAYERS - i);
				paddleIA->getComponent<Health>()->setHealth(gameManager->getHealth());

				paddles.push_back(paddleIA);
			}
			else
			{
				GameObject* wall = instantiate("Wall", playerTransforms[i + nPlayers].first);
				wall->transform->setRotation(playerTransforms[i + nPlayers].second);
				wall->transform->setScale(wallScale);
			}
		}
	}

	gameManager->setPlayersAlive(paddles.size());
}

void Game::createSpawners()
{
	std::vector<GameObject*> aux;

	int n = spawnerTransforms.size();

	for (int i = 0; i < n; i++)
	{
		GameObject* spawner = instantiate("Spawner", spawnerTransforms[i].first);
		spawner->transform->setRotation(spawnerTransforms[i].second);

		aux.push_back(spawner);
	}

	findGameObjectWithName("SpawnerManager")->getComponent<SpawnerManager>()->setSpawners(aux);
}

void Game::playSong()
{
	//findGameObjectWithName("MainCamera")->getComponent<SoundEmitter>()->play(GameManager::GetInstance()->getSong());
}

void Game::chooseWinner()
{
	gameTimer = 0.0f;

	bool tie = false;
	int majorHealth = 0;
	int majorIndex = 0;

	for (int i = 0; i < paddles.size(); i++)
	{
		Health* health = paddles[i]->getComponent<Health>();
		if (health != nullptr && health->isAlive())
		{
			if (health->getHealth() > majorHealth)
			{
				majorHealth = health->getHealth();
				majorIndex = i;
			}
			else if (health->getHealth() == majorHealth)
				tie = true;
		}
	}

	if (gameLayout != nullptr)
	{
		winnerPanel.setVisible(true);

		if (tie)
			winnerText.setText("TIE");
		else
		{
			winner = majorIndex;
			winnerText.setText("WINNER: P" + std::to_string(winner + 1));
		}
	}
}

Game::Game(GameObject* gameObject) : UserComponent(gameObject), gameManager(nullptr), gameLayout(nullptr), timeText(NULL), winnerPanel(NULL), winnerText(NULL), finishTimer(3.0f), winner(0)
{

}

Game::~Game()
{

}

void Game::start()
{
	gameManager = GameManager::GetInstance();

	GameObject* mainCamera = findGameObjectWithName("MainCamera");

	if (mainCamera != nullptr)
		gameLayout = mainCamera->getComponent<UILayout>();

	if (gameLayout != nullptr)
	{
		timeText = gameLayout->getRoot().getChild("Time");

		winnerPanel = gameLayout->getRoot().getChild("WinnerBackground");
		winnerPanel.setVisible(false);

		winnerText = winnerPanel.getChild("Winner");
	}

	createLevel();
	createPlayers();
	//createSpawners();
	playSong();

	gameTimer = gameManager->getTime();
}

void Game::update(float deltaTime)
{
	if (gameTimer > 0)
	{
		gameTimer -= deltaTime;

		if (gameTimer <= 0.0f)
			chooseWinner();

		if (gameLayout != nullptr)
			timeText.setText(std::to_string((int)gameTimer % 60));
	}
	else
	{
		finishTimer -= deltaTime;

		if (finishTimer <= 0.0f)
			SceneManager::GetInstance()->changeScene("ConfigurationMenu"); // Cambiar a menu de final de partida
	}

	if (gameManager->getPlayersAlive() == 0)
		chooseWinner();
}