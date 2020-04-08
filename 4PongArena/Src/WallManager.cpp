#include "WallManager.h"
#include <sstream>
#include "MeshRenderer.h"
#include <math.h>
#include <MathUtils.h>
#include <GameObject.h>
#include "Health.h"
#include <ComponentRegister.h>
#include <RigidBody.h>


REGISTER_FACTORY(WallManager);


WallManager::WallManager(GameObject* gameObject) : UserComponent(gameObject), rigidBody(nullptr), health(nullptr)
{
}

WallManager::~WallManager()
{
}

void WallManager::start()
{
	rigidBody = gameObject->getComponent<RigidBody>();

	aliveMeshId = gameObject->getComponent<MeshRenderer>()->getMeshId();
	aliveMeshName = gameObject->getComponent<MeshRenderer>()->getMeshName();
}

void WallManager::update(float deltaTime)
{
	/*if (health == nullptr)
	{
		Vector3 pos = gameObject->transform->getPosition() + Vector3(sin(gameObject->transform->getRotation().y * DEG_TO_RAD), 0, cos(gameObject->transform->getRotation().y * DEG_TO_RAD));

		GameObject* sensor = instantiate("Sensor", pos);

		health = sensor->getComponent<Health>();
		if (cos(gameObject->transform->getRotation().y * DEG_TO_RAD) == 1 || cos(gameObject->transform->getRotation().y * DEG_TO_RAD) == -1) {
			if (health != nullptr)
				sensor->getComponent<RigidBody>()->multiplyScale(Vector3(5, 1, 1));
		}
		else {
			if (health != nullptr)
				sensor->getComponent<RigidBody>()->multiplyScale(Vector3(1, 1, 5));
		}

		initialPosition = gameObject->transform->getPosition();

	}

	if (!health->isAlive() && /wall)
	/*{
		wall = true;
		changeShapeToWall();
		rigidBody->setLinearVelocity(Vector3(0, 0, 0));
		gameObject->transform->setPosition(initialPosition);
	}
	else if (wall)
	{
		rigidBody->setRotationConstraints(Vector3(0, 0, 0));
		rigidBody->setMovementConstraints(Vector3(0, 0, 0));
		rigidBody->setStatic(true);
	}*/
}

void WallManager::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties()) {
		std::stringstream ss(prop.second);

		if (prop.first == "wallMesh") {
			if (!(ss >> wallMeshId >> wallMeshName))
				LOG("HEALTH: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else if (prop.first == "wallScale") {
			double x, y, z;
			if (!(ss >> x >> y >> z))
				LOG("HEALTH: Invalid property with name \"%s\"", prop.first.c_str());
			//else
				//wallScale = { x,y,z };
		}
		else
			LOG("GHOST MANAGER: Invalid property name \"%s\"", prop.first.c_str());
	}
}

void WallManager::changeShapeToWall()
{
	/*if (wallMeshId != "" || wallMeshName != "")
		gameObject->getComponent<MeshRenderer>()->changeMesh(wallMeshId, wallMeshName);

	Vector3 scale = gameObject->transform->getScale();
	if (sin(gameObject->transform->getRotation().y) != 0) scale.x = 5;
	else scale.z = 5;

	if (health != nullptr)
	{
		if (wallScale == Vector3(1, 1, 1))
		{
			double size = 5 / gameObject->transform->getScale().x;
			gameObject->transform->setScale(scale);
			rigidBody->multiplyScale(Vector3(size, 1, 1));
		}
		else
		{
			gameObject->transform->setScale(wallScale);
			rigidBody->multiplyScale(wallScale / aliveScale);
		}
	}*/
}
