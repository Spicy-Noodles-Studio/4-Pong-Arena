#include "Spawner.h"
#include <GameObject.h>
#include <RigidBody.h>
#include <MathUtils.h>
#include <sstream>
#include <Quaternion.h>

#include "Ball.h"
#include "ParticleManager.h"
#include <ComponentRegister.h>
#include <SoundEmitter.h>

REGISTER_FACTORY(Spawner);

Spawner::Spawner(GameObject* gameObject) : UserComponent(gameObject), velocity(3.0f), angle(25)
{

}

Spawner::~Spawner()
{

}

void Spawner::start()
{
	Vector3 direction = Vector3::ZERO - gameObject->transform->getPosition();
	direction.y = 0;
	gameObject->transform->setDirection(direction.normalized());
	particleManager = gameObject->getComponent<ParticleManager>();
	soundEmitter = gameObject->getComponent<SoundEmitter>();
	soundEmitter->setVolume(0.7);
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
	Vector3 direction = Vector3::ZERO - gameObject->transform->getPosition();

	double rand = random(-angle, angle);
	direction.rotateAroundAxis(Vector3::UP, rand);

	direction.y = 0;

	gameObject->transform->resetOrientation();
	gameObject->transform->setDirection(direction.normalized());

	if (ball != nullptr)
	{
		ball->transform->setPosition(gameObject->transform->getPosition() + direction.normalized());
		ball->getComponent<Ball>()->setVelocity(velocity);
		ball->getComponent<Ball>()->setTargetVelocity(velocity);

		ball->getComponent<RigidBody>()->setLinearVelocity(direction.normalized() * velocity);

		if (particleManager != nullptr)
		{
			particleManager->playParticles(1);
		}

		if (soundEmitter != nullptr) soundEmitter->playSound("Ball_Launch");
	}
}