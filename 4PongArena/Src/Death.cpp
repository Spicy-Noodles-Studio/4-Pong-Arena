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

Death::Death(GameObject* gameObject) : UserComponent(gameObject), soundEmitter(nullptr), gameManager(nullptr), meshRenderer(nullptr), rigidBody(nullptr), health(nullptr), scores(nullptr), id(-1)
{

}

Death::~Death()
{
	gameManager = nullptr;
	meshRenderer = nullptr;
	rigidBody = nullptr;
	health = nullptr;
	scores = nullptr;
	soundEmitter = nullptr;
}

void Death::start()
{
	checkNullAndBreak(gameObject);
	if (notNull(gameObject->transform)) initialPosition = gameObject->transform->getPosition();

	meshRenderer = gameObject->getComponent<MeshRenderer>();
	rigidBody = gameObject->getComponent<RigidBody>();
	health = gameObject->getComponent<Health>();

	gameManager = GameManager::GetInstance();

	if (notNull(gameManager))
		scores = gameManager->getScore();

	PlayerIndex* playerId = gameObject->getComponent<PlayerIndex>();
	if (notNull(playerId))
		id = playerId->getPosVector();

	soundEmitter = gameObject->getComponent<SoundEmitter>();
	if (notNull(soundEmitter))
		soundEmitter->setVolume(1.2);
}

void Death::update(float deltaTime)
{
	if (notNull(health) && !health->isAlive())
	{
		die();
		setActive(false);
	}
}

void Death::handleData(ComponentData* data)
{
	checkNullAndBreak(data);

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
	this->baseColour = baseColour;
	this->neonColour = neonColour;
}

void Death::setWallScale(const Vector3& wallScale)
{
	this->wallScale = wallScale;
}

void Death::die()
{
	if (notNull(gameObject) && notNull(gameObject->transform)) gameObject->transform->setPosition(initialPosition);
	if (notNull(rigidBody)) rigidBody->setStatic(true);

	if (wallMeshId != "" || wallMeshName != "")
	{
		if (notNull(meshRenderer)) {
			meshRenderer->changeMesh(wallMeshId, wallMeshName);
			meshRenderer->setDiffuse(0, playerColour, 1);

			meshRenderer->setDiffuse(2, neonColour.first, 1);
			meshRenderer->setEmissive(2, neonColour.second);

			meshRenderer->setDiffuse(1, baseColour.first, 1);
			meshRenderer->setEmissive(1, baseColour.second);
		}

		if (notNull(gameObject) && notNull(gameObject->transform)) {
			Vector3 scaleRatio = wallScale / gameObject->transform->getScale();
			gameObject->transform->setScale(wallScale);
			if (rigidBody != nullptr) rigidBody->multiplyScale(scaleRatio);
		}
	}

	if (id != -1 && notNull(scores) && notNull(gameManager))
		scores->setTimeAlive(id, gameManager->getInitialTime(), gameManager->getTime());


	GameObject* object = findGameObjectWithName("Game");
	if (notNull(object) && notNull(gameObject))
	{
		Game* game = object->getComponent<Game>();
		PlayerIndex* playerIndex = gameObject->getComponent<PlayerIndex>();

		if (notNull(playerIndex) && notNull(game) && notNull(gameManager))
		{
			gameManager->getRanking().push(ii(playerIndex->getId(), 0 - game->getPlayer()));
			game->playerDeath();
		}
	}

	if (notNull(soundEmitter))
		soundEmitter->playSound("Death");

	SpawnerManager* spawnerManager = nullptr;
	GameObject* spawner = findGameObjectWithName("SpawnerManager");
	if (notNull(spawner))
		spawnerManager = spawner->getComponent<SpawnerManager>();

	if (notNull(spawnerManager))
	{
		float genTime = spawnerManager->getGenerationTime();
		float minTime = spawnerManager->getMinimumTime();

		if (genTime / 2 < minTime)
			spawnerManager->setGenerationTime(minTime);
		else
			spawnerManager->setGenerationTime(genTime / 2);
	}
}