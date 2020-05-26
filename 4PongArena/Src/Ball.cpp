#include "Ball.h"
#include <GameObject.h>
#include <RigidBody.h>
#include <sstream>
#include <ComponentRegister.h>
#include <SoundEmitter.h>
#include "ParticleManager.h"
#include "GameManager.h"

REGISTER_FACTORY(Ball);

Ball::Ball(GameObject* gameObject) : UserComponent(gameObject), rigidBody(nullptr), velocity(0.0f), targetVelocity(0.0f), particleManager(nullptr), soundEmitter(nullptr), acceleration(0.0f), idPlayer(-1), volume(0.0f)
{

}

Ball::~Ball()
{
	rigidBody = nullptr;
	soundEmitter = nullptr;
	particleManager = nullptr;
}

void Ball::start()
{
	checkNullAndBreak(gameObject);

	rigidBody = gameObject->getComponent<RigidBody>();
	soundEmitter = gameObject->getComponent<SoundEmitter>();
	particleManager = gameObject->getComponent<ParticleManager>();
	
	volume = 0.8;
	if (notNull(soundEmitter)) soundEmitter->setVolume(volume);

}

void Ball::update(float deltaTime)
{
	if (velocity < targetVelocity)
		velocity += acceleration * deltaTime;
	else
		velocity = targetVelocity;

	if (notNull(rigidBody))
		rigidBody->setLinearVelocity(rigidBody->getLinearVelocity().normalized() * velocity);

	GameManager* gameManager = GameManager::GetInstance();
	if (volume > 0 && notNull(gameManager) && gameManager->isGameEnded()) {
		volume = 0;
		if (notNull(soundEmitter)) soundEmitter->setVolume(0);
	}
}

void Ball::setVelocity(float velocity)
{
	this->velocity = velocity;
}

void Ball::setTargetVelocity(float targetVelocity)
{
	this->targetVelocity = targetVelocity;
}

void Ball::setAcceleration(float acceleration)
{
	this->acceleration = acceleration;
}

void Ball::setIdPlayerHit(int id)
{
	idPlayer = id;
}

int Ball::getIdPlayerHit() const
{
	return idPlayer;
}

void Ball::onCollisionEnter(GameObject* other)
{
	checkNullAndBreak(other);

	std::string soundToPlay = "NO SOUND";
	if (other->getTag() == "wall" || other->getTag() == "spawner")
		soundToPlay = "Wall_Bounce";
	else if (other->getTag() == "ball")
		soundToPlay = "Ball_bounce_hard";
	else if (other->getTag() == "paddle" || other->getTag() == "paddleIA")
		soundToPlay = "Ball_bounce";
	else if (other->getTag() == "obstacle")
		soundToPlay = "Ball_obstacle";

	if (soundToPlay != "NO SOUND" && notNull(soundEmitter)) soundEmitter->playSound(soundToPlay);

	if (notNull(particleManager)) particleManager->playParticles(0.3);
}
