#include "Game.h"
#include <GaiaData.h>
#include <SceneManager.h>
#include <UILayout.h>
#include <GameObject.h>
#include <RigidBody.h>
#include <MeshRenderer.h>
#include <Strider.h>
#include <SoundEmitter.h>

#include "PlayerController.h"
#include "IAPaddle.h"
#include "Health.h"
#include "SpawnerManager.h"
#include "GameManager.h"
#include "PlayerIndex.h"
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

	// render mesh
	configureLevelRender(renderName);

	// collider mesh
	configureLevelCollider(colliderName);

	// wall scale
	GaiaData wallScaleData = levelData[levelBase].find("WallScale");
	std::stringstream ss(wallScaleData.getValue());
	double wEscX, wEscY, wEscZ;

	if (!(ss >> wEscX >> wEscY >> wEscZ))
		LOG_ERROR("GAME", "invalid wall scale \"%s\"", wallScaleData.getValue().c_str());
	else
		wallScale = { wEscX, wEscY, wEscZ };

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

	if (levelObstacles > 0)
	{
		// LEVEL OBSTACLES
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
}

void Game::createPlayers()
{
	std::vector<Player> players = gameManager->getPlayers();

	int nPlayers = players.size();

	for (int i = 0; i < nPlayers; i++) // fill with a player
	{
		GameObject* paddle = instantiate("Paddle", playerTransforms[i].first);
		paddle->setName("Paddle" + std::to_string(i) + std::to_string(levelBase));
		paddle->getComponent<RigidBody>()->setGravity({ 0, 0, 0 });
		paddle->transform->setRotation(playerTransforms[i].second);
		paddle->getComponent<PlayerController>()->setPlayer(players[i].id, players[i].index);
		paddle->getComponent<PlayerIndex>()->setId(players[i].id);
		paddle->getComponent<Health>()->setHealth(gameManager->getHealth());
		paddle->getComponent<MeshRenderer>()->setDiffuse(0, playerColours[i], 1);

		Death* death = paddle->getComponent<Death>();
		death->setPlayerColour(playerColours[i]);
		death->setwallColours(baseColour, neonColour);
		death->setWallScale(wallScale);

		paddles.push_back(paddle);
	}

	int nUnfilled = MAX_PLAYERS - nPlayers;

	if (nUnfilled > 0)
	{
		for (int i = 0; i < nUnfilled; i++)
		{
			if (gameManager->getIA()) // fill with IA
			{
				GameObject* paddleIA = instantiate("IA", playerTransforms[i + nPlayers].first);
				paddleIA->setName("PaddleIA" + std::to_string(i) + std::to_string(levelBase));
				paddleIA->transform->setRotation(playerTransforms[i + nPlayers].second);
				paddleIA->getComponent<RigidBody>()->setGravity({ 0, 0, 0 });
				paddleIA->getComponent<PlayerIndex>()->setId(i + nPlayers + 1);
				paddleIA->getComponent<Health>()->setHealth(gameManager->getHealth());
				paddleIA->getComponent<MeshRenderer>()->setDiffuse(0, playerColours[i + nPlayers], 1);

				Death* deathIA = paddleIA->getComponent<Death>();
				deathIA->setPlayerColour(playerColours[i + nPlayers]);
				deathIA->setwallColours(baseColour, neonColour);
				deathIA->setWallScale(wallScale);

				paddles.push_back(paddleIA);
			}
			else // fill with a wall (no player)
			{
				GameObject* wall = instantiate("Wall", playerTransforms[i + nPlayers].first);
				wall->setName("Wall" + std::to_string(levelBase));
				wall->transform->setRotation(playerTransforms[i + nPlayers].second);
				wall->transform->setScale(wallScale);

				RigidBody* wallRigidBody = wall->getComponent<RigidBody>();
				if (wallRigidBody == nullptr)
				{
					LOG_ERROR("GAME", "Rigidbody of wall not found"); return;
				}

				wallRigidBody->setStatic(true);
				wallRigidBody->setFriction(0.5f);
				wallRigidBody->setActive(true);

				MeshRenderer* wallMesh = wall->getComponent<MeshRenderer>();
				if (wallMesh == nullptr)
				{
					LOG_ERROR("GAME", "MeshRenderer of wall not found"); return;
				}

				wallMesh->setDiffuse(0, playerColours[i + nPlayers], 1);

				wallMesh->setDiffuse(2, neonColour.first, 1);
				wallMesh->setEmissive(2, neonColour.second);

				wallMesh->setDiffuse(1, baseColour.first, 1);
				wallMesh->setEmissive(1, baseColour.second);
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
		spawner->setName("Spawner" + std::to_string(levelBase));
		spawner->transform->setRotation(spawnerTransforms[i].second);

		MeshRenderer* spawnerMesh = spawner->getComponent<MeshRenderer>();
		if (spawnerMesh == nullptr)
		{
			LOG_ERROR("GAME", "MeshRenderer of spawner not found"); return;
		}

		spawnerMesh->setDiffuse(0, neonColour.first, 1);
		spawnerMesh->setEmissive(0, neonColour.second);

		spawnerMesh->setDiffuse(1, baseColour.first, 1);
		spawnerMesh->setEmissive(1, baseColour.second);
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
		obstacle->setName("Obstacle" + std::to_string(levelBase));
		obstacle->transform->setScale(obstacleTransforms[i].second);
		obstacle->setActive(true);

		MeshRenderer* obstacleMesh = obstacle->getComponent<MeshRenderer>();
		if (obstacleMesh == nullptr)
		{
			LOG_ERROR("GAME", "MeshRenderer of obstacle not found"); return;
		}

		obstacleMesh->setDiffuse(0, neonColour.first, 1);
		obstacleMesh->setEmissive(0, neonColour.second);

		obstacleMesh->setDiffuse(1, baseColour.first, 1);
		obstacleMesh->setEmissive(1, baseColour.second);
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

	meshRenderer->changeMesh("levelRender", name);

	// get colours of level render
	baseColour.first = meshRenderer->getDiffuse(4);
	baseColour.second = meshRenderer->getEmissive(4);

	neonColour.first = meshRenderer->getDiffuse(3);
	neonColour.second = meshRenderer->getEmissive(3);
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
	gameManager->playMusic(gameManager->getSong());
	if (gameManager->getSong() == "DefenseMatrix") gameManager->setMusicVolume(0.5);
	else gameManager->setMusicVolume(0.4);
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
				gameManager->getScore()->setTimeAlive(i + 1, gameManager->getInitialTime(), gameManager->getTime());
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
			winnerText.setText("WINNER: P" + std::to_string(winner + 1));
		}
	}
}

void Game::endgameHandleSound()
{
	gameManager->setMusicVolume(0.2);
	soundEmitter->setVolume(1.5);
	soundEmitter->playSound("Game_End");
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
	gameManager->setGameEnded(false);
	soundEmitter = gameObject->getComponent<SoundEmitter>();

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
	playerColours = gameManager->getPlayerColours();
	levelBase = gameManager->getLevelBase();
	levelObstacles = gameManager->getLevelObstacles();
	levelForces = gameManager->getLevelForces();

	createLevel();
	createPlayers();
	createSpawners();
	createForceField();
	createObstacles();
	playSong();

	gameTimer = gameManager->getTime();
}

void Game::update(float deltaTime)
{
	if (gameTimer > 0)
	{
		gameTimer -= deltaTime;
		gameManager->setTime((int)gameTimer);
		if (gameTimer <= 0.0f && !gameManager->isGameEnded()) {
			gameManager->setGameEnded(true);
			endgameHandleSound();
			chooseWinner();
		}

		if (gameLayout != nullptr)
			timeText.setText(std::to_string((int)gameTimer % 60));
	}
	else if (gameTimer == 0)
	{
		finishTimer -= deltaTime;

		if (finishTimer <= 0.0f) {
			gameManager->setGameEnded(false);
			SceneManager::GetInstance()->changeScene("LeaderBoardMenu"); // Cambiar a menu de final de partida
		}
	}

	if (gameManager->getPlayersAlive() == 1 && !gameManager->isGameEnded()) {
		gameManager->setGameEnded(true);
		endgameHandleSound();
		chooseWinner();
	}
}