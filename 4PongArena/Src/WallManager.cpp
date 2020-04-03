#include "WallManager.h"
#include <sstream>
#include "MeshRenderer.h"
#include <math.h>
#include <MathUtils.h>
#include <GameObject.h>
#include "Health.h"
#include <RigidBody.h>

WallManager::WallManager(GameObject* gameObject) : UserComponent(gameObject)
{
}

void WallManager::start()
{

	rigidBody = gameObject->getComponent<RigidBody>();
	
	aliveMeshId = gameObject->getComponent<MeshRenderer>()->getMeshId();
	aliveMeshName = gameObject->getComponent<MeshRenderer>()->getMeshName();
	aliveScale = gameObject->transform->getScale();
}

void WallManager::update(float deltaTime)
{
	if (health == nullptr)
	{
		Vector3 pos = Vector3(0, 0, 1);
		//double x = cos(gameObject->transform->getRotation().y * DEG_TO_RAD);
		pos = gameObject->transform->getPosition() + Vector3(sin(gameObject->transform->getRotation().y* DEG_TO_RAD), 0, cos(gameObject->transform->getRotation().y* DEG_TO_RAD));
		

		GameObject* sensor = instantiate("Sensor", pos);

		health = sensor->getComponent<Health>();
		if (cos(gameObject->transform->getRotation().y * DEG_TO_RAD) ==1 || cos(gameObject->transform->getRotation().y * DEG_TO_RAD) == -1) {
			if (health != nullptr)
				sensor->getComponent<RigidBody>()->multiplyScale(Vector3(health->getTriggerSize(), 1, 1));
		}
		else {
			if (health != nullptr)
				sensor->getComponent<RigidBody>()->multiplyScale(Vector3(1, 1, health->getTriggerSize()));
		}

		OriginalPosition = gameObject->transform->getPosition();

	}
	UserComponent::update(deltaTime);

	if (!health->isAlive() && !wall)
	{
		wall = true;
		changeShapeToWall();
		rigidBody->setLinearVelocity(Vector3(0, 0, 0));
		gameObject->transform->setPosition(OriginalPosition);
	}
	else if (wall)
	{
		rigidBody->setRotationConstraints(Vector3(0, 0, 0));
		rigidBody->setMovementConstraints(Vector3(0, 0, 0));
		rigidBody->setStatic(true);
		
		//rigidBody->clearForces();
	}
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
			else
				wallScale = { x,y,z };
		}
		else
			LOG("GHOST MANAGER: Invalid property name \"%s\"", prop.first.c_str());
	}
}

void WallManager::changeShapeToWall()
{
	if(wallMeshId!="" || wallMeshName!="")
	gameObject->getComponent<MeshRenderer>()->changeMesh(wallMeshId, wallMeshName);
	
	if (sin(gameObject->transform->getRotation().y) != 0)
	{
		if (health != nullptr)
		{
			if (wallScale == Vector3(1, 1, 1))
			{
				double  size = health->getTriggerSize() / gameObject->transform->getScale().x;
				gameObject->transform->setScale(Vector3(health->getTriggerSize(), gameObject->transform->getScale().y, gameObject->transform->getScale().z));
				rigidBody->multiplyScale(Vector3(size, 1, 1));
			}
			else
			{
				
				gameObject->transform->setScale(wallScale);
				rigidBody->multiplyScale(wallScale/aliveScale);
			}
		}
	}
	else
	{
		if (health != nullptr)
		{
			if (wallScale == Vector3(1, 1, 1))
			{
				double  size = health->getTriggerSize() / gameObject->transform->getScale().x;
				rigidBody->multiplyScale(Vector3(1, 1, size));
				gameObject->transform->setScale(Vector3(gameObject->transform->getScale().x, gameObject->transform->getScale().y, health->getTriggerSize()));
			}
			else
			{
				gameObject->transform->setScale(wallScale);
				rigidBody->multiplyScale(wallScale / aliveScale);
			}
		}
	}
}

bool WallManager::IsWall()
{
	return wall;
}

Health* WallManager::GetHealth()
{
	return health;
}
