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
	checkNullAndBreak(gameObject);

	if (soundEmitter == nullptr) {
		soundEmitter = gameObject->getComponent<SoundEmitter>();
	}
	dontDestroyOnLoad(gameObject);
}

void MusicEmitter::playMusic(const std::string& sound)
{
	if (notNull(soundEmitter)) soundEmitter->playMusic(sound);
}

void MusicEmitter::stopMusic(const std::string& sound)
{
	if (notNull(soundEmitter)) soundEmitter->stop(sound);
}

void MusicEmitter::stopMusicAll()
{
	if (notNull(soundEmitter)) soundEmitter->stopAll();
}

void MusicEmitter::setVolume(float volume)
{
	if (notNull(soundEmitter)) soundEmitter->setVolume(volume);
}

