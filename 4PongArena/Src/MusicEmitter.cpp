#include "MusicEmitter.h"
#include <ComponentRegister.h>
#include <GameObject.h>
#include <SoundEmitter.h>
#include "GameManager.h"

REGISTER_FACTORY(MusicEmitter);

MusicEmitter::MusicEmitter(GameObject* gameObject) : UserComponent(gameObject)
{

}

MusicEmitter::~MusicEmitter()
{

}

void MusicEmitter::start()
{
	if (soundEmitter == nullptr) {
		soundEmitter = gameObject->getComponent<SoundEmitter>();
	}
	dontDestroyOnLoad(gameObject);
}

void MusicEmitter::update(float deltaTime)
{
}

void MusicEmitter::playMusic(const std::string& sound)
{
	soundEmitter->playMusic(sound);
}

void MusicEmitter::stopMusic(const std::string& sound)
{
	soundEmitter->stop(sound);
}

void MusicEmitter::stopMusicAll()
{
	soundEmitter->stopAll();
}

void MusicEmitter::setVolume(float volume)
{
	soundEmitter->setVolume(volume);
}

