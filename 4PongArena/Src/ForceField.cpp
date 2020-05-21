#include "ForceField.h"
#include <GameObject.h>
#include <RigidBody.h>
#include <sstream>
#include<Trail.h>

#include "Ball.h"
#include "GameManager.h"

#include <ComponentRegister.h>
#include <SoundEmitter.h>

REGISTER_FACTORY(ForceField);

ForceField::ForceField(GameObject* gameObject) : UserComponent(gameObject), targetVelocity(0.0f), acceleration(0.0f)
{

}

ForceField::~ForceField()
{

}

void ForceField::start()
{
	if (gameObject != nullptr) soundEmitter = gameObject->getComponent<SoundEmitter>();
	volume = 1.5;
	if (soundEmitter != nullptr) soundEmitter->setVolume(volume);
}

void ForceField::update(float deltaTime)
{
	if (volume > 0 && GameManager::GetInstance()->isGameEnded() && soundEmitter != nullptr)
	{
		volume = 0;
		soundEmitter->setVolume(0);
	}
}

void ForceField::handleData(ComponentData* data)
{
	if (data == nullptr) return;

	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "targetVelocity")
		{
			if (!(ss >> targetVelocity))
				LOG("FORCE FIELD: Invalid value for property with name \"%s\"", prop.first.c_str());
		}
		else if (prop.first == "acceleration")
		{
			if (!(ss >> acceleration))
				LOG("FORCE FIELD: Invalid value for property with name \"%s\"", prop.first.c_str());
		}
		else
			LOG("FORCE FIELD: Invalid property with name \"%s\"", prop.first.c_str());
	}
}

void ForceField::onObjectEnter(GameObject* other)
{
	if (other == nullptr) return;

	RigidBody* rigidBody = other->getComponent<RigidBody>();
	Ball* ball = other->getComponent<Ball>();

	if (ball == nullptr || rigidBody == nullptr) return;
	ball->setTargetVelocity(targetVelocity);
	ball->setAcceleration(acceleration);
	other->getComponent<Trail>()->setColour({ 0.0,1.0,1.0 }, 0.7);

	if (soundEmitter != nullptr) soundEmitter->playSound("Impulse_02");
}

void ForceField::setTargetVelocity(float targetVelocity)
{
	this->targetVelocity = targetVelocity;
}

void ForceField::setAcceleration(float acceleration)
{
	this->acceleration = acceleration;
}