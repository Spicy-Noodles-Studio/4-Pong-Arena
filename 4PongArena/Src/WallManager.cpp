#include "WallManager.h"
#include <GameObject.h>
#include <RigidBody.h>
#include <MeshRenderer.h>
#include <sstream>

#include "Health.h"
#include "PlayerController.h"
#include "ComponentRegister.h"

REGISTER_FACTORY(WallManager);

WallManager::WallManager(GameObject* gameObject) : UserComponent(gameObject)
{

}

WallManager::~WallManager()
{

}

void WallManager::start()
{
	initialPosition = gameObject->transform->getPosition();

	sensor = instantiate("Sensor", initialPosition);
	sensor->transform->setRotation(gameObject->transform->getRotation());

	int id = gameObject->getComponent<PlayerController>()->getPlayerId();

	if (id == 1)
		sensor->transform->setPosition(initialPosition + Vector3(0, 0, sensorOffset));
	else if (id == 2)
		sensor->transform->setPosition(initialPosition + Vector3(sensorOffset, 0, 0));
	else if (id == 3)
		sensor->transform->setPosition(initialPosition + Vector3(0, 0, -sensorOffset));
	else if (id == 4)
		sensor->transform->setPosition(initialPosition + Vector3(-sensorOffset, 0, 0));

	rigidBody = gameObject->getComponent<RigidBody>();
	health = gameObject->getComponent<Health>();

	aliveMeshId = gameObject->getComponent<MeshRenderer>()->getMeshId();
	aliveMeshName = gameObject->getComponent<MeshRenderer>()->getMeshName();

	aliveScale = gameObject->transform->getScale();

	wall = false;
}

void WallManager::update(float deltaTime)
{
	if (health != nullptr && !health->isAlive() && !wall)
		changeShapeToWall();
}

void WallManager::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "sensorOffset")
		{
			if (!(ss >> sensorOffset))
				LOG("HEALTH: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else if (prop.first == "wallMesh")
		{
			if (!(ss >> wallMeshId >> wallMeshName))
				LOG("HEALTH: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else if (prop.first == "wallScale")
		{
			double x, y, z;
			if (!(ss >> x >> y >> z))
				LOG("HEALTH: Invalid property with name \"%s\"", prop.first.c_str());
			else
				wallScale = { x,y,z };
		}
		else
			LOG("GHOST MANAGER: Invalid property name \"%s\"", prop.first.c_str());
	}
}

void WallManager::changeShapeToWall()
{
	wall = true;

	rigidBody->setMovementConstraints(Vector3(0, 0, 0));
	rigidBody->setRotationConstraints(Vector3(0, 0, 0));
	rigidBody->setStatic(true);

	gameObject->transform->setPosition(initialPosition);

	if (wallMeshId != "" || wallMeshName != "")
	{
		gameObject->getComponent<MeshRenderer>()->changeMesh(wallMeshId, wallMeshName);
		rigidBody->multiplyScale(wallScale);
	}
}