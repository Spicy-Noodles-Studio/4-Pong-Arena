#include "Death.h"
#include <GameObject.h>
#include <RigidBody.h>
#include <MeshRenderer.h>
#include <sstream>
#include <MathUtils.h>

#include "Health.h"
#include "SpawnerManager.h"
#include "GameManager.h"
#include "Score.h"
#include "PlayerIndex.h"

#include <ComponentRegister.h>
#include <SoundEmitter.h>

REGISTER_FACTORY(Death);

Death::Death(GameObject* gameObject) : UserComponent(gameObject), rigidBody(nullptr), health(nullptr)
{

}

Death::~Death()
{

}

void Death::start()
{
	initialPosition = gameObject->transform->getPosition();

	rigidBody = gameObject->getComponent<RigidBody>();
	health = gameObject->getComponent<Health>();
	meshRenderer = gameObject->getComponent<MeshRenderer>();
	///Para no repetirlos por el codigo y evitar errores.
	manager = GameManager::GetInstance();
	if (manager != nullptr)
		scores = manager->getScore();

	PlayerIndex* playerId= this->gameObject->getComponent<PlayerIndex>();
	soundEmitter = gameObject->getComponent<SoundEmitter>();
	soundEmitter->setVolume(1.2);

	id = -1;
	if (playerId != nullptr)
	{
		id = playerId->getId();
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
	gameObject->transform->setPosition(initialPosition);
	rigidBody->setStatic(true);

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

		Vector3 scaleRatio = wallScale / gameObject->transform->getScale();
		gameObject->transform->setScale(wallScale);
		rigidBody->multiplyScale(scaleRatio);
	}

	if (id != -1 && scores != nullptr)
	{
		scores->setPositionOnLeaderBoard(id, manager->getPlayersAlive());
		scores->setTimeAlive(id, manager->getInitialTime(), manager->getTime());
	}
	manager->setPlayersAlive(manager->getPlayersAlive() - 1);

	soundEmitter->playSound("Death");

	// increase spawner generation frequency
	SpawnerManager* spawnerManager = findGameObjectWithName("SpawnerManager")->getComponent<SpawnerManager>();

	float genTime = spawnerManager->getGenerationTime();
	float minTime = spawnerManager->getMinimumTime();

	if (genTime / 2 < minTime)
		spawnerManager->setGenerationTime(minTime);
	else
		spawnerManager->setGenerationTime(genTime / 2);
}