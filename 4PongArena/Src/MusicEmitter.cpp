#include "MusicEmitter.h"
#include <ComponentRegister.h>
#include <GameObject.h>
#include <SoundEmitter.h>
#include "GameManager.h"

REGISTER_FACTORY(MusicEmitter);

MusicEmitter::MusicEmitter(GameObject* gameObject) : UserComponent(gameObject), soundEmitter (nullptr)
{

}

MusicEmitter::~MusicEmitter()
{
	soundEmitter = nullptr;
}

void MusicEmitter::start()
{
	if (soundEmitter == nullptr && gameObject != nullptr) {
		soundEmitter = gameObject->getComponent<SoundEmitter>();
	}
	dontDestroyOnLoad(gameObject);
}

void MusicEmitter::playMusic(const std::string& sound)
{
	if (soundEmitter != nullptr) soundEmitter->playMusic(sound);
}

void MusicEmitter::stopMusic(const std::string& sound)
{
	if (soundEmitter != nullptr) soundEmitter->stop(sound);
}

void MusicEmitter::stopMusicAll()
{
	if (soundEmitter != nullptr) soundEmitter->stopAll();
}

void MusicEmitter::setVolume(float volume)
{
	if (soundEmitter != nullptr) soundEmitter->setVolume(volume);
}

