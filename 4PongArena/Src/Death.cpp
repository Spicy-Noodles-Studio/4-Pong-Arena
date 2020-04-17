#include "Death.h"
#include <GameObject.h>
#include <RigidBody.h>
#include <MeshRenderer.h>
#include <sstream>

#include "Health.h"
#include "BallGenerator.h"
#include "GameManager.h"

#include <ComponentRegister.h>

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

void Death::die()
{
	gameObject->transform->setPosition(initialPosition);
	rigidBody->setStatic(true);

	if (wallMeshId != "" || wallMeshName != "")
	{
		gameObject->getComponent<MeshRenderer>()->changeMesh(wallMeshId, wallMeshName);

		Vector3 scaleRatio = wallScale / gameObject->transform->getScale();
		gameObject->transform->setScale(wallScale);
		rigidBody->multiplyScale(scaleRatio);
	}

	GameManager* gameManger = GameManager::GetInstance();

	gameManger->setPlayersAlive(gameManger->getPlayersAlive() - 1);

	for (int i = 0; i < gameManger->getGenerators().size(); i++)
	{
		BallGenerator* aux = gameManger->getGenerators()[i]->getComponent<BallGenerator>();

		if (aux->getGenerationTime() / 2 > aux->getMinimumTime())
			aux->setGenerationTime(aux->getGenerationTime() / 2);
		else
			aux->setGenerationTime(aux->getMinimumTime());
	}
}