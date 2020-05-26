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
	checkNullAndBreak(gameManager);

	std::vector<int> indexes = gameManager->getPlayerIndexes();
	gameManager->getPaddles().clear();
	Score* score = gameManager->getScore();
	if (notNull(score)) score->clearIDs();

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
				if (notNull(paddle)) {
					paddle->setName("Paddle" + std::to_string(i) + std::to_string(levelBase));

					PlayerController* playerController = paddle->getComponent<PlayerController>();
					if (notNull(playerController)) playerController->setIndex(indexes[i]);
				}
			}
			else // fill with a wall (no player)
			{
				paddle = instantiate("IA", playerTransforms[i].first);
				if (notNull(paddle)) paddle->setName("PaddleIA" + std::to_string(i) + std::to_string(levelBase));
			}

			if (notNull(paddle)) {
				if (notNull(paddle->transform)) paddle->transform->setRotation(playerTransforms[i].second);

				RigidBody* rigidBody = paddle->getComponent<RigidBody>();
				if (notNull(rigidBody))
					rigidBody->setGravity(Vector3::ZERO);

				PlayerIndex* index = paddle->getComponent<PlayerIndex>();
				if (notNull(index))
				{
					index->setId(i + 1);
					index->setPos(posInScore);
					posInScore++;
				}

				Health* health = paddle->getComponent<Health>();
				if (notNull(health)) health->setHealth(gameManager->getHealth());

				MeshRenderer* meshRenderer = paddle->getComponent<MeshRenderer>();
				if (notNull(meshRenderer) && i < playerColours.size()) meshRenderer->setDiffuse(0, playerColours[i], 1);

				Trail* trail = paddle->getComponent<Trail>();
				if (notNull(trail)) trail->setColour(playerColours[i], 1.0);

				Death* death = paddle->getComponent<Death>();
				if (notNull(death)) {
					death->setPlayerColour(playerColours[i]);
					death->setwallColours(baseColour, neonColour);
					death->setWallScale(wallScale);
				}

				paddles.push_back(paddle);
			}

			gameManager->getPaddles().push_back(paddle);
			if (notNull(score)) score->getPlayerIDs().push_back(i + 1);
		}
		else
		{
			GameObject* wall = instantiate("Wall", playerTransforms[i].first);
			checkNullAndBreak(wall);

			if (notNull(wall->transform)) {
				wall->setName("Wall" + std::to_string(i) + std::to_string(levelBase));
				wall->transform->setRotation(playerTransforms[i].second);
				wall->transform->setScale(wallScale);
			}

			RigidBody* wallRigidBody = wall->getComponent<RigidBody>();
			if (notNull(wallRigidBody)) {
				wallRigidBody->setStatic(true);
				wallRigidBody->setFriction(0.5f);
				wallRigidBody->setActive(true);
			}

			MeshRenderer* wallMesh = wall->getComponent<MeshRenderer>();
			if (notNull(wallMesh)) {
				wallMesh->setDiffuse(0, playerColours[i], 1);

				wallMesh->setDiffuse(2, neonColour.first, 1);
				wallMesh->setEmissive(2, neonColour.second);

				wallMesh->setDiffuse(1, baseColour.first, 1);
				wallMesh->setEmissive(1, baseColour.second);
			}
		}
	}

	gameManager->setInitialPlayers(paddles.size());
	if (notNull(score)) score->initScore(paddles.size());
}

void Game::createSpawners()
{
	std::vector<GameObject*> aux;

	for (int i = 0; i < spawnerTransforms.size(); i++)
	{
		GameObject* spawner = instantiate("Spawner", spawnerTransforms[i].first);
		if (notNull(spawner)) {
			spawner->setName("Spawner" + std::to_string(levelBase));

			if (notNull(spawner->transform))
				spawner->transform->setRotation(spawnerTransforms[i].second);

			MeshRenderer* spawnerMesh = spawner->getComponent<MeshRenderer>();
			if (notNull(spawnerMesh)) {
				spawnerMesh->setDiffuse(0, neonColour.first, 1);
				spawnerMesh->setEmissive(0, neonColour.second);

				spawnerMesh->setDiffuse(1, baseColour.first, 1);
				spawnerMesh->setEmissive(1, baseColour.second);
			}

			spawner->setActive(true);
			aux.push_back(spawner);
		}
	}

	GameObject* spawnerObject = findGameObjectWithName("SpawnerManager");
	if (notNull(spawnerObject)) {
		SpawnerManager* spawnerManager = spawnerObject->getComponent<SpawnerManager>();
		if (notNull(spawnerManager))
			spawnerManager->setSpawners(aux);
	}
}

void Game::createForceField()
{
	for (int i = 0; i < forceFieldTransforms.size(); i++)
	{
		GameObject* forceField = instantiate("ForceField", forceFieldTransforms[i].first);
		if (notNull(forceField)) {
			if (notNull(forceField->transform))
				forceField->transform->setScale(forceFieldTransforms[i].second);

			RigidBody* rigidbody = forceField->getComponent<RigidBody>();
			if (notNull(rigidbody)) rigidbody->multiplyScale(forceFieldTransforms[i].second);

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
		checkNullAndBreak(obstacle);

		obstacle->setName("Obstacle" + std::to_string(levelBase));
		if (notNull(obstacle->transform)) obstacle->transform->setScale(obstacleTransforms[i].second);
		obstacle->setActive(true);

		MeshRenderer* obstacleMesh = obstacle->getComponent<MeshRenderer>();
		if (notNull(obstacleMesh)) {
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
	checkNullAndBreak(levelRender);

	MeshRenderer* meshRenderer = levelRender->getComponent<MeshRenderer>();
	checkNullAndBreak(meshRenderer);

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
	checkNullAndBreak(levelCollider);

	MeshRenderer* meshRenderer = levelCollider->getComponent<MeshRenderer>();
	checkNullAndBreak(meshRenderer);

	Strider* strider = levelCollider->getComponent<Strider>();
	checkNullAndBreak(strider);

	meshRenderer->setMesh("levelCollider", name);
	meshRenderer->attachEntityToNode("levelCollider");
	meshRenderer->setVisible(false);
	strider->stride("levelCollider");
	strider->setFriction(0.5f);
}

void Game::playSong()
{
	checkNullAndBreak(gameManager);

	gameManager->playMusic(gameManager->getSong());
	gameManager->setMusicVolume(0.4);
}

void Game::endHandleSound()
{
	checkNullAndBreak(gameManager);
	checkNullAndBreak(soundEmitter);

	gameManager->setMusicVolume(0.2);
	soundEmitter->setVolume(1.5);
	soundEmitter->playSound("Game_End");
}

void Game::setRanking()
{
	checkNullAndBreak(gameManager);

	for (int i = 0; i < paddles.size(); i++)
	{
		if (!notNull(paddles[i])) continue;

		Health* health = paddles[i]->getComponent<Health>();
		PlayerController* paddleController = paddles[i]->getComponent<PlayerController>();
		IAPaddle* IAController = paddles[i]->getComponent<IAPaddle>();
		PlayerIndex* index = paddles[i]->getComponent<PlayerIndex>();
		Score* score = gameManager->getScore();

		if (notNull(health) && health->getHealth() > 0) {
			gameManager->getRanking().push(ii(i + 1, health->getHealth()));
			if (notNull(score) && notNull(index))
				score->setTimeAlive(index->getPosVector(), gameManager->getInitialTime(), gameManager->getTime());
		}
		if (paddleController != nullptr)
			paddleController->setActive(false);
		else if (IAController != nullptr)
			IAController->setActive(false);
	}

	std::priority_queue<ii, std::vector<ii>, Less> aux = gameManager->getRanking();
	std::vector<bool> alreadyInRanking(4, false);

	int cont = 0;
	bool tie = false;
	ii last = ii(-1e9, -1e9);

	while (!aux.empty())
	{
		ii info = aux.top();
		aux.pop();

		if (info.first > 0 && alreadyInRanking[info.first - 1]) continue;

		if (info.second == last.second)
			tie = tie || cont <= 1;
		else
			cont++;

		gameManager->setPlayerRanking(info.first, cont);
		alreadyInRanking[info.first - 1] = true;
		last = info;
	}

	if (tie)
		gameManager->setWinner(-1);
	else
		gameManager->setWinner(gameManager->getRanking().top().first);

	gameManager->emptyRanking();
}

void Game::chooseWinner()
{
	end = true;
	setRanking();

	GameObject* spawnerObject = findGameObjectWithName("SpawnerManager");
	if (notNull(spawnerObject))
	{
		SpawnerManager* spawnerManager = spawnerObject->getComponent<SpawnerManager>();
		if (notNull(spawnerManager))
			spawnerManager->deactivateAll();
	}

	checkNullAndBreak(gameManager);
	gameManager->setGameEnded(true);
	gameManager->stopMusic(gameManager->getSong());

	endHandleSound();
	if (notNull(cameraEffects))
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

	if (players <= 1 && notNull(gameManager) && !gameManager->isGameEnded())
		chooseWinner();
}

int Game::getPlayer() const
{
	return players;
}

float Game::getTime() const
{
	return gameTimer;
}

void Game::start()
{
	checkNullAndBreak(gameObject);

	gameManager = GameManager::GetInstance();

	checkNullAndBreak(gameManager);

	gameManager->setGameEnded(false);
	soundEmitter = gameObject->getComponent<SoundEmitter>();

	GameObject* mainCamera = findGameObjectWithName("MainCamera");
	if (notNull(mainCamera)) {
		gameLayout = mainCamera->getComponent<UILayout>();
		cameraEffects = mainCamera->getComponent<CameraEffects>();
	}

	if (notNull(gameLayout))
		timePanel = gameLayout->getRoot().getChild("TimeBackground");

	GameObject* countdownObject = findGameObjectWithName("Countdown");
	if (notNull(countdownObject)) countdown = countdownObject->getComponent<Countdown>();

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
	checkNullAndBreak(countdown);

	if (notNull(gameManager))
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
			if (gameTimer <= 0.0f && !gameManager->isGameEnded())
				chooseWinner();

		}
		else if (!countdown->isCounting() && gameTimer < 0)
		{
			gameTimer -= deltaTime;
			gameManager->setTime((int)gameTimer);
			if (!gameManager->isGameEnded() && players <= 1)
				chooseWinner();
		}
	}

	if (notNull(cameraEffects))
	{
		if (!darkness)
		{
			cameraEffects->setDarkness();
			darkness = true;
		}
		else if (countdown->getRemainingTime() < 2.6 && fadeIn)
		{
			cameraEffects->fadeIn();
			fadeIn = false;
		}

		if (end && !cameraEffects->isFading()) {
			SceneManager* sceneManager = SceneManager::GetInstance();
			if (notNull(sceneManager)) sceneManager->changeScene("ScoreMenu");
		}
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