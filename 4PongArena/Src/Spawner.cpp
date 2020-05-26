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

Spawner::Spawner(GameObject* gameObject) : UserComponent(gameObject), particleManager(nullptr), soundEmitter(nullptr), velocity(3.0f), angle(25)
{

}

Spawner::~Spawner()
{
	soundEmitter = nullptr;
	particleManager = nullptr;
}

void Spawner::start()
{
	checkNullAndBreak(gameObject);

	Vector3 direction = Vector3::ZERO - gameObject->transform->getPosition();
	direction.y = 0;

	particleManager = gameObject->getComponent<ParticleManager>();
	soundEmitter = gameObject->getComponent<SoundEmitter>();
	if (notNull(soundEmitter)) soundEmitter->setVolume(0.7);
	if (notNull(gameObject->transform)) gameObject->transform->setDirection(direction.normalized());
}

void Spawner::handleData(ComponentData* data)
{
	if (data == nullptr) return;

	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "velocity")
		{
			setFloat(velocity);
		}
		else if (prop.first == "angle")
		{
			setFloat(angle);
		}
		else
			LOG("SPAWNER: Invalid property name \"%s\"", prop.first.c_str());
	}
}

void Spawner::shoot(GameObject* ballObject)
{
	checkNullAndBreak(gameObject);
	checkNullAndBreak(gameObject->transform);
	checkNullAndBreak(ballObject);
	checkNullAndBreak(ballObject->transform);

	Vector3 direction = Vector3::ZERO - gameObject->transform->getPosition();

	double rand = random(-angle, angle);
	direction.rotateAroundAxis(Vector3::UP, rand);
	direction.y = 0;

	gameObject->transform->resetOrientation();
	gameObject->transform->setDirection(direction.normalized());
	ballObject->transform->setPosition(gameObject->transform->getPosition() + direction.normalized());

	Ball* ball = ballObject->getComponent<Ball>();
	Trail* trail = ballObject->getComponent<Trail>();
	RigidBody* rigidBody = ballObject->getComponent<RigidBody>();

	if (notNull(ball)) {
		ball->setVelocity(velocity);
		ball->setTargetVelocity(velocity);
	}

	if (notNull(rigidBody))
		rigidBody->setLinearVelocity(direction.normalized() * velocity);

	if (notNull(trail)) {
		trail->start();
		trail->setColour({ 1.0,1.0,1.0 }, 0.7);
	}

	if (notNull(particleManager)) particleManager->playParticles(1);
	if (notNull(soundEmitter)) soundEmitter->playSound("Ball_Launch");

}