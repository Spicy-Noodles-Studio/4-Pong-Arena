#include "Game.h"
#include <GaiaData.h>
#include <SceneManager.h>
#include <GameObject.h>
#include <RigidBody.h>
#include <MeshRenderer.h>
#include <Strider.h>
#include <UILayout.h>
#include <SoundEmitter.h>
#include <Trail.h>

#include "PlayerController.h"
#include "PlayerIndex.h"
#include "IAPaddle.h"
#include "Health.h"
#include "Death.h"
#include "Countdown.h"
#include "SpawnerManager.h"
#include "GameManager.h"
#include "PlayerIndex.h"
#include "Death.h"
#include "CameraEffects.h"

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
	if (gameManager == nullptr) return;

	std::vector<int> indexes = gameManager->getPlayerIndexes();
	gameManager->getPaddles().clear();
	gameManager->getScore()->clearIds();
	int posInScore = 0;
	for (int i = 0; i < indexes.size(); i++)
	{
		if (indexes[i] != -1)
		{
			players++;
			GameObject* paddle;

			if (indexes[i] != 9)
			{
				paddle = instantiate("Paddle", playerTransforms[i].first);
				if (paddle != nullptr) {
					paddle->setName("Paddle" + std::to_string(i) + std::to_string(levelBase));
					if (paddle->getComponent<PlayerController>() != nullptr) paddle->getComponent<PlayerController>()->setIndex(indexes[i]);
				}
			}
			else // fill with a wall (no player)
			{
				paddle = instantiate("IA", playerTransforms[i].first);
				if (paddle != nullptr) paddle->setName("PaddleIA" + std::to_string(i) + std::to_string(levelBase));
			}

			if (paddle != nullptr) {
				paddle->getComponent<RigidBody>()->setGravity(Vector3(0, 0, 0));
				paddle->transform->setRotation(playerTransforms[i].second);

				if (paddle->getComponent<PlayerIndex>() != nullptr)
				{
					paddle->getComponent<PlayerIndex>()->setId(i + 1);
					paddle->getComponent<PlayerIndex>()->setPos(posInScore);
					posInScore++;
				}
				if (paddle->getComponent<Health>() != nullptr) paddle->getComponent<Health>()->setHealth(gameManager->getHealth());
				if (paddle->getComponent<MeshRenderer>() != nullptr) paddle->getComponent<MeshRenderer>()->setDiffuse(0, playerColours[i], 1);
				if (paddle->getComponent<Trail>() != nullptr) paddle->getComponent<Trail>()->setColour(playerColours[i], 1.0);
			}

			
			if (paddle != nullptr) {
				Death* death = paddle->getComponent<Death>();
				death->setPlayerColour(playerColours[i]);
				death->setwallColours(baseColour, neonColour);
				death->setWallScale(wallScale);
			}

			if (paddle != nullptr) paddles.push_back(paddle);
			if (gameManager != nullptr)
			{
				gameManager->getPaddles().push_back(paddle);
				if (gameManager->getScore() != nullptr) gameManager->getScore()->pushPlayerId(i + 1);
			}
		}
		else
		{
			GameObject* wall = instantiate("Wall", playerTransforms[i].first);
			if (wall != nullptr) {
				wall->setName("Wall" + std::to_string(i) + std::to_string(levelBase));
				wall->transform->setRotation(playerTransforms[i].second);
				wall->transform->setScale(wallScale);
			}

			RigidBody* wallRigidBody = wall->getComponent<RigidBody>();
			if (wallRigidBody == nullptr)
			{
				LOG_ERROR("GAME", "Rigidbody of wall not found"); return;
			}
			if (wallRigidBody != nullptr) {
				wallRigidBody->setStatic(true);
				wallRigidBody->setFriction(0.5f);
				wallRigidBody->setActive(true);
			}

			MeshRenderer* wallMesh = wall->getComponent<MeshRenderer>();
			if (wallMesh == nullptr)
			{
				LOG_ERROR("GAME", "MeshRenderer of wall not found"); return;
			}
			if (wallMesh != nullptr) {
				wallMesh->setDiffuse(0, playerColours[i], 1);

				wallMesh->setDiffuse(2, neonColour.first, 1);
				wallMesh->setEmissive(2, neonColour.second);

				wallMesh->setDiffuse(1, baseColour.first, 1);
				wallMesh->setEmissive(1, baseColour.second);
			}
		}
	}

	if (gameManager != nullptr)
	{
		gameManager->setInitialPlayers(paddles.size());
		if (gameManager->getScore() != nullptr) gameManager->getScore()->initScore(paddles.size());
	}
}

void Game::createSpawners()
{
	std::vector<GameObject*> aux;

	int n = spawnerTransforms.size();

	for (int i = 0; i < n; i++)
	{
		GameObject* spawner = instantiate("Spawner", spawnerTransforms[i].first);
		if (spawner != nullptr) {
			spawner->setName("Spawner" + std::to_string(levelBase));
			spawner->transform->setRotation(spawnerTransforms[i].second);

			MeshRenderer* spawnerMesh = spawner->getComponent<MeshRenderer>();
			if (spawnerMesh == nullptr)
			{
				LOG_ERROR("GAME", "MeshRenderer of spawner not found"); return;
			}
			if (spawnerMesh != nullptr) {
				spawnerMesh->setDiffuse(0, neonColour.first, 1);
				spawnerMesh->setEmissive(0, neonColour.second);

				spawnerMesh->setDiffuse(1, baseColour.first, 1);
				spawnerMesh->setEmissive(1, baseColour.second);
			}
			if (spawner != nullptr) {
				spawner->setActive(true);
				aux.push_back(spawner);
			}
		}
	}

	if (findGameObjectWithName("SpawnerManager") != nullptr && findGameObjectWithName("SpawnerManager")->getComponent<SpawnerManager>() != nullptr) 
		findGameObjectWithName("SpawnerManager")->getComponent<SpawnerManager>()->setSpawners(aux);
}

void Game::createForceField()
{
	int n = forceFieldTransforms.size();

	for (int i = 0; i < n; i++)
	{
		GameObject* forceField = instantiate("ForceField", forceFieldTransforms[i].first);
		if (forceField != nullptr) {
			forceField->transform->setScale(forceFieldTransforms[i].second);
			if (forceField->getComponent<RigidBody>() != nullptr) forceField->getComponent<RigidBody>()->multiplyScale(forceFieldTransforms[i].second);
			forceField->setActive(true);
		}
	}
}

void Game::createObstacles()
{
	int n = obstacleTransforms.size();

	for (int i = 0; i < n; i++)
	{
		GameObject* obstacle = instantiate("Obstacle", obstacleTransforms[i].first);
		if (obstacle != nullptr) {
			obstacle->setName("Obstacle" + std::to_string(levelBase));
			if (obstacle->transform != nullptr) obstacle->transform->setScale(obstacleTransforms[i].second);
			obstacle->setActive(true);
		}
		MeshRenderer* obstacleMesh = obstacle->getComponent<MeshRenderer>();
		if (obstacleMesh == nullptr)
		{
			LOG_ERROR("GAME", "MeshRenderer of obstacle not found"); return;
		}
		if (obstacleMesh != nullptr) {
			obstacleMesh->setDiffuse(0, neonColour.first, 1);
			obstacleMesh->setEmissive(0, neonColour.second);

			obstacleMesh->setDiffuse(1, baseColour.first, 1);
			obstacleMesh->setEmissive(1, baseColour.second);
		}
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
	if (gameManager == nullptr) return;

	gameManager->playMusic(gameManager->getSong());
	if (gameManager->getSong() == "DefenseMatrix") gameManager->setMusicVolume(0.5);
	else gameManager->setMusicVolume(0.4);
}

void Game::endHandleSound()
{
	if (gameManager == nullptr || soundEmitter == nullptr) return;
	gameManager->setMusicVolume(0.2);
	soundEmitter->setVolume(1.5);
	soundEmitter->playSound("Game_End");
}

void Game::setRanking()
{
	if (gameManager == nullptr) return;

	for (int i = 0; i < paddles.size(); i++)
	{
		Health* health = paddles[i]->getComponent<Health>();
		if (health != nullptr)
			gameManager->getRanking().push(ii(i + 1, health->getHealth()));

		if (paddles[i]->getComponent<PlayerController>() != nullptr)
			paddles[i]->getComponent<PlayerController>()->setActive(false);
		else if (paddles[i]->getComponent<IAPaddle>() != nullptr)
			paddles[i]->getComponent<IAPaddle>()->setActive(false);
		if(health != nullptr&&health->getHealth()>0)
			if(gameManager->getScore()!=nullptr)
			gameManager->getScore()->setTimeAlive(paddles[i]->getComponent<PlayerIndex>()->getPosVector() , gameManager->getInitialTime(), gameManager->getTime());
	}

	std::priority_queue<ii, std::vector<ii>, Less> aux = gameManager->getRanking();

	int cont = 0;
	bool tie = false;
	ii last = ii(0, 0);

	while (!aux.empty())
	{
		ii info = aux.top();
		aux.pop();

		if (info.second != 0 && info.second == last.second)
			tie = true;
		else
			cont++;

		gameManager->setPlayerRanking(info.first, cont);
		last = info;
	}

	if (tie)
		gameManager->setWinner(-1);
	else
	{
		gameManager->setWinner(gameManager->getRanking().top().first);
	}

	gameManager->emptyRanking();
}

void Game::chooseWinner()
{
	end = true;
	setRanking();

	GameObject* spawnerObject = findGameObjectWithName("SpawnerManager");
	if (spawnerObject != nullptr)
	{
		SpawnerManager* spawnerManager = spawnerObject->getComponent<SpawnerManager>();
		if (spawnerManager != nullptr)
			spawnerManager->deactivateAll();
	}

	gameManager->setGameEnded(true);
	gameManager->stopMusic(gameManager->getSong());

	endHandleSound();
	cameraEffects->fadeOut();
}

Game::Game(GameObject* gameObject) : UserComponent(gameObject), gameManager(nullptr), soundEmitter(nullptr), countdown(nullptr), cameraEffects(nullptr), gameLayout(nullptr), timePanel(NULL),
players(0), winner(0), gameTimer(0), levelBase(0), levelForces(0), levelObstacles(0), fadeIn(true), darkness(false), end(false)
{

}

Game::~Game()
{
	gameManager = nullptr;
	soundEmitter = nullptr;
	gameLayout = nullptr;
	timePanel = nullptr;
	countdown = nullptr;
	cameraEffects = nullptr;
	paddles.clear();
	playerColours.clear();
	levelColours.clear();
	playerTransforms.clear();
	spawnerTransforms.clear();
	forceFieldTransforms.clear();
	obstacleTransforms.clear();
}

void Game::playerDeath()
{
	players--;

	if (players <= 1)
		chooseWinner();
}

float Game::getTime()
{
	return gameTimer;
}

void Game::start()
{
	if (gameObject == nullptr) return;

	gameManager = GameManager::GetInstance();

	if (gameManager == nullptr) return;

	gameManager->setGameEnded(false);
	soundEmitter = gameObject->getComponent<SoundEmitter>();

	GameObject* mainCamera = findGameObjectWithName("MainCamera");

	if (mainCamera != nullptr) {
		gameLayout = mainCamera->getComponent<UILayout>();
		cameraEffects = mainCamera->getComponent<CameraEffects>();
	}

	if (gameLayout != nullptr)
		timePanel = gameLayout->getRoot().getChild("TimeBackground");

	if (findGameObjectWithName("Countdown") != nullptr) countdown = findGameObjectWithName("Countdown")->getComponent<Countdown>();

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
	if (countdown != nullptr && gameManager != nullptr)
	{
		if (!countdown->isCounting() && gameTimer > 0)
		{
			if (!timePanel.isVisible())
			{
				timePanel.setVisible(true);
				timePanel.setAlwaysOnTop(true);
			}

			timePanel.getChild("Time").setText(timeToText().first + " : " + timeToText().second);

			gameTimer -= deltaTime;
			if (gameTimer <= 0)
				gameTimer = 0;

			gameManager->setTime((int)gameTimer);
			if (gameTimer <= 0.0f && !gameManager->isGameEnded() )
				chooseWinner();
			
		}
		else if(!countdown->isCounting() && gameTimer < 0)
		{
			gameTimer -= deltaTime;
			gameManager->setTime((int)gameTimer);
			if (!gameManager->isGameEnded() && players <= 1)
				chooseWinner();
		}

		
	}

	if (cameraEffects != nullptr)
	{
		if (!darkness)
		{
			cameraEffects->setDarkness();
			darkness = true;
		}
		else if (countdown != nullptr && countdown->getRemainingTime() < 2.6 && fadeIn)
		{
			cameraEffects->fadeIn();
			fadeIn = false;
		}

		if (end && !cameraEffects->isFading())
			if (SceneManager::GetInstance() != nullptr) SceneManager::GetInstance()->changeScene("ScoreMenu");
	}
}

std::pair<std::string, std::string> Game::timeToText()
{
	std::string minutes = std::to_string((int)gameTimer / 60);
	std::string seconds;

	if ((int)gameTimer % 60 < 10)
		seconds = "0" + std::to_string((int)gameTimer % 60);
	else
		seconds = std::to_string((int)gameTimer % 60);

	return std::pair<std::string, std::string>(minutes, seconds);
}