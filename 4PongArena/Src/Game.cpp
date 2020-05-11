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
#include "Death.h"
#include "Countdown.h"
#include "SpawnerManager.h"
#include "GameManager.h"

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
	std::vector<int> indexes = gameManager->getPlayerIndexes();
	gameManager->getPaddles().clear();

	int aux = 0;

	for (int i = 0; i < indexes.size(); i++)
	{
		if (indexes[i] != -1)
		{
			GameObject* paddle = instantiate("Paddle", playerTransforms[i].first);
			paddle->getComponent<RigidBody>()->setGravity(Vector3(0, 0, 0));
			paddle->transform->setRotation(playerTransforms[i].second);

			paddle->getComponent<PlayerController>()->setIndex(indexes[i]);
			paddle->getComponent<PlayerIndex>()->setId(i + 1);

			paddle->getComponent<Health>()->setHealth(gameManager->getHealth());
			paddle->getComponent<Death>()->setPlayerColour(playerColours[i]);

			paddle->getComponent<MeshRenderer>()->setDiffuse(0, playerColours[i], 1);

			paddles.push_back(paddle);
			gameManager->getPaddles().push_back(paddle);

			aux++;
		}
	}

	int nUnfilled = MAX_PLAYERS - aux;

	if (nUnfilled > 0)
	{
		for (int i = 0; i < nUnfilled; i++)
		{
			if (/*gameManager->getIA()*/false)
			{
				GameObject* paddleIA = instantiate("IA", playerTransforms[i + aux].first);
				paddleIA->transform->setRotation(playerTransforms[i + aux].second);
				paddleIA->getComponent<RigidBody>()->setGravity(Vector3(0, 0, 0));

				paddleIA->getComponent<PlayerIndex>()->setId(i + aux + 1);

				paddleIA->getComponent<Health>()->setHealth(gameManager->getHealth());
				paddleIA->getComponent<Death>()->setPlayerColour(playerColours[i + aux]);

				paddleIA->getComponent<MeshRenderer>()->setDiffuse(0, playerColours[i + aux], 1);

				paddles.push_back(paddleIA);
				gameManager->getPaddles().push_back(paddleIA);
			}
			else
			{
				GameObject* wall = instantiate("Wall", playerTransforms[i + aux].first);
				wall->transform->setRotation(playerTransforms[i + aux].second);
				RigidBody* wallRigidBody = wall->getComponent<RigidBody>();

				wallRigidBody->setStatic(true);
				wallRigidBody->setFriction(0.5f);
				wallRigidBody->setActive(true);
			}
		}
	}

	gameManager->initPlayerRanking(paddles.size());

	gameManager->setPlayersAlive(paddles.size());
	gameManager->setInitialPlayers(paddles.size());

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
	int tieIndex = 0;

	for (int i = 0; i < paddles.size(); i++)
	{
		Health* health = paddles[i]->getComponent<Health>();
		if (health == nullptr)
			continue;

		if (health->isAlive())
		{
			if (health->getHealth() > majorHealth)
			{
				majorHealth = health->getHealth();
				majorIndex = i;
				tie = false;
			}
			else if (health->getHealth() == majorHealth)
			{
				tieIndex = i;
				tie = true;
			}
		}
	}

	if (tie)
	{
		for (int i = 0; i < paddles.size(); i++)
		{
			if (i == majorIndex || i == tieIndex)
			{
				gameManager->getScore()->setTimeAlive(majorIndex, gameManager->getInitialTime(), gameManager->getTime());
				gameManager->setPlayerRanking(i + 1, 1);
			}
			else
				gameManager->setPlayerRanking(i + 1, gameManager->getPlayerRanking(i + 1) - 1);
		}
		gameManager->setWinner(-1);
	}
	else
	{
		gameManager->getScore()->setTimeAlive(majorIndex + 1, gameManager->getInitialTime(), gameManager->getTime());
		gameManager->setPlayerRanking(majorIndex + 1, 1);
		gameManager->setWinner(majorIndex + 1);
	}

	SceneManager::GetInstance()->changeScene("ScoreMenu");
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

	countdown = findGameObjectWithName("Countdown")->getComponent<Countdown>();

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
	if (!countdown->isCounting() && gameTimer > 0)
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