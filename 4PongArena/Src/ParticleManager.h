#pragma once
#ifndef PARTICLE_MANAGER
#define PARTICLE_MANAGER
#include <UserComponent.h>

class ParticleEmitter;
class PlayerState;

class ParticleManager : public UserComponent
{
public:

private:

	GameObject* particlesObject;
	/* PARTICLE SYSTEMS */
	ParticleEmitter* particleEmi;
	
	float duration;
	float initialTime;
	

public:
	ParticleManager(GameObject* gameObject);
	virtual ~ParticleManager();

	virtual void start();
	virtual void handleData(ComponentData* data);

	void playParticles(float time, const Vector3& position = Vector3::ZERO);
	virtual void preUpdate(float deltaTime);
	void stop();


private:
	
	void createParticle(const std::string& particleName);
	std::string particleName;


	
};

#endif