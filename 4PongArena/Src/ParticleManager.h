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
	void playParticles(float time, const Vector3& position = Vector3::ZERO);
	void stop();

private:
	void createParticle(const std::string& particleName);
	std::string particleName;

protected:
	virtual void start();
	virtual void handleData(ComponentData* data);
	virtual void preUpdate(float deltaTime);
};

#endif