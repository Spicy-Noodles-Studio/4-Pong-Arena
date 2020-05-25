#include "Death.h"
#include <GameObject.h>
#include <MeshRenderer.h>
#include <RigidBody.h>
#include <sstream>
#include <MathUtils.h>

#include "PlayerIndex.h"
#include "Health.h"
#include "Score.h"
#include "SpawnerManager.h"
#include "GameManager.h"
#include "Game.h"

#include <ComponentRegister.h>
#include <SoundEmitter.h>

REGISTER_FACTORY(Death);

Death::Death(GameObject* gameObject) : UserComponent(gameObject), gameManager(nullptr), meshRenderer(nullptr), rigidBody(nullptr), health(nullptr), scores(nullptr), id(-1)
{

}

Death::~Death()
{

}

void Death::start()
{
	if (gameObject != nullptr) {
		if (gameObject->transform != nullptr) initialPosition = gameObject->transform->getPosition();

		meshRenderer = gameObject->getComponent<MeshRenderer>();
		rigidBody = gameObject->getComponent<RigidBody>();
		health = gameObject->getComponent<Health>();

		gameManager = GameManager::GetInstance();

		if (gameManager != nullptr)
			scores = gameManager->getScore();

		PlayerIndex* playerId = gameObject->getComponent<PlayerIndex>();

		if (playerId != nullptr)
			id = playerId->getPosVector();

		soundEmitter = gameObject->getComponent<SoundEmitter>();

		if (soundEmitter != nullptr)
			soundEmitter->setVolume(1.2);
	}
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
	if (data == nullptr) return;

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

void Death::setPlayerColour(const Vector3& colour)
{
	playerColour = colour;
}

void Death::setwallColours(const std::pair<Vector3, Vector3>& baseColour, const std::pair<Vector3, Vector3>& neonColour)
{
	if (this == nullptr) return;

	this->baseColour = baseColour;
	this->neonColour = neonColour;
}

void Death::setWallScale(const Vector3& wallScale)
{
	if (this == nullptr) return;

	this->wallScale = wallScale;
}

void Death::die()
{
	if (gameObject != nullptr && gameObject->transform != nullptr) gameObject->transform->setPosition(initialPosition);
	if (rigidBody != nullptr) rigidBody->setStatic(true);

	if (wallMeshId != "" || wallMeshName != "")
	{
		if (meshRenderer != nullptr) {
			meshRenderer->changeMesh(wallMeshId, wallMeshName);
			meshRenderer->setDiffuse(0, playerColour, 1);

			meshRenderer->setDiffuse(2, neonColour.first, 1);
			meshRenderer->setEmissive(2, neonColour.second);

			meshRenderer->setDiffuse(1, baseColour.first, 1);
			meshRenderer->setEmissive(1, baseColour.second);
		}

		if (gameObject != nullptr && gameObject->transform != nullptr) {
			Vector3 scaleRatio = wallScale / gameObject->transform->getScale();
			gameObject->transform->setScale(wallScale);
			if (rigidBody != nullptr) rigidBody->multiplyScale(scaleRatio);
		}
	}

	if (id != -1 && scores != nullptr)
		scores->setTimeAlive(id, gameManager->getInitialTime(), gameManager->getTime());

	Game* game = nullptr;
	if (findGameObjectWithName("Game") != nullptr) game = findGameObjectWithName("Game")->getComponent<Game>();
	if (game != nullptr)
		game->playerDeath();

	if (soundEmitter != nullptr)soundEmitter->playSound("Death");

	SpawnerManager* spawnerManager = nullptr;
	if (findGameObjectWithName("SpawnerManager") != nullptr) spawnerManager = findGameObjectWithName("SpawnerManager")->getComponent<SpawnerManager>();

	if (spawnerManager != nullptr) {

		float genTime = spawnerManager->getGenerationTime();
		float minTime = spawnerManager->getMinimumTime();

		if (genTime / 2 < minTime)
			spawnerManager->setGenerationTime(minTime);
		else
			spawnerManager->setGenerationTime(genTime / 2);
	}
}