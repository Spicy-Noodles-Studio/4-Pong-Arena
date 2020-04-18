#include "Spawner.h"
#include <GameObject.h>
#include <RigidBody.h>
#include <MathUtils.h>
#include <sstream>

#include <ComponentRegister.h>

REGISTER_FACTORY(Spawner);

Spawner::Spawner(GameObject* gameObject) : UserComponent(gameObject), velocity(3.0f), angle(25)
{

}

Spawner::~Spawner()
{

}

void Spawner::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "velocity")
		{
			if (!(ss >> velocity))
				LOG("SPAWNER: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else if (prop.first == "angle")
		{
			if (!(ss >> angle))
				LOG("SPAWNER: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else
			LOG("SPAWNER: Invalid property name \"%s\"", prop.first.c_str());
	}
}

void Spawner::shoot(GameObject* ball)
{
	Vector3 aux = Vector3() - gameObject->transform->getPosition();

	double actualAngle = RAD_TO_DEG * atan2(aux.x, aux.z);

	if (actualAngle == -45 || actualAngle == 135) // No se por que hay que hacer esto, ¿cambiarlo?
		actualAngle += 180;

	double finalAngle = (actualAngle + random(-angle, angle)) * DEG_TO_RAD;

	Vector3 direction = Vector3(cos(finalAngle), 0, sin(finalAngle));

	if (ball != nullptr)
	{
		ball->transform->setPosition(gameObject->transform->getPosition());
		ball->getComponent<RigidBody>()->setLinearVelocity(direction * velocity);
	}
}