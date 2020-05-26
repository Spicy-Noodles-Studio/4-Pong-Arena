#include "ParticleManager.h"
#include <ComponentRegister.h>
#include <GameObject.h>
#include <ParticleEmitter.h>
#include <sstream>

REGISTER_FACTORY(ParticleManager);

ParticleManager::ParticleManager(GameObject* gameObject) : UserComponent(gameObject), particleEmitter(nullptr), particlesObject(nullptr), duration (0.0f), initialTime (0.0f)
{

}

ParticleManager::~ParticleManager()
{
	particlesObject = nullptr;
	particleEmitter = nullptr;
}

void ParticleManager::start()
{
	createParticle(particleName);
}

void ParticleManager::handleData(ComponentData* data)
{
	checkNullAndBreak(data);

	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "particleName")
		{
			setString(particleName);
		}
		else
			LOG("PARTICLE MANAGER: Invalid property name \"%s\"", prop.first.c_str());
	}
}

void ParticleManager::playParticles(float time, const Vector3& position)
{
	duration = time;
	initialTime = 0;
	if (notNull(particlesObject) && notNull(particlesObject->transform)) particlesObject->transform->setPosition(position);
	if (notNull(particleEmitter)) particleEmitter->start();

}

void ParticleManager::preUpdate(float deltaTime)
{
	initialTime += deltaTime;
	if (duration <= initialTime && notNull(particleEmitter)) particleEmitter->stop();
}

void ParticleManager::stop()
{
	if (notNull(particleEmitter)) particleEmitter->stop();
}

void ParticleManager::createParticle(const std::string& particleName)
{
	particlesObject = instantiate("ParticleEmitter");
	checkNullAndBreak(particlesObject);

	// Add child
	if (notNull(gameObject)) gameObject->addChild(particlesObject);

	particleEmitter = particlesObject->getComponent<ParticleEmitter>();
	checkNullAndBreak(particleEmitter);

	particleEmitter->newEmitter(particleName);
}