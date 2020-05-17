#include "ParticleManager.h"
#include <ComponentRegister.h>
#include <GameObject.h>
#include <ParticleEmitter.h>
#include <sstream>


REGISTER_FACTORY(ParticleManager);

ParticleManager::ParticleManager(GameObject* gameObject) :	UserComponent(gameObject), particleEmi(nullptr)
{

}

ParticleManager::~ParticleManager()
{

}

void ParticleManager::start()
{
	createParticle(particleName);
}

void ParticleManager::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);
		
		if (prop.first == "particleName")
		{
			if (!(ss >> particleName ))
				LOG("PARTICLE MANAGER: Invalid value for property with name \"%s\"", prop.first.c_str());
			
		}
		else
			LOG("PARTICLE MANAGER: Invalid property name \"%s\"", prop.first.c_str());
	}
}

void ParticleManager::playParticles(float time, const Vector3& position)
{
	duration = time;
	initialTime = 0;
	particlesObject->transform->setPosition(position);
	particleEmi->start();
	
}

void ParticleManager::preUpdate(float deltaTime)
{
	initialTime += deltaTime;
	if (duration <= initialTime)
	{
		particleEmi->stop();
		//initialTime = 0;
	}
}

void ParticleManager::stop()
{
	particleEmi->stop();
}



void ParticleManager::createParticle(const std::string& particleName)
{
	
	particlesObject = instantiate("ParticleEmitter");
	if (particlesObject == nullptr)
		return;

	// Add child
	gameObject->addChild(particlesObject);

	particleEmi = particlesObject->getComponent<ParticleEmitter>();
	if (particleEmi == nullptr)
		return;

	particleEmi->newEmitter(particleName);
}

