#pragma once
#ifndef MUSIC_EMITTER
#define MUSIC_EMITTER
#include <UserComponent.h>

class SoundEmitter;

class MusicEmitter : public UserComponent
{
public:

private:
	SoundEmitter* soundEmitter;

public:
	MusicEmitter(GameObject* gameObject);
	virtual ~MusicEmitter();

	virtual void start();
	virtual void update(float deltaTime);

	void playMusic(const std::string& sound);
	void stopMusic(const std::string& sound);
	void stopMusicAll();
};

#endif