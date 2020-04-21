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

	// force field initial transforms
	GaiaData forceFieldData = levelData.find("ForceFieldTransforms");
	for (int i = 0; i < forceFieldData.size(); i++)
	{
		std::stringstream ss(forceFieldData[i][0].getValue());
		double posX, posY, posZ;

		if (!(ss >> posX >> posY >> posZ))
		{
			LOG_ERROR("GAME", "invalid player position \"%s\"", forceFieldData[i][0].getValue().c_str());
			continue;
		}

		ss = std::stringstream(forceFieldData[i][1].getValue());
		double rotX, rotY, rotZ;

		if (!(ss >> rotX >> rotY >> rotZ))
		{
			LOG_ERROR("GAME", "invalid player rotation \"%s\"", forceFieldData[i][1].getValue().c_str());
			continue;
		}

		forceFieldTransforms.push_back({ { posX, posY, posZ }, { rotX, rotY, rotZ } });
	}
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

				paddleIA->getComponent<IAPaddle>()->setId(i + nPlayers + 1);
				paddleIA->getComponent<Health>()->setHealth(gameManager->getHealth());

				paddles.push_back(paddleIA);
			}
			else
			{
				GameObject* wall = instantiate("Wall", playerTransforms[i + nPlayers].first);
				wall->transform->setRotation(playerTransforms[i + nPlayers].second);
				wall->setActive(true);
			}
		}
	}
	win = false;
	gameManager->setPlayersAlive(paddles.size());
	gameManager->setTotalPlayers(paddles.size());
	gameManager->getScore()->initScore(paddles.size());
}

void Game::createSpawners()
{
	std::vector<GameObject*> aux;

	int n = spawnerTransforms.size();

	for (int i = 0; i < n; i++)
	{
		GameObject* spawner = instantiate("Spawner", spawnerTransforms[i].first);
		spawner->transform->setRotation(spawnerTransforms[i].second);
		spawner->setActive(true);
		spawner->getChildren()[0]->setActive(true);

		aux.push_back(spawner);
	}

	findGameObjectWithName("SpawnerManager")->getComponent<SpawnerManager>()->setSpawners(aux);
}

void Game::createForceField()
{
	int n = forceFieldTransforms.size();

	for (int i = 0; i < n; i++)
	{
		GameObject* forceField = instantiate("ForceField", forceFieldTransforms[i].first);
		forceField->transform->setRotation(forceFieldTransforms[i].second);
	}
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
	if (!win)
	{
		for (int i = 0; i < paddles.size(); i++)
		{
			Health* health = paddles[i]->getComponent<Health>();
			if (health != nullptr && health->isAlive())
			{
				if (health->getHealth() > majorHealth)
				{
					majorHealth = health->getHealth();
					majorIndex = i;
					tie = false;
				}
				else if (health->getHealth() == majorHealth)
					tie = true;
			}
		}
	}

	if (gameLayout != nullptr)
	{
		winnerPanel.setVisible(true);
		for (int i = 0; i < paddles.size(); i++)
		{
			int pos = 1;
			Health* health = paddles[i]->getComponent<Health>();
			if (health->isAlive())
			{
				for (int j = 0; j < paddles.size(); j++)
				{
					Health* health2 = paddles[j]->getComponent<Health>();
					if (health2->getHealth() > health->getHealth())
						pos++;
				}
				gameManager->getScore()->setTimeAlive(i + 1, GameManager::GetInstance()->getInitialTime(), GameManager::GetInstance()->getTime());
				gameManager->getScore()->setPositionOnLeaderBoard(i + 1, pos);
			}
		}
		if (tie)
		{
			winnerText.setText("TIE");
			
		}
		else
		{
			
			if (!win)
			{
				winner = majorIndex;
			}
			win = true;
			gameManager->getScore()->setTimeAlive(winner + 1, GameManager::GetInstance()->getInitialTime(), GameManager::GetInstance()->getTime());
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
	createSpawners();
	createForceField();
	playSong();

	gameTimer = gameManager->getTime();
}

void Game::update(float deltaTime)
{
	if (gameTimer > 0)
	{
		gameTimer -= deltaTime;
		gameManager->setTime((int)gameTimer);
		if (gameTimer <= 0.0f)
			chooseWinner();

		if (gameLayout != nullptr)
			timeText.setText(std::to_string((int)gameTimer % 60));
	}
	else
	{
		finishTimer -= deltaTime;
		
		if (finishTimer <= 0.0f)
			SceneManager::GetInstance()->changeScene("LeaderBoardMenu"); // Cambiar a menu de final de partida
	}

	if (gameManager->getPlayersAlive() == 1)
		chooseWinner();
}