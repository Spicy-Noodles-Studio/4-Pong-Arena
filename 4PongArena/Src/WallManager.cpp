#include "WallManager.h"
#include <ComponentRegister.h>
#include <GameObject.h>
#include <RigidBody.h>
#include <MeshRenderer.h>
#include <sstream>

#include "Movement.h"
#include "Health.h"
#include "Score.h"

REGISTER_FACTORY(WallManager);

WallManager::WallManager(GameObject* gameObject) : UserComponent(gameObject), rigidBody(nullptr), health(nullptr), goal(nullptr), goalOffset(0.0f)
{

}

WallManager::~WallManager()
{

}

void WallManager::start()
{
	initialPosition = gameObject->transform->getPosition();

	goal = instantiate("Goal", initialPosition);
	goal->transform->setRotation(gameObject->transform->getRotation());

	Vector3 normal = gameObject->getComponent<Movement>()->getNormal();
	goal->transform->setPosition(initialPosition - normal * goalOffset);
	goal->getComponent<Score>()->setKeeper(gameObject);

	rigidBody = gameObject->getComponent<RigidBody>();
	health = gameObject->getComponent<Health>();

	aliveMeshId = gameObject->getComponent<MeshRenderer>()->getMeshId();
	aliveMeshName = gameObject->getComponent<MeshRenderer>()->getMeshName();

	aliveScale = gameObject->transform->getScale();
}

void WallManager::update(float deltaTime)
{
	if (health != nullptr && health->isDead()) {
		changeShapeToWall();
		setActive(false);
	}
}

void WallManager::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "sensorOffset")
		{
			if (!(ss >> goalOffset))
				LOG("WALL MANAGER: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else if (prop.first == "wallMesh")
		{
			if (!(ss >> wallMeshId >> wallMeshName))
				LOG("WALL MANAGER: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else if (prop.first == "wallScale")
		{
			double x, y, z;
			if (!(ss >> x >> y >> z))
				LOG("WALL MANAGER: Invalid property with name \"%s\"", prop.first.c_str());
			else
				wallScale = { x,y,z };
		}
		else
			LOG("WALL MANAGER: Invalid property name \"%s\"", prop.first.c_str());
	}
}

void WallManager::changeShapeToWall()
{
	gameObject->transform->setPosition(initialPosition);
	rigidBody->setStatic(true);

	if (wallMeshId != "" || wallMeshName != "") {
		gameObject->getComponent<MeshRenderer>()->changeMesh(wallMeshId, wallMeshName);
		Vector3 scaleRatio = wallScale / gameObject->transform->getScale();
		gameObject->transform->setScale(wallScale);
		rigidBody->multiplyScale(scaleRatio);
	}
}
