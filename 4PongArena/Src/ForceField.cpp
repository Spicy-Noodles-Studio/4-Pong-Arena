#include "ForceField.h"

#include <GameObject.h>
#include <sstream>
#include <Trail.h>
#include <ComponentRegister.h>
#include <SoundEmitter.h>

#include "Ball.h"
#include "GameManager.h"


REGISTER_FACTORY(ForceField);

ForceField::ForceField(GameObject* gameObject) : UserComponent(gameObject), soundEmitter(nullptr), targetVelocity(0.0f), acceleration(0.0f), volume(0.0f)
{

}

ForceField::~ForceField()
{
	soundEmitter = nullptr;
}

void ForceField::start()
{
	checkNullAndBreak(gameObject);
	volume = 1.5;
	soundEmitter = gameObject->getComponent<SoundEmitter>();
	if (notNull(soundEmitter)) soundEmitter->setVolume(volume);
}

void ForceField::update(float deltaTime)
{
	GameManager* gameManager = GameManager::GetInstance();
	if (volume > 0 && notNull(gameManager) && gameManager->isGameEnded() && notNull(soundEmitter))
	{
		volume = 0;
		soundEmitter->setVolume(0);
	}
}

void ForceField::handleData(ComponentData* data)
{
	checkNullAndBreak(data);

	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "targetVelocity")
		{
			setFloat(targetVelocity);
		}
		else if (prop.first == "acceleration")
		{
			setFloat(acceleration);
		}
		else
			LOG("FORCE FIELD: Invalid property with name \"%s\"", prop.first.c_str());
	}
}

void ForceField::onObjectEnter(GameObject* other)
{
	checkNullAndBreak(other);

	Ball* ball = other->getComponent<Ball>();
	Trail* trail = other->getComponent<Trail>();

	if (notNull(ball)) {
		ball->setTargetVelocity(targetVelocity);
		ball->setAcceleration(acceleration);
	}
	if (notNull(trail)) trail->setColour({ 0.0,1.0,1.0 }, 0.7);

	if (notNull(soundEmitter)) soundEmitter->playSound("Impulse_02");
}

void ForceField::setTargetVelocity(float targetVelocity)
{
	this->targetVelocity = targetVelocity;
}

void ForceField::setAcceleration(float acceleration)
{
	this->acceleration = acceleration;
}