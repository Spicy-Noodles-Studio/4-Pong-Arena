#include "Death.h"
#include <ComponentRegister.h>
#include <GameObject.h>
#include <RigidBody.h>
#include <MeshRenderer.h>
#include <sstream>

#include "Movement.h"
#include "Health.h"
#include "Score.h"

REGISTER_FACTORY(Death);

Death::Death(GameObject* gameObject) : UserComponent(gameObject), rigidBody(nullptr), health(nullptr), goal(nullptr), goalOffset(0.0f)
{

}

Death::~Death()
{

}

void Death::start()
{
	initialPosition = gameObject->transform->getPosition();

	goal = instantiate("Goal", initialPosition); // TODO: HACER EN UN COMPONENTE KEEPER (Portero)
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

void Death::update(float deltaTime)
{
	if (health != nullptr && health->isDead()) {
		die();
		setActive(false);
	}
}

void Death::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties())	{
		std::stringstream ss(prop.second);
		if (prop.first == "sensorOffset") {
			if (!(ss >> goalOffset))
				LOG("WALL MANAGER: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else if (prop.first == "wallMesh") {
			if (!(ss >> wallMeshId >> wallMeshName))
				LOG("WALL MANAGER: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else if (prop.first == "wallScale")	{
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

void Death::die()
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
