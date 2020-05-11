#include "Death.h"
#include <GameObject.h>
#include <MeshRenderer.h>
#include <RigidBody.h>
#include <sstream>

#include "PlayerIndex.h"
#include "Health.h"
#include "Score.h"
#include "SpawnerManager.h"
#include "GameManager.h"

#include <ComponentRegister.h>

REGISTER_FACTORY(Death);

Death::Death(GameObject* gameObject) : UserComponent(gameObject), gameManager(nullptr), meshRenderer(nullptr), rigidBody(nullptr), health(nullptr), scores(nullptr), id(-1)
{

}

Death::~Death()
{

}

void Death::start()
{
	initialPosition = gameObject->transform->getPosition();

	meshRenderer = gameObject->getComponent<MeshRenderer>();
	rigidBody = gameObject->getComponent<RigidBody>();
	health = gameObject->getComponent<Health>();

	gameManager = GameManager::GetInstance();

	if (gameManager != nullptr)
		scores = gameManager->getScore();

	PlayerIndex* playerId = gameObject->getComponent<PlayerIndex>();

	if (playerId != nullptr)
		id = playerId->getId();
}

void Death::update(float deltaTime)
{
	if (health != nullptr && !health->isAlive())
	{
		die();
		setActive(false);
	}
}

void Death::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "wallMesh")
		{
			if (!(ss >> wallMeshId >> wallMeshName))
				LOG("DEATH: Invalid value for property with name \"%s\"", prop.first.c_str());
		}
		else if (prop.first == "wallScale")
		{
			if (!(ss >> wallScale.x >> wallScale.y >> wallScale.z))
				LOG("DEATH: Invalid value for property with name \"%s\"", prop.first.c_str());
		}
		else
			LOG("DEATH: Invalid property name \"%s\"", prop.first.c_str());
	}
}

void Death::setPlayerColour(Vector3 colour)
{
	playerColour = colour;
}

void Death::die()
{
	gameObject->transform->setPosition(initialPosition);
	rigidBody->setStatic(true);

	if (wallMeshId != "" || wallMeshName != "")
	{
		if (meshRenderer != nullptr)
		{
			meshRenderer->changeMesh(wallMeshId, wallMeshName);
			meshRenderer->setDiffuse(0, playerColour, 1);
		}

		Vector3 scaleRatio = wallScale / gameObject->transform->getScale();
		gameObject->transform->setScale(wallScale);
		rigidBody->multiplyScale(scaleRatio);
	}

	if (id != -1 && scores != nullptr)
		scores->setTimeAlive(id, gameManager->getInitialTime(), gameManager->getTime());

	gameManager->setPlayerRanking(id, gameManager->getPlayersAlive());
	gameManager->setPlayersAlive(gameManager->getPlayersAlive() - 1);

	SpawnerManager* spawnerManager = findGameObjectWithName("SpawnerManager")->getComponent<SpawnerManager>();

	if (spawnerManager->getGenerationTime() / 2 < spawnerManager->getMinimumTime())
		spawnerManager->setGenerationTime(spawnerManager->getMinimumTime());
	else
		spawnerManager->setGenerationTime(spawnerManager->getGenerationTime() / 2);
}