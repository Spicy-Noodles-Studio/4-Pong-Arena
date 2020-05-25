#include "Spawner.h"
#include <GameObject.h>
#include <RigidBody.h>
#include <MathUtils.h>
#include <sstream>
#include <Quaternion.h>
#include <Trail.h>

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
	if (gameObject != nullptr) {
		Vector3 direction = Vector3::ZERO - gameObject->transform->getPosition();
		direction.y = 0;
		if (gameObject->transform != nullptr) gameObject->transform->setDirection(direction.normalized());
		particleManager = gameObject->getComponent<ParticleManager>();
		soundEmitter = gameObject->getComponent<SoundEmitter>();
		if (soundEmitter != nullptr) soundEmitter->setVolume(0.7);
	}
}

void Spawner::handleData(ComponentData* data)
{
	if (data == nullptr) return;

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
	if (gameObject == nullptr || gameObject->transform == nullptr) return;
	if (ball == nullptr || ball->transform == nullptr) return;
	
	Vector3 direction = Vector3::ZERO - gameObject->transform->getPosition();

	double rand = random(-angle, angle);
	direction.rotateAroundAxis(Vector3::UP, rand);

	direction.y = 0;

	gameObject->transform->resetOrientation();
	gameObject->transform->setDirection(direction.normalized());
	
	ball->transform->setPosition(gameObject->transform->getPosition() + direction.normalized());

	if (ball->getComponent<Ball>() != nullptr) {
		ball->getComponent<Ball>()->setVelocity(velocity);
		ball->getComponent<Ball>()->setTargetVelocity(velocity);
	}

	if (ball->getComponent<RigidBody>() != nullptr)
		ball->getComponent<RigidBody>()->setLinearVelocity(direction.normalized() * velocity);
	
	if (ball->getComponent<Trail>() != nullptr) {
		ball->getComponent<Trail>()->start();
		ball->getComponent<Trail>()->setColour({ 1.0,1.0,1.0 }, 0.7);
	}

	if (particleManager != nullptr)
	{
		particleManager->playParticles(1);
	}

	if (soundEmitter != nullptr) soundEmitter->playSound("Ball_Launch");
	
}