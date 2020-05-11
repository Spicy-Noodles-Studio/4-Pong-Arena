#include "Ball.h"
#include <GameObject.h>
#include <RigidBody.h>
#include <sstream>

#include <ComponentRegister.h>
#include <SoundEmitter.h>

REGISTER_FACTORY(Ball);

Ball::Ball(GameObject* gameObject): UserComponent(gameObject), rigidBody(nullptr), velocity(0.0f), targetVelocity(0.0f), acceleration(0.0f),idPlayer(-1)
{

}

Ball::~Ball()
{

}

void Ball::start()
{
	rigidBody = gameObject->getComponent<RigidBody>();
	soundEmitter = gameObject->getComponent<SoundEmitter>();
	soundEmitter->setVolume(0.8);
}

void Ball::update(float deltaTime)
{
	if (velocity < targetVelocity)
		velocity += acceleration * deltaTime;
	else
		velocity = targetVelocity;

	if (rigidBody != nullptr)
		rigidBody->setLinearVelocity(rigidBody->getLinearVelocity().normalized() * velocity);
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

int Ball::getIdPlayerHit()
{
	return idPlayer;
}

void Ball::onCollisionEnter(GameObject* other)
{
	if (soundEmitter == nullptr) return;

	std::string soundToPlay = "NO SOUND";
	if (other->getTag() == "wall" || other->getTag() == "spawner") {
		soundToPlay = "Wall_Bounce";
	}
	else if (other->getTag() == "ball") {
		soundToPlay = "Ball_bounce_hard";
	}
	else if (other->getTag() == "paddle" || other->getTag() == "paddleIA") {
		soundToPlay = "Ball_bounce";
	}

	if(soundToPlay != "NO SOUND") soundEmitter->playSound(soundToPlay);
}
