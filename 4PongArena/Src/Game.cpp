#include "Game.h"
#include <GaiaData.h>
#include <SceneManager.h>
#include <GameObject.h>
#include <RigidBody.h>
#include <MeshRenderer.h>
#include <Strider.h>
#include <UILayout.h>

#include "PlayerController.h"
#include "PlayerIndex.h"
#include "IAPaddle.h"
#include "Health.h"
#include "SpawnerManager.h"
#include "GameManager.h"
#include "Death.h"

#include <ComponentRegister.h>

REGISTER_FACTORY(Game);

void Game::createLevel()
{
	GaiaData levelData;

	// LEVEL BASE
	levelData.load("./Assets/Levels/Bases.level");

	std::string renderName = levelData[levelBase].find("RenderMesh").getValue();
	std::string colliderName = levelData[levelBase].find("ColliderMesh").getValue();

	// collider mesh
	configureLevelCollider(colliderName);

	// render mesh
	configureLevelRender(renderName);

	// player initial transforms
	GaiaData playerData = levelData[levelBase].find("PlayerTransforms");
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
	GaiaData spawnerData = levelData[levelBase].find("SpawnerTransforms");
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

	// LEVEL FORCE FIELDS
	if (levelForces > 0)
	{
		levelData.load("./Assets/Levels/ForceFields.level");

		// force field initial transforms
		GaiaData forceFieldData = levelData[levelForces - 1].find("ForceFieldTransforms");
		for (int i = 0; i < forceFieldData.size(); i++)
		{
			std::stringstream ss(forceFieldData[i][0].getValue());
			double posX, posY, posZ;

			if (!(ss >> posX >> posY >> posZ))
			{
				LOG_ERROR("GAME", "invalid force field position \"%s\"", forceFieldData[i][0].getValue().c_str());
				continue;
			}

			ss = std::stringstream(forceFieldData[i][1].getValue());
			double escX, escY, escZ;

			if (!(ss >> escX >> escY >> escZ))
			{
				LOG_ERROR("GAME", "invalid force field scale \"%s\"", forceFieldData[i][1].getValue().c_str());
				continue;
			}

			forceFieldTransforms.push_back({ { posX, posY, posZ }, { escX, escY, escZ } });
		}
	}

	// LEVEL OBSTACLES
	if (levelObstacles > 0)
	{
		levelData.load("./Assets/Levels/Obstacles.level");

		// obstacles initial transforms
		GaiaData obstacleData = levelData[levelObstacles - 1].find("ObstacleTransforms");
		for (int i = 0; i < obstacleData.size(); i++)
		{
			std::stringstream ss(obstacleData[i][0].getValue());
			double posX, posY, posZ;

			if (!(ss >> posX >> posY >> posZ))
			{
				LOG_ERROR("GAME", "invalid obstacle position \"%s\"", obstacleData[i][0].getValue().c_str());
				continue;
			}

			ss = std::stringstream(obstacleData[i][1].getValue());
			double escX, escY, escZ;

			if (!(ss >> escX >> escY >> escZ))
			{
				LOG_ERROR("GAME", "invalid obstacle scale \"%s\"", obstacleData[i][1].getValue().c_str());
				continue;
			}

			obstacleTransforms.push_back({ { posX, posY, posZ }, { escX, escY, escZ } });
		}
	}
}

void Game::createPlayers()
{
	std::vector<Player> players = gameManager->getPlayers();

	int nPlayers = players.size();

	for (int i = 0; i < nPlayers; i++)
	{
		GameObject* paddle = instantiate("Paddle", playerTransforms[i].first);
		paddle->getComponent<RigidBody>()->setGravity(Vector3(0, 0, 0));
		paddle->transform->setRotation(playerTransforms[i].second);

		paddle->getComponent<PlayerController>()->setPlayer(players[i].id, players[i].index);
		paddle->getComponent<PlayerIndex>()->setId(players[i].id);

		paddle->getComponent<Health>()->setHealth(gameManager->getHealth());
		paddle->getComponent<Death>()->setPlayerColour(playerColours[i]);

		paddle->getComponent<MeshRenderer>()->setDiffuse(0, playerColours[i], 1);
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
				paddleIA->getComponent<RigidBody>()->setGravity(Vector3(0, 0, 0));

				paddleIA->getComponent<PlayerIndex>()->setId(i + nPlayers + 1);

				paddleIA->getComponent<Health>()->setHealth(gameManager->getHealth());
				paddleIA->getComponent<Death>()->setPlayerColour(playerColours[i + nPlayers]);

				paddleIA->getComponent<MeshRenderer>()->setDiffuse(0, playerColours[i + nPlayers], 1);
				paddles.push_back(paddleIA);
			}
			else
			{
				GameObject* wall = instantiate("Wall", playerTransforms[i + nPlayers].first);
				wall->transform->setRotation(playerTransforms[i + nPlayers].second);
				RigidBody* wallRigidBody = wall->getComponent<RigidBody>();

				wallRigidBody->setStatic(true);
				wallRigidBody->setFriction(0.5f);
				wallRigidBody->setActive(true);
			}
		}
	}

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
		forceField->transform->setScale(forceFieldTransforms[i].second);
		forceField->getComponent<RigidBody>()->multiplyScale(forceFieldTransforms[i].second);
		forceField->setActive(true);
	}
}

void Game::createObstacles()
{
	int n = obstacleTransforms.size();

	for (int i = 0; i < n; i++)
	{
		GameObject* obstacle = instantiate("Obstacle", obstacleTransforms[i].first);
		obstacle->transform->setScale(obstacleTransforms[i].second);
		obstacle->setActive(true);
	}
}

void Game::configureLevelRender(const std::string& name)
{
	GameObject* levelRender = findGameObjectWithName("LevelRender");
	if (levelRender == nullptr)
	{
		LOG_ERROR("GAME", "LevelRender object not found on scene");
		return;
	}

	MeshRenderer* meshRenderer = levelRender->getComponent<MeshRenderer>();
	if (meshRenderer == nullptr)
	{
		LOG_ERROR("GAME", "MeshRenderer not found"); return;
	}

	meshRenderer->setMesh("levelRender", name);
	meshRenderer->attachEntityToNode("levelRender");
}

void Game::configureLevelCollider(const std::string& name)
{
	GameObject* levelCollider = findGameObjectWithName("LevelCollider");
	if (levelCollider == nullptr)
	{
		LOG_ERROR("GAME", "LevelCollider object not found on scene"); return;
	}

	MeshRenderer* meshRenderer = levelCollider->getComponent<MeshRenderer>();
	if (meshRenderer == nullptr)
	{
		LOG_ERROR("GAME", "MeshRenderer not found"); return;
	}

	Strider* strider = levelCollider->getComponent<Strider>();
	if (strider == nullptr)
	{
		LOG_ERROR("GAME", "Strider not found"); return;
	}

	meshRenderer->setMesh("levelCollider", name);
	meshRenderer->attachEntityToNode("levelCollider");
	meshRenderer->setVisible(false);
	strider->stride("levelCollider");
	strider->setFriction(0.5f);
}

void Game::playSong()
{
	//findGameObjectWithName("MainCamera")->getComponent<SoundEmitter>()->play(GameManager::GetInstance()->getSong());
}

void Game::chooseWinner()
{
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
				tie = false;
			}
			else if (health->getHealth() == majorHealth)
				tie = true;
		}
	}

	if (gameLayout != nullptr)
	{
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
				gameManager->getScore()->setTimeAlive(i + 1, gameManager->getInitialTime(), gameManager->getTime());
				gameManager->getScore()->setPositionOnLeaderBoard(i + 1, pos);
			}
		}
	}

	SceneManager::GetInstance()->changeScene("LeaderBoardMenu");
}

Game::Game(GameObject* gameObject) : UserComponent(gameObject), gameManager(nullptr), gameLayout(nullptr), timeText(NULL), gameTimer(0), levelBase(0), levelForces(0), levelObstacles(0)
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
		timeText = gameLayout->getRoot().getChild("Time");

	playerColours = gameManager->getPlayerColours();

	gameTimer = gameManager->getTime();

	levelBase = gameManager->getLevelBase();
	levelForces = gameManager->getLevelForces();
	levelObstacles = gameManager->getLevelObstacles();

	createLevel();
	createSpawners();
	createForceField();
	createObstacles();
	createPlayers();

	playSong();
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

	if (gameManager->getPlayersAlive() == 1)
		chooseWinner();
}